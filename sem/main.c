/* bring in all the stuff defined elsewhere */
#include "methane.h"

/* used for random number generator
time_t t;
if necessary */

int main(int argc, char **argv){

  int semid;

  /* shared memory id */
  int shmid;

  /* array used to initialize semaphores */
  unsigned short seminit[NUM_SEMS];

  /* ptr to shared vars amongst the processes */
  struct common *shared;

  /* union defined in header file */
  union semun semcontrol;









}
