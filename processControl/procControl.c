/* Process Control Assignment 1
   Nicholas Hudeck - nicholas.hudeck@case.edu
*/

#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <errno.h> //fdfda
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
   Then C1 and C2 print their pid : getpid() and parents : getppid
 }
*/


/*Method for printing information about process*/
void printPid(){
 
  time_t currentTime;
  time(&currentTime);
  char cwd[1024];
  char hostname[128];

  if(gethostname(hostname, sizeof hostname)!=0){
	perror("Hostname error");     
  }
  else{
     	printf("Host name:     %s  \n", hostname);
  };

  if (currentTime == ((time_t) -1)) {
        perror("time_wrapper");
  }
  else{
	printf("Time of day:   %s  ", ctime(&currentTime));
  }; 

  //no wrapper for cuser id, read that get login is better 
  //using cuserid for purpose of assignment requirements 
  printf("User name:     %s  \n", cuserid(NULL));
  
  if(getcwd(cwd, sizeof(cwd))==NULL){
     perror("getcwd");
  }
  else{
    printf("Working directory: %s  \n", getcwd(cwd, sizeof(cwd)) );
  }

  return;
}


int main(){

  pid_t pid, mypid = getpid();
  time_t begin, end;
  int status;

  char *var = "WHALE=7";
  int n = putenv(var);
  char *tmp = getenv("WHALE");
  printPid();

  begin = time(NULL);

  //Child 1
  if((pid = fork()) == 0){
    begin = time(NULL);
    mypid = getpid();
    
    fflush(stdout);
    printf("C1 Process id:    %d  \n", mypid);
    printf("Parent  id:    %d  \n\n", (int)getppid());
    fflush(stdout);
    sleep(2);
    char* buf = getenv("WHALE");
    //printf(buf);
    int val = atoi(buf);
    

    fflush(stdout);
    printf("C1: %d shrimp (WHALE environment variable value is now %d) \n",val-1,val-1);
    fflush(stdout);
    sleep(4);

    fflush(stdout);
    printf("C1: %d shrimp (WHALE environment variable value is now %d) \n",val-4,val-4);
    fflush(stdout);
    sleep(5);

    char cwd[1024];
    fflush(stdout);
    printf("\nC1: About to change directory from working directory: %s\n",   getcwd(cwd, sizeof(cwd)) );
    fflush(stdout);
    char *directory = "/";
    int ret;
    ret = chdir (directory);
    fflush(stdout);
    printf("\nC1: Directory changed to: %s \n", getcwd(cwd, sizeof(cwd)));
    fflush(stdout);

    fflush(stdout);
    printf("\nC1: About to use exec() call to execute 'ls - la' \n");
    fflush(stdout);

    execl("/bin/ls", "ls", "-la", (char *)NULL);
    
    fflush(stdout);
    printf("\n\n\n");

    exit(0);

  }
  //Child 2
  else if ((pid = fork()) == 0 ){
    begin = time(NULL);
    mypid = getpid();

    fflush(stdout);
    printf("\nC2 Process id:    %d  \n", mypid);
    printf("Parent  id:    %d    \n\n", (int)getppid());
    fflush(stdout);
    sleep(3);

    char* buf = getenv("WHALE");
    //printf(buf);
    int val = atoi(buf);
    
    fflush(stdout);
    printf("C2: %d shrimp (WHALE environment variable value is now %d) \n",val-2,val-2);
    fflush(stdout);
    sleep(4);

    fflush(stdout);
    printf("C2: %d shrimp (WHALE environment variable value is now %d) \n",val-5,val-5);
    fflush(stdout);
    sleep(7);
    
    
    char cwd[1024];
    printf("\n\nC2: Printing current working directory: %s  \n\n", getcwd(cwd, sizeof(cwd)) );


    exit(0);
  }

  //Parent
 else if ((pid = getppid())){
    printf("Parent PID: %d  \n\n", mypid);
    sleep(1);

    char* buf = getenv("WHALE"); 
    int val = atoi(buf);

    fflush(stdout);
    printf("PO: %d shrimp (WHALE environment variable value is now %d) \n",val,val);
    fflush(stdout);
    sleep(4);


    fflush(stdout);
    printf("P0: %d shrimp (WHALE environment variable value is now %d) \n",val-3,val-3);
    fflush(stdout);
    sleep(4);

    fflush(stdout);
    printf("P0: %d shrimp (WHALE environment variable value is now %d) \n",val-6,val-6);
    fflush(stdout);
    sleep(7);


    if((pid=wait(&status))){
      fflush(stdout);
      printf("\nParent(pid %d). Child 1 terminated \n",mypid);
      fflush(stdout);
    }
    else{
      fflush(stdout);
      printf("\nParent(pid %d). Child 1 termination error \n",mypid);
      fflush(stdout);
    }

    if((pid=wait(&status))){
      fflush(stdout);
      printf("\nParent(pid %d): Child 2 terminated \n",mypid);
      fflush(stdout);
    }
    else{
      fflush(stdout);
      printf("\nParent(pid %d): Child 2 termination error \n",mypid);
      fflush(stdout);
    }

    end = time(NULL);

    printf("\nFinal parent call from pid: %d \nWHALE variable value is now %d \n",mypid, val-7);
    exit(0);
  }
}

