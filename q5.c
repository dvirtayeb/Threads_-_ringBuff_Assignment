#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#include "q5.h"


sem_t mutex;

void q5b(int argc, char **argv) {
	 sem_init(&mutex, 0, 1);
	 if (argc < 2) {
	 	fprintf(stderr, "You need to give main a minimum of 1 parameter!!!\n");
	 }
	 int *params = (int*) malloc((argc - 1) * sizeof(int)); // Allocating array of numbers

	 for (int i = 1; i < argc; i++) {
	 	sscanf(argv[i], "%d", &params[i - 1]); // Convert from string to int and save in the array
	 }

	 int j;
	 int vector[argc];
	 int count;
	 pthread_t tid_array[argc];
	 pthread_attr_t attr;
	 pthread_attr_init(&attr);

	 for (j = 0; j < argc; j++) {
		 pthread_create(&tid_array[j], &attr,
				 thread_collatz(&params[j], &vector[j], &count), NULL);
	 	printf("vector[%d] = %d\n", j, vector[j]);
	 }

	 for (j = 0; j < argc-1; j++) {
		 sleep(4);
	 	pthread_join(tid_array[j], NULL);
//	 	sleep(4);
	 }
	 printVector(vector, argc);
	 sem_destroy(&mutex);
}

void* thread_collatz(int *num, int *vector, int *count)
{
    //wait
    sem_wait(&mutex);
    printf("\nStart - Do not allow any other thread to get into the critical section ...\n");

    //critical section
    int k = 0;
    	//int pid = pthread_self();
    	while (*num > 1) {
    		if (*num % 2 == 0) {
    			*num = *num / 2;
    		} else {
    			if (*num == 1) {
    				break;
    			} else {
    				*num = (*num * 3) + 1;
    				printf("Number is now %d after %d iterations\n", *num, k);
    			}
    		}
    		k++;
    	}

    	printf("Last iteration: Number is now %d after %d iterations\n", *num, k);
    	*vector = k;
    	(*count)++;

    //signal
    printf("\nFinish - Exiting to allow another thread to enter...\n");
    sem_post(&mutex);
    return EXIT_SUCCESS;
}
