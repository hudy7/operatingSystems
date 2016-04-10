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
int methane_count = 0;

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


	//should never reach here but program complete
	exit(EXIT_SUCCESS);
}


void carbon(void *arg){
	fflush(stdout);
	printf("Carbon spawn!\n");
	fflush(stdout);
	
	if(sem_wait(&mutex) != 0){
		perror("sem_wait");
		exit(EXIT_FAILURE);
	}
	
	fflush(stdout);
	printf("Carbon at the barrier.\nCarbon waiting: %d.\nHydrogen waiting: %d\n\n", waiting_C, waiting_H);
	fflush(stdout);


	/* if > 4 hyrdogens at barrier, enter */
	if(waiting_H >= 4){
		if(sem_post(&sem_h) != 0){
			perror("sem_post");
			exit(EXIT_FAILURE);
		}
		if(sem_post(&sem_h) != 0){
			perror("sem_post");
			exit(EXIT_FAILURE);
		}
		if(sem_post(&sem_h) != 0){
			perror("sem_post");
			exit(EXIT_FAILURE);
		}
		if(sem_post(&sem_h) != 0){
			perror("sem_post");
			exit(EXIT_FAILURE);
		}

		waiting_H = waiting_H - 4;
		
		methane_count =	methane_count + 1;
		
		fflush(stdout);
		printf("Methane Formed!\nNumber of methanes is now %d\n",	methane_count);
		fflush(stdout);
		
		if(sem_post(&mutex) != 0){
			perror("sem_post");
			exit(EXIT_FAILURE);
		}
	}
	
	else{ // wait for hydrogen
		waiting_C = waiting_C + 1;

		if(sem_post(&mutex) != 0){
			perror("sem_post");
			exit(EXIT_FAILURE);
		}
		if(sem_post(&sem_c) != 0){
			perror("sem_post");
			exit(EXIT_FAILURE);
		}
	}		
}
//hydrogen function
void hydrogen(void *arg){
	fflush(stdout);
	printf("Hydrogen spawn!\n");
	fflush(stdout);
	
	if(sem_wait(&mutex) != 0){
		perror("sem_wait");
		exit(EXIT_FAILURE);
	}
	

	fflush(stdout);
	printf("Hydrogen at the barrier.\nCarbon waiting: %d.\nHydrogen waiting: %d\n\n", waiting_C, waiting_H);
	fflush(stdout);
	
	
	/* if there are >=3 hydrogens and 1 or more carbon at barrier, enter */
	if(waiting_H >= 3 && waiting_C >= 1){
		if(sem_post(&sem_h) != 0){
			perror("sem_post");
			exit(EXIT_FAILURE);
		}
		if(sem_post(&sem_h) != 0){
			perror("sem_post");
			exit(EXIT_FAILURE);
		}
		if(sem_post(&sem_h) != 0){
			perror("sem_post");
			exit(EXIT_FAILURE);
		}
		if(sem_post(&sem_c) != 0){
			perror("sem_post");
			exit(EXIT_FAILURE);
		}
		/* update counts */
		waiting_H = waiting_H - 3;
		waiting_C = waiting_C - 1;
		methane_count =	methane_count + 1;

		fflush(stdout);
		printf("Methane Formed!\nNumber of methanes is now %d\n",	methane_count);
		fflush(stdout);

		if(sem_post(&mutex) != 0){
			perror("sem_post");
			exit(EXIT_FAILURE);
		}
		
	}
	
	else{ //wait for hydrogens and carbons
		waiting_H = waiting_H + 1;
		
		if(sem_post(&mutex) != 0){
			perror("sem_post");
			exit(EXIT_FAILURE);
		}

		if(sem_post(&sem_h) != 0){
			perror("sem_post");
			exit(EXIT_FAILURE);
		}
	}		
}

