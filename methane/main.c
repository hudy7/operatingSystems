/*
Nicholas Hudeck
EECS 338 - Assignment 5
A Semaphore-based Solution to Sequential CH4 Production Problem

main.c
*/
#include "methane.h"

time_t t; //used for random

// used to execute carbon atom process
void carbon(int semid, int shmid) {
	// convert the semid and shmid into chars
  char semidbuf[1];
  char shmidbuf[1];
  sprintf(semidbuf, "%d", semid);
  sprintf(shmidbuf, "%d", shmid);

  // execute carbon with semid and shmid
  execl("carbon", "carbon", semidbuf, shmidbuf, NULL);
  perror("CARBON: execl");
  exit(EXIT_FAILURE);
}

// used to execute hydrogen atom process
void hydrogen(int semid, int shmid) {
	// convert the semid and shmid into characters
	char semidbuf[1];
  char shmidbuf[1];
  sprintf(semidbuf, "%d", semid);
  sprintf(shmidbuf, "%d", shmid);

  // execute hydrogen with semid and shmid
	execl("hydrogen", "hydrogen", semidbuf, shmidbuf, NULL);
	perror("HYDROGEN: execl");
	exit(EXIT_FAILURE);
}

/* remember array :
  0    |   1   |   2   |
  MUTEX|  SC   |  SH   |
*/

int main(int argc, char *argv[]) {

  int semid;  /* semaphore memory id */
  int shmid;  /* shared memory id */
  unsigned short seminit[SEMAPHORES];  /* array used to initialize semaphores */
  struct common *shared;  /* ptr to shared vars amongst the processes */
  union semun semctlarg;  /* union defined in header file, used to initialize semaphores */
  srand((unsigned) time(&t));  /* generates random numbers */

	if ((semid = semget(IPC_PRIVATE, SEMAPHORES, 0777)) < 0) {
		perror("MAIN: semget");
		exit(EXIT_FAILURE);
	}

  /* initialize semaphore values */
	seminit[MUTEX] = MUTEX_VALUE;
	seminit[SH] = SH_VALUE;
	seminit[SC] = SC_VALUE;
	semctlarg.array = seminit;

  /* apply initialization */
	if ((semctl(semid, SEMAPHORES, SETALL, semctlarg)) < 0) {
		perror("MAIN: semctl");
		exit(EXIT_FAILURE);
	}

	/* get shared memory id */
	if ((shmid = shmget(IPC_PRIVATE, 1*K, 0777)) < 0) {
		perror("MAIN: shmget");
		exit(EXIT_FAILURE);
	}

  /* retrieve pointer to shared data structure */
	if ((shared = (struct common *)shmat(shmid, 0, 0)) < 0) {
		perror("shmat_main");
		exit(EXIT_FAILURE);
	}

  /* initialize shared data structure variables */
	shared->waiting_C = 0;
	shared->waiting_H = 0;

  /* initialize count number of CARBONs and HYDROGENs atoms randomly spawned */
	int carbon_count = 0;
	int hydrogen_count = 0;

	/* do random execution */
	int i;
	int retVal = 0;
	for (i = 0; i < ATOMS; i++) {
		/* spawn carbon atom when rand is an odd number */
		int random = rand();
		if (random % 2 != 0) {
			if ((retVal = fork()) == 0){
				carbon(semid, shmid);
			} else if (retVal < 0) {
				perror("fork");
				exit(EXIT_FAILURE);
			} else {
				carbon_count++;
			}
		} else { /* spawn hydrogen atom when random is an even number */
			if ((retVal = fork()) == 0)
				hydrogen(semid, shmid);
			else if (retVal < 0){
				perror("fork");
				exit(EXIT_FAILURE);
			} else {
				hydrogen_count++;
			}
		}
	}

  /*
    arithmetically hacky way of determining which atoms have been spawned and
    which are left over from the previous loop.

    then adjust accordingly for mismatches in the spawning process.
  */
	int carbs = hydrogen_count / 4;
	int leftoverHydrogen = hydrogen_count % 4;
	int carbonsLeft = carbon_count - carbs;
	int hydrogenLeft = (carbonsLeft * 4) - leftoverHydrogen;

	for (i = 0; i < hydrogenLeft; i++) {
		fflush(stdout);
		/* spawn extra hydrogen atom */
		if ((retVal = fork()) == 0){
			hydrogen(semid, shmid);
		} else if (retVal < 0) {
			perror("fork");
			exit(EXIT_FAILURE);
		}
	}

	int totalAtoms = ATOMS + hydrogenLeft;

  /* wait for atoms to be deleted */
  for (i = 0; i < totalAtoms; i++) {
  	fflush(stdout);
  	printf("Waiting for process %d to exit\n", i);
  	fflush(stdout);
  	if (wait(0) < 0) {
  		perror("wait");
  		exit(EXIT_FAILURE);
  	}
  }

  fflush(stdout);
  printf("CH4 complete. Cleaning up.\n");
  fflush(stdout);

  /* delete shared memory */
  if (shmctl(shmid, IPC_RMID, 0) < 0) {
  	perror("MAIN: shmctl");
  	exit(EXIT_FAILURE);
  }

  /* delete semaphores */
  if (semctl(semid, SEMAPHORES, IPC_RMID, 0) < 0) {
  	perror("MAIN: semctl");
  	exit(EXIT_FAILURE);
  }

  fflush(stdout);
  printf("Exiting!\n");
  fflush(stdout);

  return EXIT_SUCCESS;
}
