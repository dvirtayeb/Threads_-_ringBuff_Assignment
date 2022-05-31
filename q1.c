#include <stdio.h>
#include <sys/types.h> /* pid_t */
#include <unistd.h>  /* _exit, fork */
#include <stdlib.h>  /* exit */
#include <errno.h>   /* errno */
#include <sys/wait.h>
#include <sys/mman.h>
#include <stdint.h>
#include <fcntl.h>
#include "q1.h"

void q1(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "You need to give main a minimum of 1 parameter!!!\n");
		return;
	}
	int *params = (int*) malloc((argc - 1) * sizeof(int)); // Allocating array of numbers
	for (int i = 1; i < argc; i++) {
		sscanf(argv[i], "%d", &params[i - 1]); // Convert from string to int and save in the array
	}
	for (int i = 0; i < argc - 1; i++) {
		do123(params[i]);
	}
	free(params);
}

void do123(int n1) {
	int number = n1;
	int pid;
	int k = 0;

//pipe file descriptor pipe[0]-> read pipe[1]->write
	int pipefdfromchildtoparent[2];
	if (pipe(pipefdfromchildtoparent) == -1) { //check success of pipe()
		perror("pipe\n");
		exit(EXIT_FAILURE);
	}

	int pipefdfromparenttochild[2];
	if (pipe(pipefdfromparenttochild) == -1) { //check success of pipe()
		perror("pipe\n");
		exit(EXIT_FAILURE);
	}
//forking creation and check
	pid = fork();
	if (pid < 0) {
		printf(" Unsuccessful Did Not Create Child Process \n");
		exit(-1);
	}
	int args[3] = { 0 };
	args[0] = number;
	args[1] = k;
	args[2] = n1;
//
	if (pid > 0) { //parent proc
		int i = 0;
		while (args[0] != 1) {
			i++;
			if (args[0] % 2 == 0) {
				close(pipefdfromparenttochild[0]);
				close(pipefdfromchildtoparent[1]);
				write(pipefdfromparenttochild[1], &args[0], sizeof(int));
				write(pipefdfromparenttochild[1], &args[1], sizeof(int));
				while (args[0] % 2 == 0) {
					read(pipefdfromchildtoparent[0], &args[0], sizeof(int));
					read(pipefdfromchildtoparent[0], &args[1], sizeof(int));
				}
			} else {
				args[0] = (args[0] * 3) + 1;
				args[1] += 1;
				sleep(1);
			}
		}
		close(pipefdfromchildtoparent[0]);
		close(pipefdfromparenttochild[1]);
		if (args[0] == 1) {
			write(pipefdfromparenttochild[1], &args[0], sizeof(int));
			printf("The OG Number is %d K is %d n is %d\n", args[2], args[1],
					args[0]);
			exit(EXIT_SUCCESS);
		}
		wait(0);
	} else if (pid == 0) { //child proc
		while (args[0] != 1) {
			if (args[0] % 2 == 0) {
				args[0] = args[0] / 2;
				args[1] += 1;
				write(pipefdfromchildtoparent[1], &args[0], sizeof(int));
				write(pipefdfromchildtoparent[1], &args[1], sizeof(int));
				if (args[0] == 1) {
					close(pipefdfromparenttochild[0]);
					close(pipefdfromchildtoparent[1]);
					break;
				}
			} else {
				if (args[0] == 1) {
					break;
				}
				close(pipefdfromchildtoparent[0]);
				close(pipefdfromparenttochild[1]);
				write(pipefdfromchildtoparent[1], &args[0], sizeof(int));
				write(pipefdfromchildtoparent[1], &args[1], sizeof(int));
			}
			while (args[0] % 2 != 0) {
				if (args[0] == 1) {
					break;
				}
				read(pipefdfromparenttochild[0], &args[0], sizeof(int));
				read(pipefdfromparenttochild[0], &args[1], sizeof(int));
			}

		}
//		close(pipefdfromparenttochild[0]);
//		close(pipefdfromchildtoparent[1]);

	}

}

