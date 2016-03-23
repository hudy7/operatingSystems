#include "methane.h"

struct common *shared; //ptr to shared data

void printVariables(void);

int main(int argc, char const *argv[]){

  int semid;
  int shmid;

  int pid = getpid();

  //get semaphore memory id
	if ((semid = semget(SEMKEY, NUM_SEMS, 0777)) < 0) {
		perror("semget");
		exit(EXIT_FAILURE);
	}

	//get shared memory id
	if ((shmid = shmget(SHMKEY, 1*K, 0777)) < 0) {
		perror("shmget");
		exit(EXIT_FAILURE);
	}

	//get pointer to shared data structure
	if ((shared = (struct common *)shmat(shmid, 0, 0)) < 0) {
		perror("shmat");
		exit(EXIT_FAILURE);
	}

  semWait(semid, MUTEX);

  fflush(stdout);
  printf("HYDROGEN %d HAS ARRIVED", pid);
  fflush(stdout);








}
