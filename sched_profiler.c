#include <stdio.h>
#include <stdlib.h>
#include "pthread.h"
#include <semaphore.h>
#include <string.h>

int THREADS = 4;
int BUFFER_SIZE = 10000000;

char *buffer;
int buffer_pointer = 0;

pthread_mutex_t mutex;

void *task(void *thread_id);
void resume_buffer();
void count_scheduled_threads(int* arr);
void print_scheduled_threads_summary(int *arr);
void print_memory_stats();

int 
main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "usage: sched_profiler <number of threads> <buffer size>\n");
        return -1;
    }

    THREADS = atoi(argv[1]);
    BUFFER_SIZE = atoi(argv[2]);
    
    if (THREADS < 0 || THREADS > 26) {
        fprintf(stderr,"%d must be > 0 and < 26\n",atoi(argv[1]));
        return -1;
    }

	long int i;
	pthread_t threads[THREADS];

    buffer = (char*) malloc(sizeof(char) * BUFFER_SIZE);

	pthread_mutex_init(&mutex, NULL);

	for (i = 0; i < THREADS; i++) {
		pthread_create(&threads[i], NULL, task, (void *)i);
	}

	for (i = 0; i < THREADS; i++) {
		pthread_join(threads[i], NULL);
	}

    pthread_mutex_destroy(&mutex);

	printf("%s", buffer);

    resume_buffer();

    printf("\n\n\n%s\n\n\n", buffer);

    int *scheduled_threads_summary = (int*) malloc(sizeof(int) * BUFFER_SIZE);

    count_scheduled_threads(scheduled_threads_summary);

    print_scheduled_threads_summary(scheduled_threads_summary);

    free(buffer);
	return 0;
}

/////////////////////////////////////////////////////////////
// Registra a thread que está sendo executada
void 
*task(void *thread_id)
{
    int id = (int)(long int) thread_id;

    while(buffer_pointer < BUFFER_SIZE) {
		pthread_mutex_lock(&mutex);
        buffer[buffer_pointer] = 'A' + id;
		buffer_pointer++;
		pthread_mutex_unlock(&mutex);
    }
    pthread_exit(0);
}

////////////////////////////////////////////////////////////////
// Printa memória global
void 
print_memory_stats() 
{
    printf("buffer_pointer: %d\n", buffer_pointer);
    printf("buffer_size: %d\n", BUFFER_SIZE);
    printf("buffer: %s\n", buffer);
}

////////////////////////////////////////////////////////////////
// Compacta o buffer
// Ex: AAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBB
// Output: AB
// { Todo: realocar o buffer }
void resume_buffer() {
    char current_letter, next_letter = 0;
    int new_buffer_pointer = 0;
    char *new_buffer = (char*) malloc(sizeof(char) * (BUFFER_SIZE)); // todo: alloc a small buffer and realloc if needs
    new_buffer[0] = buffer[0];

    for (int i = 0; i < buffer_pointer; i++) 
        if (buffer[i] != buffer[i+1])
            new_buffer[++new_buffer_pointer] = buffer[i+1];

    BUFFER_SIZE = new_buffer_pointer + 1;
    buffer = new_buffer;
    buffer_pointer = new_buffer_pointer;
}

/////////////////////////////////////////////////////////////
// Conta quantas threads foram escalonadas
void count_scheduled_threads(int *arr) 
{
    for (int i = 0; i < BUFFER_SIZE; i++) arr[buffer[i] - 'A']++;
}

/////////////////////////////////////////////////////////////
// Printa quantas threads foram escalonadas
void print_scheduled_threads_summary(int *arr) 
{
    for (int i = 0; i < THREADS; i++) printf("%c: %d\n", ('A' + i), arr[i]);
}

////////////////////////////////////////////
// Desalocar o buffer
// printf("BUFFER_SIZE: %d\n", BUFFER_SIZE);

// buffer = (char*)realloc(buffer, sizeof(char) * BUFFER_SIZE);

// for (int i = 0; i < BUFFER_SIZE; i++) {
//     buffer[i] = new_buffer[i];
// }

// free(new_buffer);