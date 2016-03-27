/*
Nicholas Hudeck
EECS 338 - Assignment 5
A Semaphore-based Solution to Sequential CH4 Production Problem

methane.h
*/

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/errno.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define SEM_KEY 100
#define SHM_KEY 100
#define K 1024
#define ATOMS 25
#define SEMAPHORES 3
#define MUTEX 0
#define SC 1
#define SH 2
#define MUTEX_VALUE 1
#define SC_VALUE 0
#define SH_VALUE 0
typedef enum { false, true } bool;

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

/*
  prints the atom being spawned
  using boolean value
*/
void printAtom (bool isCarbon){
  fflush(stdout);
  if(isCarbon == true){
    printf("Carbon spawn\n");
  }
  else{
    printf("Hydrogen spawn\n");
  }
  fflush(stdout);
  return;
}

/*
  print the elements at the barrier
*/
void printBarrier(bool isCarbon, int waiting_C, int waiting_H){
  fflush(stdout);
  if(isCarbon == true){
    printf("Carbon is at the barrier\n");
  }
  else{
    printf("Hydrogen is at the barrier\n");
  }
  fflush(stdout);
  //print the number of elements waiting
  printf("Number of Carbons waiting:  %d\n", waiting_C);
  fflush(stdout);
  printf("Number of Hydrogens waiting:  %d\n", waiting_H);
  fflush(stdout);

  return;
}

void printCH4(){
  fflush(stdout);
  printf("CH4 has crossed barrier! YES METHANE!\n");
  fflush(stdout);
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
  semop(semid, &psembuf, 1);
  return;
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
  semop(semid, &vsembuf, 1);
  return;
}
