#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct s_data {
	pthread_mutex_t mutex;
	int exit;
} t_data;

void *print_numbers(void *arg)
{
    int i;
	int exit;
	t_data *data = (t_data *)arg;

	//printf("thread 1 started\n");
    //sleep(10);
	pthread_mutex_lock(&data->mutex);
    printf("Numbers: ");
    for (i = 1; i <= 10; i++) {
        printf("%d ", i);
    }
    printf("\n");
    pthread_mutex_unlock(&data->mutex);
    exit = 2;
	return ("error");
}

void *print_alphabet(void *arg)
{
    char c;
	t_data *data = (t_data *)arg;
	
    printf("%i\n", pthread_mutex_lock(&data->mutex));
    printf("Alphabet: ");
    for (c = 'A'; c <= 'Z'; c++) {
        printf("%c ", c);
    }
    printf("\n");
    pthread_mutex_unlock(&data->mutex);
    return NULL;
}

int main()
{
	pthread_mutex_t mutex;
    pthread_t tid1, tid2;
	t_data data; 

	pthread_mutex_init(&data.mutex, NULL);
    pthread_create(&tid1, NULL, print_numbers, (void *)&data);
    pthread_create(&tid2, NULL, print_alphabet, (void *)&data);
    pthread_join(tid1, NULL);
    //printf("thread exited with %s\n", exit);
	pthread_join(tid2, NULL);
    pthread_mutex_destroy(&mutex);
    return 0;
}

