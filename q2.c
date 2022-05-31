///*
// * q2.c
// *
// *  Created on: 11 Apr 2021
// *      Author: noaaviel
// */
//
//#include <stdio.h>
//#include <sys/types.h> /* pid_t */
//#include <unistd.h>  /* _exit, fork */
//#include <stdlib.h>  /* exit */
//#include <errno.h>   /* errno */
//#include <sys/wait.h>
//#include <sys/mman.h>
//
//void do123q2(int n1);
//void writeToBuff(int *mymap, int *in, int *args);
//void readFromBuff(int *mymap, int *out, int *args);
//
////V
//int main(int argc, char **argv) {
//	if (argc < 2) {
//		fprintf(stderr, "You should give at least one parameter!!!\n");
//		return -1;
//	}
//
//	int *params = (int*) malloc((argc - 1) * sizeof(int)); // Allocating array of numbers
//
//	for (int i = 1; i < argc; i++) {
//		sscanf(argv[i], "%d", &params[i - 1]); // Convert from string to int and save in the array
//	}
//
//	for (int i = 0; i < argc - 1; i++) // Sending each of the parameters to the function
//			{
//		do123q2(params[i]);
////printf("----------------------------\n");
//	}
//
//	free(params);
//	return 0;
//
//}
////V
//void writeToBuff(int *mymap, int *in, int *args) {
//	mymap[*in] = args[0];
//	*in = *in + 1;
//	mymap[*in] = args[1];
//	*in = *in + 1;
//	mymap[*in] = args[2];
//}
////V
//void readFromBuff(int *mymap, int *out, int *args) {
//	args[0] = mymap[*out];
//	*out = *out + 1;
//	args[1] = mymap[*out];
//	*out = *out + 1;
//	args[2] = mymap[*out];
//}
//
////XXXXXXXXXXXXXXXXXXXXXXXXXX
//void do123q2(int n1) {
//
//	//V
//	int number = n1;
//	int pid;
//	int k = 0;
//	int size = 3;
////each pipe -> cyclicBuff V
//	int inPar = 4;
//	int outPar = 7;
//	int inCh = 7;
//	int outCh = 4;
//
//	//V
//// *2 for two buffers +4 for in->0 out->1 flags 2,3,4->buff1,5,6,7->buff2
//	int *mymap = mmap( NULL, 4 + (2 * sizeof(int) * size),
//	PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
//
////CHECK mmap success
//	if (mymap == MAP_FAILED) {  //check Success
//		printf("Mapping Failed\n");
//	}
//	mymap[0] = outPar;
//	mymap[1] = inPar;
//	mymap[2] = outCh;
//	mymap[3] = inCh;
//
////forking creation and check
//	pid = fork();
//	if (pid < 0) {
//		printf(" Unsuccessful Did Not Create Child Process \n");
//		exit(-1);
//	}
//	int args[3] = { 0 };
//	args[0] = number;
//	args[1] = k;
//	args[2] = n1;
/////////////////////////////////////->V
//	if (pid > 0) { //parent proc
//		//	waitpid(0,NULL, 0 /* options */ );
//		//(0);
//		sleep(10);
//		int i = 0;
//		///reading writing to mmap
//		while (args[0] != 1) {
//			i++;
//			if (args[0] % 2 == 0) {
//				while (args[0] % 2 == 0) {
//					//making buffer cyclic
//					//in parent write to index 2 3 4 in map
//					while (inPar >= 4 && inPar <= 6) {
//						if (inPar == 4) {
//							writeToBuff(mymap, &inPar, args);
//							break;
//						} else if (inPar == 6) {
//							inPar -= 2;
//							writeToBuff(mymap, &inPar, args);
//							break;
//						}
//					}
//					break;
//				}
//				while (args[0] % 2 == 0) {
//					//read until child gives back odd number
//					while (outPar >= 7 && outPar <= 9) {
//						if (outPar == 7) {
//							readFromBuff(mymap, &outPar, args);
//							break;
//						} else if (outPar == 9) {
//							outPar -= 2;
//							readFromBuff(mymap, &outPar, args);
//							break;
//						}
//					}
//				}
//
//			} else {
//				//V
//				printf(
//						"ELSE from parent The OG Number is %d K is %d n is %d %d\n",
//						args[2], args[1], args[0], i);
//				args[0] = (args[0] * 3) + 1;
//				args[1] += 1;
//				sleep(1);
//			}
//		}
//		//V
//		wait(NULL);
//		if (args[0] == 1) {
//			printf("The OG Number is %d K is %d n is %d\n", args[2], args[1],
//					args[0]);
//		}
//	} else if (pid == 0) { //child proc
//		while (args[0] != 1) {
//			if (args[0] % 2 == 0) {
//				args[0] = args[0] / 2;
//				args[1] += 1;
//				//write
//				while (args[0] % 2 == 0) {
//					while (inCh >= 7 && inCh <= 9) {
//						if (inCh == 9) {
//							writeToBuff(mymap, &inCh, args);
//							break;
//						} else if (inCh == 9) {
//							inCh -= 2;
//							writeToBuff(mymap, &inCh, args);
//							break;
//						}
//					}
//				}
//			} else {
//				if (args[0] % 2 == 0) {
//					while (args[0] % 2 == 0) {
//						while (inCh >= 7 && inCh <= 9) {
//							if (inCh == 9) {
//								writeToBuff(mymap, &inCh, args);
//								break;
//							} else if (inCh == 9) {
//								inCh -= 2;
//								writeToBuff(mymap, &inCh, args);
//								break;
//							}
//						}
//					}
//					while (args[0] % 2 == 0) {
//						while (outCh <= 6 && outCh >= 4) {
//							if (outCh == 4) {
//								readFromBuff(mymap, &outCh, args);
//								break;
//							} else if (outCh == 6) {
//								outCh -= 2;
//								readFromBuff(mymap, &outCh, args);
//								break;
//							}
//						}
//
//					}
//				}
//			}
//
//		}
//
//	}
//}
//
