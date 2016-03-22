/* bring in all the stuff defined elsewhere */
#include "methane.h"

/* used for random number generator */
time_t t;
/* if necessary */

/* remember array :

  0    |   1   |   2   |
  MUTEX|  SC   |  SH   |

*/


int main(int argc, char **argv){
  int semid;  /* semaphore memory id */
  int shmid;  /* shared memory id */
  unsigned short seminit[NUM_SEMS];  /* array used to initialize semaphores */
  struct common *shared;  /* ptr to shared vars amongst the processes */
  union semun semctlarg;  /* union defined in header file, used to initialize semaphores */
  semctlarg.array = seminit;/*set control array */
  srand((unsigned) time(&t));  /* generates random numbers */

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

  /*
  passed into another sys call
  passed in id group
  set all of the semaphores in that group
  */
  semcontrol










}
