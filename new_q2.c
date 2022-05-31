#include <stdio.h>
#include <sys/types.h> /* pid_t */
#include <unistd.h>  /* _exit, fork */
#include <stdlib.h>  /* exit */
#include <errno.h>   /* errno */
#include <sys/wait.h>
#include <sys/mman.h>
#include <stdint.h>
#include <fcntl.h>
#include "new_q2.h"

#define SIZE_OF_BUFFS 46

void q2(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "You need to give main a minimum of 1 parameter!!!\n");
	}

	int *params = (int*) malloc((argc - 1) * sizeof(int)); // Allocating array of numbers

	for (int i = 1; i < argc; i++) {
		sscanf(argv[i], "%d", &params[i - 1]); // Convert from string to int and save in the array
	}

	for (int i = 0; i < argc - 1; i++) { // Sending each of the parameters to the function
		do123q2(params[i]);
//printf("----------------------------\n");
	}

	//free(params);
	//return 0;

}

void do123q2(int n1) {
//VARIABLES
	int number = n1;
	int pid;
	int k = 0;

//CREATE RING BUFFER
	ringBuff *c_buff = (ringBuff*) calloc(1, sizeof(ringBuff));
	if (!c_buff) {
		return;
	}
	c_buff->maxlen = SIZE_OF_BUFFS;
	c_buff->head = 0;
	c_buff->tail = 0;
	c_buff->nums = (int*) calloc(c_buff->maxlen, sizeof(int));
	if (!c_buff->nums) {
		return;
	}

//CREATE MMAP
	int *mymap = mmap(NULL, SIZE_OF_BUFFS * sizeof(int), PROT_READ | PROT_WRITE,
	MAP_SHARED | MAP_ANONYMOUS, 0, 0);

//CHECK mmap success
	if (mymap == MAP_FAILED) {  //check Success
		printf("Mapping Failed\n");
	}

//INIT MMAP
	mymap[0] = number;
	mymap[1] = k;
	mymap[2] = n1;

//forking creation and check
	pid = fork();
	if (pid < 0) {
		printf(" Unsuccessful Did Not Create Child Process \n");
		exit(-1);
	}

//Parent proc
	if (pid > 0) {

		while (mymap[0] != 1) {
			if (mymap[0] % 2 == 0) {
				circ_bbuf_push(c_buff, mymap[0]);
			} else if (mymap[0] != 1) {
				mymap[0] = (mymap[0] * 3) + 1;
				mymap[1] += 1;
				printf("in parent:n= %d  k = %d\n", mymap[0], mymap[1]);
				//sleep(1);
			}
		}
		if (mymap[0] == 1) {
			//printf("----------FINISH----------\n");
			printf("n1=%d, n=%d, k=%d\n", mymap[2], mymap[0], mymap[1]);
			exit(EXIT_SUCCESS);
		}

//Child proc
	} else if (pid == 0) {
		while (mymap[0] != 1) {
			if (mymap[0] % 2 == 0) {
				mymap[0] = mymap[0] / 2;
				mymap[1] += 1;
			} else {
				if (circ_bbuf_push(c_buff, mymap[0])) {
					if (circ_bbuf_push(c_buff, mymap[1])) {
					}
				}
			}
		}
	}

}
