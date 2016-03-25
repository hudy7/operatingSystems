/* bring in all the stuff defined elsewhere */
#include "methane.h"

/* used for random number generator */
time_t t;
/* if necessary */

/* remember array :

  0    |   1   |   2   |
  MUTEX|  SC   |  SH   |

*/

void printVariables(void);
void hydrogen(void);
void carbon(void);


int main(int argc, char **argv){
  int semid;  /* semaphore memory id */
  int shmid;  /* shared memory id */
  unsigned short seminit[NUM_SEMS];  /* array used to initialize semaphores */
  struct common *shared;  /* ptr to shared vars amongst the processes */
  union semun semctlarg;  /* union defined in header file, used to initialize semaphores */
  semctlarg.array = seminit;/*set control array */
  srand((unsigned) time(&t));  /* generates random numbers */

  printf("Made it through initialization \n");

  /*
  semget call, pass in a semaphore key,
  want the system to generate this number of semaphores
  returns id that corresponds to the group of semaphores
  this is the only way to access them
  MAIN POINT: get semaphore memory id
  */
  if ((semid = semget(SEMKEY, NUM_SEMS, 0777)) < 0) {
		perror("semget");
		exit(EXIT_FAILURE);
	}

  printf("Made it through semid = semget line 34\n");

  /*
  initialize the element counts of the array accordingly
  */
  seminit[MUTEX] = 1;
  seminit[SC] = 0;
  seminit[SH] = 0;

  /* set control array */
  semctlarg.array = seminit;

  /* apply initialization */
	if ((semctl(semid, NUM_SEMS, SETALL, semctlarg)) < 0) {
		perror("semctl");
		exit(EXIT_FAILURE);
	}

  /* get shared memory id */
	if ((shmid = shmget(SHMKEY, 1*K, 0777)) < 0) {
		perror("shmget");
		exit(EXIT_FAILURE);
	}

  /* retrieve pointer to shared data structure */
  if ((shared = (struct common *)shmat(shmid, 0, 0)) < 0) {
		perror("shmat");
		exit(EXIT_FAILURE);
	}

  /* initialize shared data structure variables */
  shared->waiting_C = 0;
  shared->waiting_H = 0;

  /**** ^^^ may need more variables! ^^^ ****/

  /* This is where the processes begin to be forked! */


  int retVal = 0;

  for(int i = 0; i < NUM_HYDROGEN; i++){
    if((retVal = fork()) == 0){
      semWait(semid,MUTEX);
      printf("\nHYDROGEN: HERE FOR THE %d ABOUT TO MAKE HYDROGEN CALL\n", i);
      printf("\nHYDROGEN: Just waited MUTEX\n");
      hydrogen();
      printf("HYDROGEN: FORKED ALREADY\n");
    }
  }
  /*
  int carbonRetVal = 0;
  for(int i = 0; i < NUM_CARBON; i++){
    if((carbonRetVal = fork()) == 0){
      semWait(semid,MUTEX);
      printf("\nCARBON: HERE FOR THE %d ABOUT TO MAKE CARBON CALL\n", i);
      printf("\n CARBON: Just waited MUTEX\n");
      carbon();
      printf("CARBON: FORKED ALREADY\n");
    }
  }
*/

//  printVariables();

  /* RANDOM OPTIONAL

  for(int i = 0; i < NUM_MOLECULES; ++i){
    if(rand() % 4 == 0){
      if((retVal = fork())==0){
        hydrogen();
      }
    }
    else {
      if ((retVal = fork()) == 0){
        carbon();
      }
    }
  }

  //waits for that to finish
  for(int i = 0; i < NUM_MOLECULES; ++i){
    wait(0);
  }
  */

  /* PRESET SPAWNING OF MOLECULES

  //spawn 6 hydrogen
		for (int i = 0; i < 4; ++i) {
			if ((retVal = fork()) == 0) {
        printf("%d forked HYDROGENS \n", i);
        hydrogen();
			} else if (retVal < 0) {
				perror("fork");
				exit(EXIT_FAILURE);
			}//else still main process.. continue loop
		}

		//spawn 6 west cars
		for (int i = 0; i < 6; ++i) {
			if ((retVal = fork()) == 0) {
        printf("%d forked west cars\n",i);
        //west();
			} else if (retVal < 0) {
				perror("fork");
				exit(EXIT_FAILURE);
			}//else still main process.. continue loop
		}

		//spawn 1 more east car
		if ((retVal = fork()) == 0) {
      printf("Forked a single east car\n");
      //east();
		} else if (retVal < 0) {
			perror("fork");
			exit(EXIT_FAILURE);
		}//else still in main process

		//wait for all car processes to finish
		for (int i = 0; i < 13; ++i) {
			if (wait(0) < 0) {
				perror("wait");
				exit(EXIT_FAILURE);
			}
		}

  //delete semaphores
	if (semctl(semid, NUM_SEMS, IPC_RMID, 0) < 0) {
		perror("semctl");
		exit(EXIT_FAILURE);
	}

  //delete shared memory
	if (shmctl(shmid, IPC_RMID, 0) < 0) {
		perror("shmctl");
		exit(EXIT_FAILURE);
	}

  */

  return EXIT_SUCCESS;

}

/*
void printVariables(void){
  printf("waiting_H: %d,  waiting_C: %d", shared->waiting_H, shared->waiting_C);
}
*/




void carbon(void){
  printf("IN THE CARBON METHOD\n");
  struct common *shared;

  int semid, shmid;//semaphore memory id, shared memory id
  int pid = getpid();

  //get semaphore memory id
	if ((semid = semget(SEMKEY, NUM_SEMS, 0777)) < 0) {
		perror("CARBON: semget");
		exit(EXIT_FAILURE);
	}

	//get shared memory id
  if ((shmid = shmget(SHMKEY, 1*K, 0777)) < 0) {
  	perror("CARBON: shmget");
  	exit(EXIT_FAILURE);
  }

  //get pointer to shared data structure
  if ((shared = (struct common *)shmat(shmid, 0, 0)) < 0) {
  	perror("CARBON: shmat");
  	exit(EXIT_FAILURE);
  }

  //semWait(semid, MUTEX);

  printf("waiting_H: %d,  waiting_C: %d \n", shared->waiting_H, shared->waiting_C);
  printf("CARBON: signaling mutex\n");
  shared->waiting_C += 1;
  semSignal(semid,MUTEX);
  semWait(semid,SH);
  printf("CARBON: waiting for SH");
}


void hydrogen(void){
  printf("HYDROGEN: IN THE HYDROGEN METHOD\n");
  //printVariables();

  //printf("Printing in hydrogen: waiting_H: %d,  waiting_C: %d", shared->waiting_H, shared->waiting_C);

  struct common *shared;

  int semid, shmid;//semaphore memory id, shared memory id
  int pid = getpid();

  //get semaphore memory id
	if ((semid = semget(SEMKEY, NUM_SEMS, 0777)) < 0) {
		perror("HYDROGEN: semget");
		exit(EXIT_FAILURE);
	}

	//get shared memory id
  if ((shmid = shmget(SHMKEY, 1*K, 0777)) < 0) {
  	perror("HYDROGEN: shmget");
  	exit(EXIT_FAILURE);
  }

  //get pointer to shared data structure
  if ((shared = (struct common *)shmat(shmid, 0, 0)) < 0) {
  	perror("HYDROGEN: shmat");
  	exit(EXIT_FAILURE);
  }

  //semWait(semid, MUTEX);

  printf("HYDROGEN: waiting_H: %d,  waiting_C: %d \n", shared->waiting_H, shared->waiting_C);

  if(shared->waiting_H >= 3 && shared->waiting_C >= 1){
      for(int i = 0; i < 3; i++){ // release 3 HYDROGEN
        semSignal(semid,SH);
        shared->waiting_H -= 3;
        semSignal(semid,SC);
        shared->waiting_C -= 1;

        semSignal(semid, MUTEX); //release MUTEXT
      }
  }
  else{
    shared->waiting_H += 1;
    printf("HYDROGEN: signaling MUTEX\n");
    fflush(stdout);
    printf("HYDROGEN: %d HAS ARRIVED\n", pid);
    fflush(stdout);

    printf("HYDROGEN: waiting_H: %d,  waiting_C: %d \n", shared->waiting_H, shared->waiting_C);

    semSignal(semid,MUTEX);

    printf("HYDROGEN : released lock on MUTEX\n");
    semWait(semid,SC);
  }





}
