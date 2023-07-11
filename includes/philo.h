#ifndef PHILO_H
# define PHILO_H

/* INCLUDES */

#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>

/* STRUCTURES */
typedef struct s_data {
	pthread_mutex_t	print;
	int num_philos;
	int time_to_eat;
	int	time_to_sleep;
	int	time_to_die;
	int	num_forks;
	int	n_has_to_eat;
}	t_data;

/*
typedef struct s_mutex {
	pthread_mutex_t	*arr_forks;
	pthread_mutex_t	print;
	pthread_mutex_t sim_protect;
	int sim;
}	t_mutex;
*/

typedef struct s_waiter {
	pthread_mutex_t	print;
	pthread_mutex_t	*arr_m_philo_status;
	int				*arr_philo_status;
	pthread_mutex_t	*arr_m_n_eaten;
	int				*arr_n_eaten;
	pthread_mutex_t	*arr_forks;
}	t_waiter;

typedef struct s_philo {
	int				index;
	t_data			*data;
	pthread_mutex_t	*arr_forks;
	pthread_mutex_t	print;
	pthread_mutex_t	m_philo_status;
	int				philo_status;
	pthread_mutex_t	m_n_eaten;
	int				n_eaten;
	long long		last_meal_ms;
}	t_philo;

typedef struct	s_table {
	t_data		*data;
	t_philo		*arr_philos;
	pthread_t	*arr_threads;
} t_table;

typedef enum {
	SUCCESS,
	MALLOC_FAIL,
	INPUT_FAIL,
	INIT_FAIL
}	t_exit_code;

typedef enum {
  EAT,
  SLEEP,
  THINK,
  DIE,
  FORK
} t_action;

/* FUNCTIONS */
int ft_atoi(const char *nptr);

/* init.c */
t_exit_code	init(t_table **table, int argc, char **argv);

/*time.c */
long long current_timestamp_ms(void); 

/* MACROS */

#define ON_SUCCESS(exit_code, function) ((exit_code) == SUCCESS ? (function) : 0)
#define UPDATE_EXIT(e, r) ((e) = ((e) == SUCCESS && (r) != SUCCESS) ? (r) : (e))
#define CHECK_MALLOC(ptr, e) ((ptr) ? 0 : (UPDATE_EXIT((e), MALLOC_FAIL)))

# endif
