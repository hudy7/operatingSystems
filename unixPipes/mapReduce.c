/*
Assignment 2 - EECS 338
Nicholas Hudeck
*/


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define ALPHA_OFFSET 97 // ascii code for "a"
#define LETTERS 26 // letters in the alphabet
#define NUMBER_OF_MAPPERS 4
#define NUMBER_OF_REDUCERS 26

const int PIPE_BUFFER_SIZE = 32;    // arbitrary size for pipe buffers
const int PIPE_READ_END = 0;        // read end of pipe
const int PIPE_WRITE_END = 1;       // write end of pipe

int main(void){

  int mapper_pipes[NUMBER_OF_MAPPERS][2];
  int reducer_pipes[NUMBER_OF_REDUCERS][2];

  int i = 0;
  for(i; i< NUMBER_OF_MAPPERS; i++){
    if(pipe(mapper_pipes[i]) == -1){
      perror("Pipe!");
      exit(EXIT_FAILURE);
    }
    else{
      printf("Piped %d\n ", i);
    }
  }

  printf("Piped mappers \n\n");

  int j = 0;
  for(j; j < NUMBER_OF_REDUCERS; j++){
    if(pipe(reducer_pipes[i]) == -1){
      perror("Reducer pipe!");
      exit(EXIT_FAILURE);
    }
    else{
      printf("Piped %d \n",j);
    }
  }

  printf("Piped reducers \n\n");
  
  printf("Now we have these pipes to mappers:  \n\n");
  int k = 0;
  for(k; k < NUMBER_OF_MAPPERS; k++){
    printf("Mappers: %d\n",&mapper_pipes[k]);
  }




  //map forks

  int map = 0;
  int pid;
  for(map; map < 4; map++){
    pid = fork();
    if(pid < 0){
      printf("Fork error");
      exit(EXIT_FAILURE);
    }

    else if(pid == 0){
      printf("\nChild %d : gonna do some work\n", getpid());
      char buf[BUFFER_SIZE];   
      read(mapper_pipes[map][0],buf,BUFFER_SIZE);  //use read end     
      //check length
      //int len = strlen(buf);
      printf("Just got some information from mapper pipe %d",map);
      exit(0);
    }
    //insert check of fork
  }


  //reduce forks

  int red = 0;
  int redPid;
  for(red; red < 26; red++){
    redPid = fork();
    if(redPid < 0){
      printf("Fork error");
      exit(EXIT_FAILURE);
    }

    else if(redPid == 0){
      printf("\nReduced Child %d : gonna do some work\n", getpid());
      exit(0);
    }
  }  


  
  FILE *input_file = fopen("input.txt","r");
  int rlen = 0;  // read length
  char buffer[BUFFER_SIZE];
  /* Parent needs to read file and send
     it through pipe to mapper */
  while(fgets(buffer, BUFFER_SIZE, input_file) > 0){
    int loopVar = 1;
    for(loopVar; loopVar < 5; loopVar++){
      printf("line %d: %s \n\n",loopVar,buffer);
      write(mapper_pipes[loopVar-1],buffer,strlen(buffer));
      printf("wrote to %d\n",loopVar);
     }
  }


}












 
