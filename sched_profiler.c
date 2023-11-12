#include <stdio.h>
#include <stdlib.h>
#include "pthread.h"
#include <semaphore.h>
#include <string.h>

#define THREADS 4

int BUFFER_SIZE = 1024 * 1024;

char *buffer;
int buffer_pointer;

pthread_mutex_t mutex;

void *task(void *thread_id);
void resume_buffer();
void count_scheduled_threads(int* arr);
void print_scheduled_threads_summary(int *arr);
void print_current_buffer();

int main(void)
{
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

	printf("%s\n", buffer);

	printf("=======================\n");

    resume_buffer();

	printf("%s\n", buffer);
	printf("%c\n", buffer[BUFFER_SIZE - 1]);

    print_current_buffer();


    // int scheduled_threads_summary[THREADS];

    // count_scheduled_threads(scheduled_threads_summary);

    // print_scheduled_threads_summary(scheduled_threads_summary);

    free(buffer);
	return 0;
}

void *task(void *thread_id)
{
    int id;
	id = (int)(long int) thread_id;

    while(buffer_pointer < BUFFER_SIZE) {
        buffer[buffer_pointer] = 'A' + id;

		pthread_mutex_lock(&mutex);
		buffer_pointer++;
		pthread_mutex_unlock(&mutex);
    }
    pthread_exit(0);
}

void resume_buffer() {
    char current_letter, next_letter;
    int new_buffer_pointer = 0;
    
    char *new_buffer = (char*) malloc(sizeof(char) * (BUFFER_SIZE)); // todo: alloc a small buffer and realloc if needs
    new_buffer_pointer = 0;
    new_buffer[0] = buffer[0];
    printf("first_letter: %c\n", new_buffer[0]);
    printf("buffer_pointer: %d\n", buffer_pointer);
    printf("buffer_size: %d\n", BUFFER_SIZE);

    for (int i = 0; i < buffer_pointer; i++) 
        if (buffer[i] != buffer[i+1])  {
            printf("Current Letter %c\n", buffer[i+1]);
            new_buffer[++new_buffer_pointer] = buffer[i+1];

        }


    printf("new_buffer_pointer: %d\n", new_buffer_pointer);

    BUFFER_SIZE = new_buffer_pointer + 1;

    printf("BUFFER_SIZE: %d\n", BUFFER_SIZE);

    // buffer = (char*)realloc(buffer, sizeof(char) * BUFFER_SIZE);

    // for (int i = 0; i < BUFFER_SIZE; i++) {
    //     buffer[i] = new_buffer[i];
    // }

    // free(new_buffer);
    buffer = new_buffer;
    buffer_pointer = new_buffer_pointer;
}

void print_current_buffer() {
    for (int i = 0; i < BUFFER_SIZE; i++) {
        printf("print buffer: %c", buffer[i]);
    }
}

void count_scheduled_threads(int *arr) {
    int i;
    // for (i = 0; i < BUFFER_SIZE; i++) {
    //     printf("%c",  buffer[i]);
    // }

    for (i = 0; i < BUFFER_SIZE; i++) {
        int letter_index = buffer[i] - 'A';
        arr[letter_index]++;
    }
    
    for (i = 0; i < BUFFER_SIZE; i++) {
        printf("%c",  arr[i]);
    }
}

void print_scheduled_threads_summary(int *arr) {
    int i;
    for (i = 0; i < 26; i++) {
        if (arr[i] == 0) {
            printf("%c: %d\n", ('A' + i), arr[i]);
        }
    }
}


//gcc app.c -O2 -o appGcc -lpthread
//taskset -c 0 ./appGcc
