/*
Nicholas Hudeck
EECS 338 - Assignment 5
A Semaphore-based Solution to Sequential CH4 Production Problem

carbon.c
*/
#include "methane.h"

struct common *shared;

int main(int argc, char *argv[]){
  int semid = atoi(argv[1]);
  int shmid = atoi(argv[2]);
	int pid = getpid();

	if ((shared = (struct common *)shmat(shmid, 0, 0)) < 0) {
		perror("CARBON: shmat");
		exit(EXIT_FAILURE);
	}

  printAtom(true);
	semWait(semid, MUTEX);

	fflush(stdout);
	printf("Carbon atom %d created\n", my_pid);
	fflush(stdout);

	printBarrier(true, shared->waiting_C, shared->waiting_H);

	if (shared->waiting_H >= 4) {
		for(int i = 0; i < 4; i++){
      	semSignal(semid, SH);
    }
		shared->waiting_H -= 4;
		semSignal(semid, MUTEX);
		printCH4();
	} else {
		shared->waiting_C += 1;
		semSignal(semid, MUTEX);
		semWait(semid, SC);
	}

	return EXIT_SUCCESS;
}
