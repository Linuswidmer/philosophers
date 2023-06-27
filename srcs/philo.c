/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwidmer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 11:32:39 by lwidmer           #+#    #+#             */
/*   Updated: 2023/06/27 18:05:12 by lwidmer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philo_eat(t_philo *philo);

int check_sim_status(t_philo *philo)
{
  pthread_mutex_lock(&(philo)->mutex->sim_protect);
  if (philo->mutex->sim == 1)
  {
    pthread_mutex_unlock(&(philo)->mutex->sim_protect);
    // printf("simulation ended\n");
    return (1);
  }
  else
  {
    pthread_mutex_unlock(&(philo)->mutex->sim_protect);
    return (0);
  }
}


void  print_action(t_philo *philo, t_action action)
{
  int philo_index;
  long long time;

  philo_index = philo->index;
  //if (check_sim_status(philo) == 1)
  //  return (NULL);
  pthread_mutex_lock(&(philo->mutex->print));
  time = current_timestamp_ms();
  if (action == EAT)
    printf("%lld %i is eating\n", time, philo_index);
  if (action == SLEEP)
    printf("%lld %i is sleeping\n", time, philo_index);
  if (action == THINK)
    printf("%lld %i is thinking\n", time, philo_index);
  if (action == FORK)
    printf("%lld %i has taken a fork\n", time, philo_index);
  pthread_mutex_unlock(&(philo->mutex->print));
}

void *philo_think(t_philo *philo)
{
  if (check_sim_status(philo) == 1)
    return (NULL);
  print_action(philo, THINK);
  philo_eat(philo);
}

void	*philo_sleep(t_philo *philo)
{
  if (check_sim_status(philo) == 1)
    return (NULL);
  print_action(philo, SLEEP);
  usleep(philo->data->time_to_sleep);
  philo_think(philo);
}

int end_simulation(t_philo *philo)
{
  long long time;
  
  pthread_mutex_lock(&(philo)->mutex->sim_protect);
  time = current_timestamp_ms();
  if (philo->mutex->sim == 0)
  {
    philo->mutex->sim = 1;
    pthread_mutex_lock(&(philo->mutex->print));
    printf("%lld %i died\n", time, philo->index);
    pthread_mutex_unlock(&(philo->mutex->print));
    pthread_mutex_unlock(&(philo)->mutex->sim_protect);
  }
  else
    pthread_mutex_unlock(&(philo)->mutex->sim_protect);
}

int is_philo_dead(t_philo *philo)
{
  long long time_now;
  long long ms_since_last_meal;

  time_now = current_timestamp_ms();
  ms_since_last_meal = time_now - philo->last_meal_ms;
  if (ms_since_last_meal > philo->data->time_to_die)
    return (1);
  else
    return (0);
}

void philo_pickup_forks2(t_philo *philo)
{
	int philo_index;
	int last_philo;

	philo_index = philo->index;
	last_philo = philo->data->num_philos - 1;
	pthread_mutex_lock(&(philo->mutex->arr_forks[philo_index]));
	print_action(philo, FORK);
	if (philo_index == last_philo)
	{
		pthread_mutex_lock(&(philo->mutex->arr_forks[0]));
		// printf("philo %i took fork %i\n", philo_index, 0);
	}
	else
	{
		pthread_mutex_lock(&(philo->mutex->arr_forks[philo_index + 1]));
		// printf("philo %i took fork %i\n", philo_index, philo_index + 1);
	}
	print_action(philo, FORK);
}

void philo_pickup_forks(t_philo *philo)
{
  int philo_index;
  int last_philo;

  philo_index = philo->index;
  last_philo = philo->data->num_philos - 1;
  if (philo_index % 2 == 0)
  {
	  pthread_mutex_lock(&(philo->mutex->arr_forks[philo_index]));
    // printf("philo %i took fork %i\n", philo_index,philo_index);
    print_action(philo, FORK);
	  if (philo_index == last_philo)
    {
      pthread_mutex_lock(&(philo->mutex->arr_forks[0]));
      // printf("philo %i took fork %i\n", philo_index, 0);
    }
    else
    {
      pthread_mutex_lock(&(philo->mutex->arr_forks[philo_index + 1]));
      // printf("philo %i took fork %i\n", philo_index, philo_index + 1);
    }
  }
  else
  {
	  if (philo_index == last_philo)
    {
      pthread_mutex_lock(&(philo->mutex->arr_forks[0]));
      // printf("philo %i took fork %i\n", philo_index, 0);
    }
    else
    {
      pthread_mutex_lock(&(philo->mutex->arr_forks[philo_index + 1]));
      // printf("philo %i took fork %i\n", philo_index, philo_index + 1);
    }
    print_action(philo, FORK);
	  pthread_mutex_lock(&(philo->mutex->arr_forks[philo_index]));
    // printf("philo %i took fork %i\n", philo_index,philo_index);
    print_action(philo, FORK);
  }
}

void philo_drop_forks(t_philo *philo)
{
  int philo_index;
  int last_philo;

  philo_index = philo->index;
  last_philo = philo->data->num_philos - 1;

  if (philo_index % 2 == 0)
  {
	  pthread_mutex_unlock(&(philo->mutex->arr_forks[philo_index]));
	  if (philo_index == last_philo)
      pthread_mutex_unlock(&(philo->mutex->arr_forks[0]));
    else
      pthread_mutex_unlock(&(philo->mutex->arr_forks[philo_index + 1]));
  }
  else
  {
	  if (philo_index == last_philo)
      pthread_mutex_unlock(&(philo->mutex->arr_forks[0]));
    else
      pthread_mutex_unlock(&(philo->mutex->arr_forks[philo_index + 1]));
	  pthread_mutex_unlock(&(philo->mutex->arr_forks[philo_index]));
  }
}

void	*philo_eat(t_philo *philo)
{
	if (check_sim_status(philo) == 1)
		return (NULL);
	philo_pickup_forks2(philo);
	if (is_philo_dead(philo) == 1)
	{
		philo_drop_forks(philo);
		end_simulation(philo);
		return (NULL);
	}
	print_action(philo, EAT);
	usleep(philo->data->time_to_eat);
	philo_drop_forks(philo);
	philo_sleep(philo);
	return (NULL);
}

void *start_simulation(void *philo)
{
	t_philo	*philosopher;

	philosopher = (t_philo *)philo;
	philosopher->last_meal_ms = current_timestamp_ms();
	if (philosopher->index % 2 == 0)
		usleep(philosopher->data->time_to_eat /2);
	philo_eat(philosopher);
	return (NULL);
}

int	run_threads(t_table *table)
{
	int	i;
	int	n_philosophers;

	i = 0;
	n_philosophers = table->data->num_philos;
	while (i < n_philosophers)
	{
		pthread_create(&(table->arr_threads[i]), NULL, start_simulation,
                &(table->arr_philos)[i]);
		i++;
	}
	return (0);
}

int	wait_threads(t_table *table)
{
	int	i;
	int	n_philosophers;

	i = 0;
	n_philosophers = table->data->num_philos;
	while (i < n_philosophers)
	{
		pthread_join(table->arr_threads[i], NULL);
		i++;
	}
	return (0);
}

int main(int argc, char **argv)
{
	t_table *table;
	t_exit_code exit;
	pthread_t tid1, tid2;
	
	// check_input(argc, argv)
	exit = init(&table, argc, argv);

	run_threads(table);

	wait_threads(table);
	pthread_mutex_destroy(&(table->mutex->arr_forks[0]));
	//print_input(table->data, table->arr_philos);	
	// print EXIT OF PROGRAM, if SUCCESS print nothing
	//if (exit != SUCCESS)
	//	print_error_msg(exit);
}
