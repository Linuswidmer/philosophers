/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwidmer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 11:32:39 by lwidmer           #+#    #+#             */
/*   Updated: 2023/07/13 15:28:51 by lwidmer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philo_eat(t_philo *philo);
void	end_simulation(t_waiter *waiter, int index);

int	is_philo_dead(t_philo *philo)
{
	pthread_mutex_lock(&(philo->m_philo_status));
	//printf("p %i status is %i\n", philo->index + 1, philo->philo_status);
	if (philo->philo_status == 1)
	{
		printf("philo %i is dead\n", philo->index + 1);
		pthread_mutex_unlock(&(philo->m_philo_status));
		return (1);
	}
	else
	{
		pthread_mutex_unlock(&(philo->m_philo_status));
		return (0);	
	}
}

void  print_action(t_philo *philo, t_action action)
{
	int philo_index;
	long long time;

	philo_index = philo->index + 1;
	pthread_mutex_lock(&(philo->print));
	time = current_timestamp_ms() - philo->data->time_sim_start;
	if (action == EAT)
		printf("%lld %i is eating\n", time, philo_index);
	if (action == SLEEP)
		printf("%lld %i is sleeping\n", time, philo_index);
	if (action == THINK)
		printf("%lld %i is thinking\n", time, philo_index);
	if (action == FORK)
		printf("%lld %i has taken a fork\n", time, philo_index);
	pthread_mutex_unlock(&(philo->print));
}

void	*philo_think(t_philo *philo)
{
	if (is_philo_dead(philo) == 0)
	{
		print_action(philo, THINK);
		philo_eat(philo);
	}
	return (NULL);
}

void	*philo_sleep(t_philo *philo)
{
	if (is_philo_dead(philo) == 0)
	{
		print_action(philo, SLEEP);
		usleep(philo->data->time_to_sleep);
		philo_think(philo);
	}
	return (NULL);
}

/*
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
*/

void *philo_pickup_forks(t_philo *philo)
{
	int	philo_index;
	int	last_philo;

	if (is_philo_dead(philo) == 1)
		return (NULL);
	philo_index = philo->index;
	last_philo = philo->data->num_philos - 1;
	

	if (philo_index % 2 == 0)
	{
		pthread_mutex_lock(&(philo->arr_forks[(philo_index + 1) % philo->data->num_philos]));
		print_action(philo, FORK);
		pthread_mutex_lock(&(philo->arr_forks[philo_index]));
		print_action(philo, FORK);
	}
	else
	{
		pthread_mutex_lock(&(philo->arr_forks[philo_index]));
		print_action(philo, FORK);
		pthread_mutex_lock(&(philo->arr_forks[(philo_index + 1) % philo->data->num_philos]));
		print_action(philo, FORK);
	}
	/*
	if (philo_index % 2 != 0)
	{	
		pthread_mutex_lock(&(philo->arr_forks[philo_index]));
    // printf("philo %i took fork %i\n", philo_index,philo_index);
		print_action(philo, FORK);
		if (philo_index == last_philo)
		{
			pthread_mutex_lock(&(philo->arr_forks[0]));
			// printf("philo %i took fork %i\n", philo_index, 0);
		}
		else
		{
		pthread_mutex_lock(&(philo->arr_forks[philo_index + 1]));
		// printf("philo %i took fork %i\n", philo_index, philo_index + 1);
		}
		print_action(philo, FORK);
	}
	else
	{
		if (philo_index == last_philo)
		{
			pthread_mutex_lock(&(philo->arr_forks[0]));
			// printf("philo %i took fork %i\n", philo_index, 0);
		}
		else
		{
			pthread_mutex_lock(&(philo->arr_forks[philo_index + 1]));
			// printf("philo %i took fork %i\n", philo_index, philo_index + 1);
		}
		print_action(philo, FORK);
		pthread_mutex_lock(&(philo->arr_forks[philo_index]));
		// printf("philo %i took fork %i\n", philo_index,philo_index);
		print_action(philo, FORK);
	}
	*/
	return (NULL);
}

void philo_drop_forks(t_philo *philo)
{
  int philo_index;
  int last_philo;

  philo_index = philo->index;
  last_philo = philo->data->num_philos - 1;

  if (philo_index % 2 != 0)
  {
	  pthread_mutex_unlock(&(philo->arr_forks[philo_index]));
	  if (philo_index == last_philo)
      pthread_mutex_unlock(&(philo->arr_forks[0]));
    else
      pthread_mutex_unlock(&(philo->arr_forks[philo_index + 1]));
  }
  else
  {
	  if (philo_index == last_philo)
      pthread_mutex_unlock(&(philo->arr_forks[0]));
    else
      pthread_mutex_unlock(&(philo->arr_forks[philo_index + 1]));
	  pthread_mutex_unlock(&(philo->arr_forks[philo_index]));
  }
}

void	*philo_eat(t_philo *philo)
{
	if (is_philo_dead(philo) == 1)
		return (NULL);
	philo_pickup_forks(philo);
	if (is_philo_dead(philo) == 1)
	{
		philo_drop_forks(philo);
		return (NULL);
	}
	print_action(philo, EAT);
	philo->last_meal_ms = current_timestamp_ms();
	usleep(philo->data->time_to_eat);
	if (is_philo_dead(philo) == 1)
	{
		philo_drop_forks(philo);
		return (NULL);
	}
	pthread_mutex_lock(&(philo->m_n_eaten));
	printf("philo %i increasing eat\n", philo->index + 1);
	philo->n_eaten = philo->n_eaten + 1;
	pthread_mutex_unlock(&(philo->m_n_eaten));
	philo_drop_forks(philo);
	philo_sleep(philo);
	return (NULL);
}

void *start_simulation(void *philo)
{
	t_philo	*philosopher;

	philosopher = (t_philo *)philo;
	// do i still need this?
	/*
	if (philosopher->index % 2 != 0)
		usleep(philosopher->data->time_to_eat /2);
	*/
	philo_eat(philosopher);
	return (NULL);
}

void	end_simulation(t_waiter *waiter, int index)
{
	int			i;
	long long	time;

	i = 0;
	//printf("ending simulation\n");
	time = current_timestamp_ms();
	while (i < waiter->data->num_philos)
	{
		pthread_mutex_lock(&(waiter->arr_philos[i].m_philo_status));
		waiter->arr_philos[i].philo_status = 1;
		//printf("philo %i status is %i\n", i + 1, waiter->arr_philo_status[i]);
		pthread_mutex_unlock(&(waiter->arr_philos[i].m_philo_status));
		i++;
	}
	if (index >= 0)
	{
		pthread_mutex_lock(&(waiter->print));
		printf("%lld %i died\n", time - waiter->data->time_sim_start, index + 1);
		pthread_mutex_unlock(&(waiter->print));
	}
}

int check_philo_status(t_waiter *waiter)
{
	int			i;
	long long	time_now;
	long long	ms_since_last_meal;
	t_philo		*philo;

	i = 0;
	time_now = current_timestamp_ms();
	while (i < waiter->data->num_philos)
	{
		//write(2, "check philo status\n", 19);
		philo = &waiter->arr_philos[i];
  		ms_since_last_meal = time_now - philo->last_meal_ms;
		// i think i need a mutex also for this
		//printf("philo %i, ms since last meal is %lld \n",i + 1,  ms_since_last_meal);
		if (ms_since_last_meal > (waiter->data->time_to_die / 1000))
			return (philo->index);
		i++;
	}
	return (-1);
}

int check_n_eaten(t_waiter *waiter)
{
	int	i;

	i = 0;
	while (i < waiter->data->num_philos)
	{
		//printf("philo %i ate %i times\n", i + 1, waiter->arr_philos[i].n_eaten);
		pthread_mutex_lock(&(waiter->arr_philos[i].m_n_eaten));
		if (waiter->arr_philos[i].n_eaten >= waiter->data->n_has_to_eat)
		{
			pthread_mutex_unlock(&(waiter->arr_philos[i].m_n_eaten));
			i++;
		}
		else
		{
			pthread_mutex_unlock(&(waiter->arr_philos[i].m_n_eaten));
			break;
		}
	}
	printf("i is %i\n", i);
	if (i == waiter->data->num_philos)
	{
		printf("reached n_eaten\n");
		return (1);
	}
	else
		return (0);
}

void *monitor_threads(void *w)
{
	int			index;
	t_waiter	*waiter;

	waiter = (t_waiter *)w;
	while (1)
	{
		index = check_philo_status(waiter);
		//printf("return from check philo status is %i\n", index);
		if (index > 0)
		{
			end_simulation(waiter, index);
			break ;
		}
		if (waiter->data->n_has_to_eat >= 0)
		{
			if (check_n_eaten(waiter) == 1)
			{
				end_simulation(waiter, -1);
				break ;
			}
		}
		usleep (10000);
	}
	return (NULL);
}

int	run_threads(t_table *table)
{
	int	i;
	int	n_philosophers;

	i = 0;
	n_philosophers = table->data->num_philos;
	pthread_create(&(table->thread_waiter), NULL, monitor_threads,
                (table->waiter));
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
		printf("philo returned\n");
		i++;
	}
	printf("all philos returned\n");
	pthread_join(table->thread_waiter, NULL);
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
	pthread_mutex_destroy(&(table->arr_philos[0].arr_forks[0]));
	//print_input(table->data, table->arr_philos);	
	// print EXIT OF PROGRAM, if SUCCESS print nothing
	//if (exit != SUCCESS)
	//	print_error_msg(exit);
}
