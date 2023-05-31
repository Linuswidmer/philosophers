/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwidmer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 11:32:39 by lwidmer           #+#    #+#             */
/*   Updated: 2023/05/25 14:15:04 by lwidmer          ###   ########.fr       */
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
    return (1);
  }
  else
  {
    pthread_mutex_unlock(&(philo)->mutex->sim_protect);
    return (0);
  }
}

long long current_timestamp_ms(void) {
  struct timeval te;
  long long milliseconds;
    
  gettimeofday(&te, NULL);
  // this can probably fail as well
  milliseconds = te.tv_sec * 1000LL + te.tv_usec / 1000;
  return (milliseconds);
}

void  *print_action(t_philo *philo, t_action action)
{
  int philo_index;
  long long time;

  philo_index = philo->index;
  pthread_mutex_lock(&(philo->mutex->print));
  if (check_sim_status(philo) == 1)
    return (NULL);
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

int philo_died(t_philo *philo)
{
  long long time;
  
  pthread_mutex_unlock(&(philo->mutex->arr_forks[0]));
  pthread_mutex_lock(&(philo)->mutex->sim_protect);

  time = current_timestamp_ms();
  if (check_sim_status(philo) == 0)
    printf("%lld %i died\n", time, philo->index);
  philo->mutex->sim = 1;
  pthread_mutex_unlock(&(philo)->mutex->sim_protect);
  print_action(philo, DIE);
  return (1);
}

int is_philo_dead(t_philo *philo)
{
  long long time_now;
  long long ms_since_last_meal;

  time_now = current_timestamp_ms();
  ms_since_last_meal = time_now - philo->last_meal_ms;
  if (ms_since_last_meal > philo->data->time_to_die)
    return (philo_died(philo));
  else
    return (0);
}

void	*philo_eat(t_philo *philo)
{
  if (check_sim_status(philo) == 1)
    return (NULL);
	// if (philo->index % 2 == 0)
	// 	usleep(100000);
  // have here a strategy that depending on the modulo of the index
  // will check the right fork first or the left fork first
	pthread_mutex_lock(&(philo->mutex->arr_forks[0]));
  if (is_philo_dead(philo) == 1)
    return (NULL);
	print_action(philo, FORK);
  print_action(philo, EAT);
	usleep(philo->data->time_to_eat);
	pthread_mutex_unlock(&(philo->mutex->arr_forks[0]));
	philo_sleep(philo);
	return (NULL);
}

void *start_simulation(void *philo)
{
	t_philo	*philosopher;

	philosopher = (t_philo *)philo;
  philosopher->last_meal_ms = current_timestamp_ms();
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
