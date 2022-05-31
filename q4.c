#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include "q4.h"

void q4(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "You should give at least one parameter!!!\n");
		return;
	}
	int *params = (int*) malloc((argc - 1) * sizeof(int)); // Allocating array of numbers

	for (int i = 1; i < argc; i++) {
		sscanf(argv[i], "%d", &params[i - 1]); // Convert from string to int and save in the array
	}
	pid_t pid;
	int j;
	int count = 0;
	int *vector = mmap(NULL, argc * sizeof(int), PROT_READ | PROT_WRITE,
	MAP_SHARED | MAP_ANONYMOUS, 0, 0);
//CHECK mmap success
	if (vector == MAP_FAILED) {  //check Success
		printf("Mapping Failed\n");
	}
	pid = fork();

	if (pid < 0) {
		fprintf(stderr, "Fork failed");
// Child Process
	} else if (pid == 0) {
		pthread_t tid_array[argc];
		pthread_attr_t attr;
		pthread_attr_init(&attr);

		for (j = 0; j < argc; j++) {
			pthread_create(&tid_array[j], &attr,
					collatz_q4(&params[j], &vector[j], &count), NULL);
		}
		for (j = 0; j < argc - 1; j++) {
			pthread_join(tid_array[j], NULL);
		}
		pthread_exit(NULL);
// Parent Process
	} else {
		wait(NULL);
		printf("\nThe vector is:");
		printVector(vector, argc);
	}

}

void* collatz_q4(int *num, int *vector, int *count) {
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

	printf("Last iteration(Thread %d): Number is now %d after %d iterations\n",
			*count, *num, k);
	*vector = k;
	(*count)++;
	return EXIT_SUCCESS;
}

void printVector(int *vector, int size) {
	printf("( ");
	fflush(stdin);
	for (int i = 0; i < size; ++i) {
		if (i != size - 1) {
			printf("%d, ", vector[i]);
			fflush(stdin);
		} else {
			printf("%d ", vector[i]);
			fflush(stdin);
		}
	}
	printf(")\n");
}

