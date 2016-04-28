/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 * NOTE: The shell running this program must have fortune installed.
 */

#include "as7.h"
#include <time.h>
#include <unistd.h>
#define NUM_PUTS 5
#define MSG_LIMIT 15

// store the current time in a global array of 26 chars
char curr_time[26];

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
    if (status == 0) {
        printf ("Current time: %s | SUCCESS from SERVER\n", currentTime);
    } else {
        printf("Current time: %s | SUCCESS from SERVER\n", currentTime);
    }
    fflush(stdout);
}

// uses the fortune program to generate a random string, returns a pointer to that string
/*char *get_random_string(){
    FILE *fp;

    // variables for creating random string
    char *random_str = NULL;
    char *temp = NULL;

    // use a temporary buffer to incrementally gather results
    char buf[100];

    // maintain size variables for dynamically adjusting the string size
    unsigned int size = 1;
    unsigned int str_length;

    // call fortune to generate a real random string
    fp = popen("fortune", "r");

    // error-check fortune teller response
    if (fp == NULL) {
        perror("Error running fortune to generate random string...");
	    fflush(stderr);
	    if (pclose(fp) == -1){
	        perror("Error closing fortune process...");
	        fflush(stderr);
	    }
	    return "error generating random string...";
    }

    // attain your fortune by writing all string characters to random_str incrementally
    while (fgets(buf, sizeof(buf), fp) != NULL) {
    	// get the current buf size
    	str_length = strlen(buf);
    	// dynamically reallocate the random string with a new size
        temp = realloc(random_str, size + str_length);
        // error-check reallocation
    	if (temp == NULL) {
        	perror("temporary random string memory allocation failed...");
            return "";
        } else {
        	// set the random string on success
        	random_str = temp;
        }
    	// copy the buffer to the random string
        if (strcpy(random_str + size - 1, buf) == NULL){
        	perror("error copying random string...");
        }
        // increase the size variable by the added string length
        size += str_length;
    }

    // error-check closing fortune process
	if (pclose(fp) == -1){
	    perror("Error closing fortune process...");
	    return "error closing fortune process...";
	}
    return random_str;
}
*/

void display_prg_1(char *host){
	CLIENT *clnt;
	srand(time(NULL));
	int host_id = (int)(rand() % (100 + 1 - 0) + 0);
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
        char *mess = "message";
        if (strcpy(put_1_arg.message, mess) == NULL){
        	perror("strcopy error");
        }

        free(mess);


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