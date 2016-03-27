/*
Nicholas Hudeck
EECS 338 - Assignment 5
A Semaphore-based Solution to Sequential CH4 Production Problem

hydrogen.c
*/
#include "methane.h"

struct common *shared;

int main(int argc, char *argv[]){
	int semid, shmid;
  semid = atoi(argv[1]);
  shmid = atoi(argv[2]);
	int pid = getpid();

	if ((shared = (struct common *)shmat(shmid, 0, 0)) < 0) {
		perror("HYDROGEN: shmat");
		exit(EXIT_FAILURE);
	}

	printAtom(false);
	semWait(semid, MUTEX);

	fflush(stdout);
  printf("Hydrogen atom %d created\n", pid);
	fflush(stdout);

	printBarrier(false, shared->waiting_C, shared->waiting_H);

  /* algorithm from class */
	if (shared->waiting_H >= 3 && shared->waiting_C >= 1) {
		for(int i = 0; i < 3; i++){
			semSignal(semid, SH);
    }
		shared->waiting_H -= 3;
		semSignal(semid, SC);
		shared->waiting_C -= 1;
		semSignal(semid, MUTEX);
		printCH4();
	} else {
		shared->waiting_H += 1;
		semSignal(semid, MUTEX);
		semWait(semid, SH);
	}

	return EXIT_SUCCESS;
}
