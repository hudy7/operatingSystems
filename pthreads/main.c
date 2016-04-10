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
	if((sem_init(&sem_h, 0, (unsigned int) 0))!= 0){
		perror("sem_init");
		exit(EXIT_FAILURE);
	}

	if((sem_init(&sem_c, 0, (unsigned int) 0)) != 0){
		perror("sem_init");
		exit(EXIT_FAILURE);
	}

	if((sem_init(&mutex, 0, (unsigned int) 1)) != 0){
		perror("sem_init");
		exit(EXIT_FAILURE);
	}		
	

	int i;
	
	/* creates 5 carbon threads */
	for(i = 0; i < NUM_C; i++){
		if(pthread_create(&threads[i], NULL, (void*)&carbon, NULL) != 0){
			perror("pthread_create");
			exit(EXIT_FAILURE);
		}	
	}

	/* creates 20 hydrogen threads */
	for(i = NUM_C; i < ATOMS; i++){
		if(pthread_create(&threads[i], NULL, (void*)&hydrogen, NULL) != 0){
			perror("pthread_create");
			exit(EXIT_FAILURE);
		}
	}

	/* exiting the threads with error checking */
	for(i = 0; i < ATOMS; ++i){
		if(pthread_join(threads[i], NULL) != 0){
			perror("pthread_join");
			exit(EXIT_FAILURE);
		}
	}

	/* destroy semaphores with error checking */

	if(sem_close(&sem_h) != 0){
		perror("sem_close");
		exit(EXIT_FAILURE);
	}


	if(sem_close(&sem_c) != 0){
		perror("sem_close");
		exit(EXIT_FAILURE);
	}


	if(sem_close(&mutex) != 0){
		perror("sem_close");
		exit(EXIT_FAILURE);
	}

	//program complete
	exit(EXIT_SUCCESS);
}


void carbon(void *arg){
	fflush(stdout);
	printf("Carbon is created!\n");
	fflush(stdout);
	
	if(sem_wait(&mutex) != 0){
		perror("sem_wait");
		exit(EXIT_FAILURE);
	}
	
	fflush(stdout);
	printf("Carbon at the barrier.\n Carbon waiting: %d.\n Hydrogen waiting: %d\n\n", waiting_C, waiting_H);
	fflush(stdout);


	/* if > 4 hyrdogens at barrier, enter */
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
	
	if(sem_wait(&mutex) != 0){
		perror("sem_wait");
		exit(EXIT_FAILURE);
	}
	

	fflush(stdout);
	printf("Hydrogen at the barrier.\n Carbon waiting: %d.\n Hydrogen waiting: %d\n\n", waiting_C, waiting_H);
	fflush(stdout);
	
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

