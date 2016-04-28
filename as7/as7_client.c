/*as7_client.c Nick Hudeck*/

#include "as7.h"
#include <time.h>
#include <unistd.h>
#define NUM_PUTS 5
#define MSG_LIMIT 15

// store the current time in a global array of 26 chars
char currentTime[26];

/* get the current time, mostly found on stack overflow */
void getTime() {
    time_t rawtime;
    struct tm * timeinfo;
    if (currentTime == NULL) {
        currentTime[0] = 0;
    }
    //current time in milliseconds
    time (&rawtime);
    if (rawtime == ((time_t)-1)) {
        perror("rawtime error");
        currentTime[0] = 0;
        return;
    }

    // gets in local time
    timeinfo = localtime (&rawtime);
    if (timeinfo == NULL) {
        perror("local time error");
        currentTime[0] = 0;
        return;
    }
    // gets time a readable manner
    asctime_r(timeinfo, currentTime);
    if (currentTime == NULL) {
        perror("readable time error");
        currentTime[0] = 0;
        return;
    }
}

// print success on 0 as per requirements
void printSuccess(int s) {
	getTime();

    fflush(stdout);
    if (s == 0) {
        printf ("Current time: %s | SUCCESS from SERVER\n", currentTime);
    } else {
        printf("Current time: %s | SUCCESS from SERVER\n", currentTime);
    }
    fflush(stdout);
}

void display_prg_1(char *host){
	CLIENT *clnt;
	srand(time(NULL));
	int host_id = (int)(rand() % (100));
	int  *result_1;
	int  get_1_arg;
	int  *result_2;
	struct data put_1_arg;

#ifndef	DEBUG
	clnt = clnt_create (host, DISPLAY_PRG, DISPLAY_VER, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	int i;
	//put messages 5 times per project requirements
	for (i = 0; i < 5; i++) {
        put_1_arg.id = host_id;
        
        if (strcpy(put_1_arg.message, "message") == NULL){
        	perror("strcopy error");
        }

        getTime();
        fflush(stdout);
	    printf ("Current time: %s | CLIENT #: %d PUT request\n", currentTime, host_id);
	    fflush(stdout);

        // put message with id on server
		result_2 = put_1(&put_1_arg, clnt);
		// signal error if occurs
	    if (result_2 == (int *) NULL) {
		    clnt_perror (clnt, "call failed");
	    }

	    // print the status of the server response
        printSuccess(*result_2);

	    // sleep one second between puts
	    sleep(1);
	}

    sleep(5);// sleep 5 seconds for project requirements

    // client calls 10 times per project requirements
    for (i = 0; i < 10; i++) {
    	// host id in message
        get_1_arg = host_id;
        getTime();
    	
        fflush(stdout);
    	printf("Current time: %s | Client #: %d  GET request.\n", currentTime, host_id);
    	fflush(stdout);

    	// server calls get
    	result_1 = get_1(&get_1_arg, clnt);

    	if (result_1 == (int *) NULL) {
    		clnt_perror (clnt, "call failed");
    	}

        printSuccess(*result_1);
    	sleep(1); // project requirements
    }

#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int main (int argc, char *argv[]){
	char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	display_prg_1 (host);
    exit (0);
}