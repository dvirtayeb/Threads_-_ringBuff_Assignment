#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/wait.h>
#include<pthread.h>
#include <stdint.h>
#include <fcntl.h>
#include "q3.h"


void q3(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "You need to give main a minimum of 1 parameter!!!\n");
	}
	int *params = (int*) malloc((argc - 1) * sizeof(int)); // Allocating array of numbers

	for (int i = 1; i < argc; i++) {
		sscanf(argv[i], "%d", &params[i - 1]); // Convert from string to int and save in the array
	}

	int j;
	int vector[argc];

	pthread_t tid_array[argc];
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	for (j = 0; j < argc; j++) {
		pthread_create(&tid_array[j], &attr, collatz(params[j], vector, j),
				NULL);
		printf("vector[%d] = %d\n", j, vector[j]);
	}
	for (j = 0; j < argc-1; j++) {
		pthread_join(tid_array[j], NULL);
	}

	for (j = 0; j < argc; j++) {
		printf("vector[%d] = %d\n", j, vector[j]);
	}
	 pthread_exit(NULL);

}

void* collatz(int num, int *vector, int index) {
	int k = 0;

	while (num > 1) {
		if (num % 2 == 0) {
			num = num / 2;
			k += 1;
		} else {
			if (num == 1) {
				break;
			} else {
				num = (num * 3) + 1;
				k++;
			}
		}
	}
	if (num == 1) {
		printf("Number is now %d after %d iterations\n", num, k);
	}
	vector[index] = k;
	return EXIT_SUCCESS;
}

