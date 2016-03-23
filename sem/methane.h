#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/errno.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define SHMKEY 77
#define SEMKEY 77

#define NUM_SEMS 3
#define MUTEX 0
#define SC 1
#define SH 2

#define NUM_HYDROGEN 4
#define NUM_CARBON 1
#define NUM_MOLECULES 25

/* used for shared memory creation */
#define K 1024

union semun {
  unsigned short *array;
};

/*
  define a struct that will hold
  all shared variables
*/
struct common {
  int waiting_C;
  int waiting_H;
};

void semWait(int semid, int semaphore){
  struct sembuf psembuf;

  psembuf.sem_op = -1;
  psembuf.sem_flg = 0;
  psembuf.sem_num = semaphore;

  /*
    executes the operation that is being
    built just above
  */
  semop(semid, &psembuf, 1);
};

void semSignal(int semid, int semaphore){
  struct sembuf vsembuf;

  vsembuf.sem_op = 1;
  vsembuf.sem_flg = 0;
  vsembuf.sem_num = semaphore;

  /*
    executes the operation that is being
    built just above
  */
  semop(semid, &vsembuf, 1);
};
