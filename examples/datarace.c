/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   datarace.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lwidmer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 16:26:40 by lwidmer           #+#    #+#             */
/*   Updated: 2023/07/07 16:37:47 by lwidmer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct s_mutex {
	pthread_mutex_t print;
}	t_mutex;

typedef struct s_data {
	t_mutex *t_mutex;
	int exit;
} t_data;

void *print_numbers(void *arg)
{
    int i;
	int exit;
	t_data *data = (t_data *)arg;

	//printf("thread 1 started\n");
    //sleep(10);
	pthread_mutex_lock(&data->t_mutex->print);
    printf("Numbers: ");
    for (i = 1; i <= 10; i++) {
        printf("%d ", i);
    }
    printf("\n");
    pthread_mutex_unlock(&data->t_mutex->print);
    exit = 2;
	return ("error");
}

void *print_alphabet(void *arg)
{
    char c;
	t_data *data = (t_data *)arg;
	
    printf("%i\n", pthread_mutex_lock(&data->t_mutex->print));
    printf("Alphabet: ");
    for (c = 'A'; c <= 'Z'; c++) {
        printf("%c ", c);
    }
    printf("\n");
    pthread_mutex_unlock(&data->t_mutex->print);
    return NULL;
}

int main()
{
	pthread_mutex_t mutex;
    pthread_t tid1, tid2;
	t_data *data;

	data = malloc(sizeof(t_data) * 1);
	data->t_mutex = malloc(sizeof(t_mutex) * 1);
	pthread_mutex_init(&data->t_mutex->print, NULL);
    pthread_create(&tid1, NULL, print_numbers, (void *)data);
    pthread_create(&tid2, NULL, print_alphabet, (void *)data);
    pthread_join(tid1, NULL);
    //printf("thread exited with %s\n", exit);
	pthread_join(tid2, NULL);
    pthread_mutex_destroy(&mutex);
    return 0;
}

