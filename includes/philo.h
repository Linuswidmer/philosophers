#ifndef PHILO_H
# define PHILO_H

/* INCLUDES */

#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>



/* STRUCTURES */
typedef struct s_data {
	int num_philos;
	int time_to_eat;
	int	time_to_sleep;
	int	time_to_die;
	int	num_forks;
	int	n_has_to_eat;
}	t_data;

typedef struct s_mutex {
	pthread_mutex_t	*arr_forks;
	pthread_mutex_t	print;
}	t_mutex;

typedef struct s_philo {
	int	index;
	t_data *data;
	t_mutex *mutex;
	int	n_eaten;
}	t_philo;

typedef struct	s_table {
	t_data	*data;
	t_mutex *mutex;
	t_philo *arr_philos;
	pthread_t	*arr_threads;
} t_table;

typedef enum {
	SUCCESS,
	MALLOC_FAIL,
	INPUT_FAIL,
	INIT_FAIL
}	t_exit_code;

/* FUNCTIONS */
int ft_atoi(const char *nptr);

/* init.c */
t_exit_code	init(t_table **table, int argc, char **argv);

/* MACROS */

#define ON_SUCCESS(exit_code, function) ((exit_code) == SUCCESS ? (function) : 0)
#define UPDATE_EXIT(e, r) ((e) = ((e) == SUCCESS && (r) != SUCCESS) ? (r) : (e))
#define CHECK_MALLOC(ptr, e) ((ptr) ? 0 : (UPDATE_EXIT((e), MALLOC_FAIL)))

# endif
