/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwidmer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 13:22:53 by lwidmer           #+#    #+#             */
/*   Updated: 2023/05/25 14:52:19 by lwidmer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_exit_code	init_data(t_data **p_data, int argc, char **argv)
{
	static t_exit_code	exit_code;
	t_data				*data;

	data = malloc(sizeof(t_data));
	CHECK_MALLOC(data, exit_code);
	if (exit_code == SUCCESS)
	{
		data->num_philos = ft_atoi(argv[1]);
		data->time_to_die = ft_atoi(argv[2]); 
		data->time_to_eat = ft_atoi(argv[3]);
		data->time_to_sleep = ft_atoi(argv[4]);
		data->num_forks = data->num_philos;
		if (argc == 6)
			data->n_has_to_eat = ft_atoi(argv[5]);
		else
			data->n_has_to_eat = -1;
	}
	*p_data = data;
	return (exit_code);
}

t_exit_code	init_mutex_forks(t_mutex *mutex, int num_forks)
{
	int	i;

	i = 0;
	while (i < num_forks)
	{
		pthread_mutex_init(&(mutex->arr_forks[i]), NULL);
		// this can possibly fail as well
		i++;
	}
	return (SUCCESS);
}

t_exit_code init_mutex_print(t_mutex *mutex)
{
  pthread_mutex_init(&(mutex->print), NULL);
  // this can possibly fail as well
  return (SUCCESS);
}

t_exit_code init_mutex_simulation(t_mutex *mutex)
{
  pthread_mutex_init(&(mutex->sim_protect), NULL);
  mutex->sim = 0;
  // this can possibly fail as well
  return (SUCCESS);
}

t_exit_code	init_mutex(t_mutex **p_mutex, t_data *data)
{
	static t_exit_code		exit;
	t_mutex					*mutex;
	pthread_mutex_t			*arr_forks;
	pthread_mutex_t			print;

	mutex = malloc(sizeof(t_mutex));
	CHECK_MALLOC(mutex, exit);
	if (exit == SUCCESS)
	{
		mutex->arr_forks = malloc(sizeof(pthread_mutex_t) * data->num_forks);
		//printf("mutex pointer is %p\n", mutex->arr_forks);
		CHECK_MALLOC(mutex->arr_forks, exit);
		ON_SUCCESS(exit, UPDATE_EXIT(exit, init_mutex_forks(mutex, data->num_forks)));
		// still needs protection
		ON_SUCCESS(exit, UPDATE_EXIT(exit, init_mutex_print(mutex)));
		// still needs protection
		ON_SUCCESS(exit, UPDATE_EXIT(exit, init_mutex_simulation(mutex)));
		// still needs protection
	}
	*p_mutex = mutex;
	return (exit);
}

t_exit_code	init_philos(t_philo **p_arr_philos, t_mutex *mutex, t_data *data)
{
	static		t_exit_code	exit_code;
	t_philo		*arr_philos;
	int			i;

	i = 0;
	arr_philos = malloc(sizeof(t_philo) * data->num_philos);
	CHECK_MALLOC(arr_philos, exit_code);
	if (exit_code == SUCCESS)
	{
		while (i < data->num_philos)
		{
			(arr_philos[i]).index = i + 1;
			(arr_philos[i]).mutex = mutex;
			(arr_philos[i]).data = data;
			(arr_philos[i]).n_eaten = 0;
			i++;
		}
	}
	*p_arr_philos = arr_philos;
	return (exit_code);
}


t_exit_code	init_table(t_table **p_table, t_data *data, t_mutex *mutex, t_philo *arr_philos)
{
	static t_exit_code	exit_code;
	t_table				*table;

	table = malloc(sizeof(t_table));
	CHECK_MALLOC(table, exit_code);
	if (exit_code == SUCCESS)
	{
		table->mutex = mutex;
		table->data = data;
		table->arr_philos = arr_philos;
		table->arr_threads = malloc(sizeof(pthread_t) * data->num_philos);
		CHECK_MALLOC(table->arr_threads, exit_code);
		// need to free if fails
	}
	*p_table = table;
	return (exit_code);
}

void	cleanup(t_table *table, t_philo *arr_philos, t_mutex *mutex, t_data *data)
{
	if (data)
		free(data);
	else
		return ;
	if (mutex)
	{
		if (mutex->arr_forks)
		{
			free(mutex->arr_forks);
			free(mutex);
		}
		else
		{
			free(mutex);
			return ;
		}
	}
	else 
		return ;
	if (arr_philos)
		free (arr_philos);
	else
		return ;
	if (table)
		free(table);
}

t_exit_code	init(t_table **table, int argc, char **argv)
{
	t_exit_code	exit;
	t_data *data;
	t_mutex *mutex;
	t_philo *philos;

	exit = SUCCESS;
	ON_SUCCESS(exit, UPDATE_EXIT(exit, init_data(&data, argc, argv)));
	ON_SUCCESS(exit, UPDATE_EXIT(exit, init_mutex(&mutex, data)));
	ON_SUCCESS(exit, UPDATE_EXIT(exit, init_philos(&philos, mutex, data)));
	ON_SUCCESS(exit, UPDATE_EXIT(exit, init_table(table, data, mutex, philos)));
	if (exit != SUCCESS)
		cleanup(*table, philos, mutex, data);
	return (exit);
}

void print_input(t_data *data, t_philo  *arr_philos)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		printf("Philosopher num %i, index: %i\n", i + 1, (arr_philos[i]).index);
		printf("Philosopher num %i, num_eaten: %i\n", i + 1, (arr_philos[i]).n_eaten);
		i++;
	}
}
