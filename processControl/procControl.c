/* Process Control Assignment 1
   Nicholas Hudeck - nicholas.hudeck@case.edu
*/

#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <time.h> 

/* Main program : P0 
   Print process id : getpid()
   Print parent process id : getppid()
   Print host name : gethostname() 
   Print username : cuserid() 
   Print time of day: ctime() 
   Print working directory : getcwd() 
      also Print some text indicating that its the parent call 

   P0 creates environment var WHALE = 7 putenv() 
   P0 forks two child processes C1 and C2 
   Then C1 and C2 print their pid : getpid() and parents : getppid() 
*/

void printPid(){
  char hostname[128];
  gethostname(hostname, sizeof hostname);

  printf("Process id:    %d  \n", (int)getpid());
  printf("Parent process id:    %d  \n", (int)getppid());

  printf("Host name:    %s  \n", hostname);
  printf("User name:    %s  \n", cuserid(NULL));
  printf("Time of day:  %d  \n", ctime(NULL));
  return 0;
}

int main(){
  printPid();
}










