/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwidmer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 11:32:39 by lwidmer           #+#    #+#             */
/*   Updated: 2023/05/24 18:00:52 by lwidmer          ###   ########.fr       */
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
		data->num_forks = data->num_philos - 1;
		if (argc == 6)
			data->n_has_to_eat = ft_atoi(argv[5]);
		else
			data->n_has_to_eat = -1;
	}
	*p_data = data;
	return (exit_code);
}

t_exit_code	init_mutex(t_mutex **p_mutex, t_data *data)
{
	static t_exit_code		exit_code;
	t_mutex					*mutex;
	pthread_mutex_t			*arr_forks;
	pthread_mutex_t			print;

	mutex = malloc(sizeof(t_mutex));
	CHECK_MALLOC(mutex, exit_code);
	if (exit_code == SUCCESS)
	{
		mutex->arr_forks = malloc(sizeof(pthread_mutex_t) * data->num_forks);
		//printf("mutex pointer is %p\n", mutex->arr_forks);
		CHECK_MALLOC(mutex->arr_forks, exit_code);
	}
	*p_mutex = mutex;
	return (exit_code);
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
	}
	*p_table = table;
	return (exit_code);
}

/*
void init_check_exit(t_exit_code exit)
{
	
}
*/

void	cleanup(t_table *table, t_philos *arr_philos, t_mutex *mutex, t_data *data)

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
	if (exit == SUCCESS)
		return (SUCCESS);
	else
	{
		cleanup(*table, philos, mutex, data);
		return (exit);
	}
}

void	print_error_msg(t_exit_code exit_code)
{
	if (exit_code == MALLOC_FAIL)
		printf("Dynamic memory allocation failed\n");
	else if (exit_code == INPUT_FAIL)
		printf("Invalid Input\n");
	else if (exit_code == INIT_FAIL)
		printf("Fail in initialization\n");
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

int main(int argc, char **argv)
{
	t_table *table;
	t_exit_code exit_code;

	// check_input(argc, argv)
	exit_code = init(&table, argc, argv);
	//printf("mutex pointer is %p\n", table->mutex->arr_forks);

	//print_input(table->data, table->arr_philos);	
	// print EXIT OF PROGRAM, if SUCCESS print nothing
	if (exit_code != SUCCESS)
		print_error_msg(exit_code);
}
