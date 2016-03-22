#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/smh.h>
#include <sys/errno.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define NUM_SEMS 3
#define SEM 1
#define SC 0
#define SH 0

#define SHMKEY 717
#define SEMKEY 717

#define NUM_HYDROGEN 4
#define NUM_CARBON 1


union semun {
  unsigned short *array;
}

/*
  define a struct that will hold
  all shared variables
*/
struct common {
  int waiting_C = 0;
  int waiting_H = 0;
}

void semWait(int semid, int semaphore){
  struct sembuf psembuf;

  psembuf.sem_op = -1;
  psembuf.sem_flg = 0;
  psembuf.sem_num = semaphore;

  /*
    executes the operation that is being
    built just above
  */
  semop(semit, &psembuf, 1);
}

void semSignal(int semid, int semaphore){
  struct sembuf vsembuf;

  vsembuf.sem_op = 1;
  vsembuf.sem_flg = 0;
  vsembuf.sem_num = semaphore;

  /*
    executes the operation that is being
    built just above
  */
  semop(semit, &psembuf, 1);
}
