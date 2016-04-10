/*
Nicholas Hudeck
EECS 338 - Assignment 6
Sequential CH4 Production Problem
main.c

simplified assignment 6 to one file
*/
 
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/errno.h>

#define ATOMS 25
#define NUM_C 5
#define NUM_H 20

void carbon(void *arg);
void hydrogen(void *arg);


//shared variables
int waiting_H = 0;
int waiting_C = 0;
int num_methane = 0;

//semaphores
sem_t sem_h;
sem_t sem_c;
sem_t mutex;

int main(int argc, char **argv){
	/* initializes a thread array that will store all the hydrogen and carbon threads */
	pthread_t threads[ATOMS];

	/* initializes  and checks semaphores */

	if((sem_init(&sem_h, 0, (unsigned int) 0) < 0){
		perror("sem_init");
		exit(EXIT_FAILURE);
	}

	if((sem_init(&sem_c, 0, (unsigned int) 0) < 0){
		perror("sem_init");
		exit(EXIT_FAILURE);
	}

	if((sem_init(&mutex, 0, (unsigned int) 1) < 0){
		perror("sem_init");
		exit(EXIT_FAILURE);
	}		
	

	int i;
	//creates 5 carbon threads
	for(i = 0; i < NUM_C; i++){
		//carbon threads take up indexes 0-4 in the thread array
		pthread_create(&threads[i], NULL, (void*)&carbon, NULL);	//reference to thread, NULL, function to execute, thread data struct corresponding
	}
	//creates 20 hydrogen threads
	for(i = NUM_C; i < ATOMS; i++){
		//hydrogen threads take up indexes 5-24 in the thread array
		pthread_create(&threads[i], NULL, (void*)&hydrogen, NULL);	//reference to thread, NULL, function to execute, thread data struct corresponding
	}
	//exits each thread
	for(i = 0; i < ATOMS; ++i){
		pthread_join(threads[i], NULL);
	}
	//destroys semaphores
	sem_close(&sem_h);
	sem_close(&sem_c);
	sem_close(&mutex);

	exit(EXIT_SUCCESS);
}


//thread functions

//carbon function
void carbon(void *arg){
	fflush(stdout);
	printf("Carbon is created!\n");
	fflush(stdout);
	sem_wait(&mutex);
	fflush(stdout);
	printf("Pre-Barrier - Carbon at the barrier! The number of other carbons waiting at the barrier are %d, the number of hydrogens waiting at the barrier are %d\n", waiting_C, waiting_H);
	fflush(stdout);
	//if there are 4 or more hydrogens at the barrier, enter the barrier
	if(waiting_H >= 4){
		sem_post(&sem_h);
		sem_post(&sem_h);
		sem_post(&sem_h);
		sem_post(&sem_h);
		waiting_H = waiting_H - 4;
		num_methane = num_methane + 1;
		fflush(stdout);
		printf("Post-Barrier - Molecule formed: the number of methanes is now %d\n", num_methane);
		fflush(stdout);
		sem_post(&mutex);
	}
	//else wait for more hydrogens
	else{
		waiting_C = waiting_C + 1;
		sem_post(&mutex);
		sem_wait(&sem_c);
	}		
}
//hydrogen function
void hydrogen(void *arg){
	fflush(stdout);
	printf("Hydrogen is created!\n");
	fflush(stdout);
	sem_wait(&mutex);
	fflush(stdout);
	printf("Pre-Barrier - Hydrogen at the barrier! The number of carbons waiting at the barrier are %d, the number of other hydrogens waiting at the barrier are %d\n", waiting_C, waiting_H);
	fflush(stdout);
	//if there are 3 or more hydrogens and 1 or more carbon at the barrier, enter the barrier
	if(waiting_H >= 3 && waiting_C >= 1){
		sem_post(&sem_h);
		sem_post(&sem_h);
		sem_post(&sem_h);
		sem_post(&sem_c);
		waiting_H = waiting_H - 3;
		waiting_C = waiting_C - 1;
		num_methane = num_methane + 1;
		fflush(stdout);
		printf("Post-Barrier - Molecule formed: the number of methanes is now %d\n", num_methane);
		fflush(stdout);
		sem_post(&mutex);
	}
	//else wait for more hydrogens and carbons
	else{
		waiting_H = waiting_H + 1;
		sem_post(&mutex);
		sem_wait(&sem_h);
	}		
}

