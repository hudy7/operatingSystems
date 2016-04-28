/*as7_server.c Nick Hudeck*/

#include "as7.h"
#include <time.h>



char currentTime[26]; // store the current time in this string/char array
struct data clientMessages[15]; // allocating space for messages

/* initialize message IDs to -1 and adjust them as they come in */
int messageIDs[15] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
int currentMessage = 0; // current client message index
int currentClient = 0; //maintain when we get a new client
int currentClientIndex[3] = {-1, -1, -1}; //keep track of three clients each having messages
int clientList[3];

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

int * get_1_svc(int *argp, struct svc_req *rqstp){
	
	int *result = (int *)malloc(sizeof(int));
	if (result == NULL) { //unneccessary check I think but just to be safe
		perror("malloc error");
		int x;
		result = &x;
	}
    *result = -1; //initially sets the result as an error 
	
    getTime();

    fflush(stdout);
	printf("Current time: %s | SERVER received GET request from CLIENT #: %d\n", currentTime, *argp);
    fflush(stdout);
    
    // should be between 0 and 2 if in clientList created above
    int clientID = -1;
    int actual = *argp;
    /* 
   	checks to see where the client is in the server client list
   	and then translates it into the clientID 
   	*/
    int i;
    for (i = 0; i < 3; i++) {
    	if (clientList[i] == actual){
    		clientID = i;
    		break; 
    	}
    }

    /* loop to see if client has to GET any messages */
    for (i = 0; i < 15; i++){
    	if (clientID >= 0 && clientID <= 2 && i > currentClientIndex[clientID]){
			if (messageIDs[i] != actual && messageIDs[i] >= 0){
				*result = 0; // GREAT SUCCESS!
				currentClientIndex[clientID] = i;
				return result;
			}
    	}
    }
	return result;
}

int * put_1_svc(struct data *argp, struct svc_req *rqstp){

	int *result = (int *)malloc(sizeof(int));
	if (result == NULL) { //unneccessary check I think but just to be safe
		perror("malloc error");
		int x;
		result = &x;
	}
    *result = -1; //initially sets the result as an error

	getTime();
	fflush(stdout);
	printf("Current time: %s | SERVER received GET request from CLIENT #: %d.\n", currentTime, argp->id);
    fflush(stdout);

    
	if (currentMessage < 15){ // only 15 messages per project requirements
	    
	    int clientID = -1; // init error
	    

	    /*
	    goes through list and finds the correct client
	    */
	    int i;
	    for (i = 0; i < 3; i++) {
	    	if (clientList[i] == argp->id){
	    		clientID = i;
	    		break;
	    	}
	    }

		// store the message
	    if (strcpy(clientMessages[currentMessage].message, argp->message) == NULL){
	    	perror("strcpy error");
	    } 
	    else {
		    // store the clientUD
			clientMessages[currentMessage].id = argp->id;
			// store in messageIDs array
			messageIDs[currentMessage] = argp->id;

			/*
			add client to list if nec
			*/
			if (clientID == -1){
				clientList[currentClient] = argp->id;
				currentClient += 1;
			}
			currentMessage += 1;
			*result = 0; // great success!
	    }
	}
	return result;
}
