#include "q1.h"
#include "new_q2.h"
#include "q3.h"
#include "q4.h"
#include "q5.h"
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv) {
	printf("-------------------q1--------------------\n");
	q1(argc, argv);
	sleep(2);
	printf("-------------------q2--------------------\n");
	q2(argc, argv);
	sleep(2);
	printf("-------------------q4--------------------\n");
	q4(argc, argv);
	sleep(2);
	printf("-------------------q3--------------------\n");
	q3(argc, argv);
	sleep(2);
	printf("-------------------q5b--------------------\n");
	q5b(argc, argv);

}
