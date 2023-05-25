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

void	philo_sleep(void *philo)
{
	printf("begins sleeping\n");
	usleep(900000);
	printf("finished sleeping\n");
}

void	*philo_eat(void *philo)
{
	t_philo	*philosopher;

	philosopher = (t_philo *)philo;
	if (philosopher->index % 2 == 0)
		usleep(100000);
	pthread_mutex_lock(&(philosopher->mutex->arr_forks[0]));
	printf("%i begins eating\n", philosopher->index);
	usleep(philosopher->data->time_to_eat);
	printf("finished eating\n");
	pthread_mutex_unlock(&(philosopher->mutex->arr_forks[0]));
	philo_sleep(philosopher);
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
		pthread_create(&(table->arr_threads[i]), NULL, philo_eat, &(table->arr_philos)[i]);
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
