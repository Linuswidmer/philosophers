/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwidmer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 13:22:53 by lwidmer           #+#    #+#             */
/*   Updated: 2023/07/13 09:21:23 by lwidmer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_exit_code	init_mutex_forks(t_waiter *waiter, int num_forks)
{
	int	i;

	i = 0;
	waiter->arr_forks = malloc(sizeof(pthread_mutex_t) * waiter->data->num_philos);
	while (i < num_forks)
	{
		pthread_mutex_init(&(waiter->arr_forks[i]), NULL);
		// this can possibly fail as well
		i++;
	}
	return (SUCCESS);
}


t_exit_code	init_data(t_data **p_data, int argc, char **argv)
{
	static t_exit_code	exit_code;
	t_data				*data;

	data = malloc(sizeof(t_data));
	CHECK_MALLOC(data, exit_code);
	if (exit_code == SUCCESS)
	{
		data->num_philos = ft_atoi(argv[1]);
		data->time_to_die = ft_atoi(argv[2]) * 1000; 
		data->time_to_eat = ft_atoi(argv[3]) * 1000;
		data->time_to_sleep = ft_atoi(argv[4]) * 1000;
		data->num_forks = data->num_philos;
		data->time_sim_start = current_timestamp_ms();
		if (argc == 6)
			data->n_has_to_eat = ft_atoi(argv[5]);
		else
			data->n_has_to_eat = -1;
	}
	*p_data = data;
	return (exit_code);
}

t_exit_code init_mutex_print(t_data *data)
{
  pthread_mutex_init(&(data->print), NULL);
  // this can possibly fail as well
  return (SUCCESS);
}

/*
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
*/

t_exit_code	init_philos(t_philo **p_arr_philos, t_waiter *waiter, t_data *data)
{
	t_exit_code	exit_code;
	t_philo		*arr_philos;
	int			i;

	i = 0;
	exit_code = SUCCESS;
	arr_philos = malloc(sizeof(t_philo) * data->num_philos);
	CHECK_MALLOC(arr_philos, exit_code);
	if (exit_code == SUCCESS)
	{
		while (i < data->num_philos)
		{
			(arr_philos[i]).index = i;
			(arr_philos[i]).arr_forks = waiter->arr_forks;
			(arr_philos[i]).data = data;
			(arr_philos[i]).m_n_eaten = waiter->arr_m_n_eaten[i];
			(arr_philos[i]).n_eaten = waiter->arr_n_eaten[i];
			(arr_philos[i]).m_philo_status = waiter->arr_m_philo_status[i];
			(arr_philos[i]).philo_status = waiter->arr_philo_status[i];
			(arr_philos[i]).last_meal_ms = current_timestamp_ms(); // ihave to improve this
			(arr_philos[i]).print = waiter->print;
			i++;
		}
	}
	*p_arr_philos = arr_philos;
	return (exit_code);
}


t_exit_code	init_table(t_table **p_table, t_data *data, t_waiter *waiter, t_philo *arr_philos)
{
	static t_exit_code	exit_code;
	t_table				*table;

	table = malloc(sizeof(t_table));
	CHECK_MALLOC(table, exit_code);
	if (exit_code == SUCCESS)
	{
		table->data = data;
		table->waiter = waiter;
		table->arr_philos = arr_philos;
		table->arr_threads = malloc(sizeof(pthread_t) * data->num_philos);
		CHECK_MALLOC(table->arr_threads, exit_code);
		// need to free if fails
	}
	*p_table = table;
	return (exit_code);
}

void	cleanup(t_table *table, t_philo *arr_philos, t_waiter *waiter, t_data *data)
{
	if (data)
		free(data);
	else
		return ;
	/*
	if (waiter)
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
	-> need to change this to the waiter logic
	*/
	if (arr_philos)
		free (arr_philos);
	else
		return ;
	if (table)
		free(table);
}

t_exit_code	init_waiter(t_waiter **p_waiter, t_data *data)
{
	t_exit_code	exit_code;
	t_waiter	*waiter;
	int			i;

	i = 0;
	exit_code = 0;
	waiter = malloc(sizeof(t_waiter));
	waiter->arr_m_philo_status = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	waiter->arr_m_n_eaten = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	waiter->arr_philo_status = malloc(sizeof(int) * data->num_philos);
	waiter->arr_n_eaten = malloc(sizeof(int) * data->num_philos);
	waiter->data = data;
	init_mutex_forks(waiter, data->num_forks);
	pthread_mutex_init(&(waiter->print), NULL);
	//ON_SUCCESS(exit, UPDATE_EXIT(exit, init_mutex_forks(waiter, data->num_forks)));
	while(i < data->num_philos)
	{
		waiter->arr_philo_status[i] = 0;
		waiter->arr_n_eaten[i] = 0;
		pthread_mutex_init(&(waiter->arr_m_philo_status[i]), NULL);
		pthread_mutex_init(&(waiter->arr_m_n_eaten[i]), NULL);
		i++;
	}
	*p_waiter = waiter;
	return (exit_code);
}

t_exit_code	init(t_table **table, int argc, char **argv)
{
	t_exit_code	exit;
	t_data		*data;
	t_philo		*philos;
	t_waiter	*waiter;

	exit = SUCCESS;
	ON_SUCCESS(exit, UPDATE_EXIT(exit, init_data(&data, argc, argv)));
	ON_SUCCESS(exit, UPDATE_EXIT(exit, init_waiter(&waiter, data)));
	ON_SUCCESS(exit, UPDATE_EXIT(exit, init_philos(&philos, waiter, data)));
	ON_SUCCESS(exit, UPDATE_EXIT(exit, init_table(table, data, waiter, philos)));
	waiter->arr_philos = (*table)->arr_philos;
	if (exit != SUCCESS)
		cleanup(*table, philos, waiter, data);
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
