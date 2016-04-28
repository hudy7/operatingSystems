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
/*
	// remove newline from end, just put terminator
	int new_line = strlen(currentTime) - 1;
	if (currentTime[new_line] == '\n') {
		currentTime[new_line] = '\0';
	}*/
}

int *
get_1_svc(int *argp, struct svc_req *rqstp)
{
	// initially, store error result
	int *result = (int *)malloc(sizeof(int));
	if (result == NULL) {
		perror("result malloc failed...");
		int n;
		result = &n;
	}
    *result = -1;
	// set the most updated time for the global time array
    getTime();
	printf("[%s] Server received a GET request from client %d.\n", currentTime, *argp);
    fflush(stdout);

	//check if there are any messages received from clients other than the one specified
    int i;
    
    // should be between 0 and 2 if in id list, else cannot see any messages
    int client_id = -1;
    int real_client_id = *argp;
    // get client id
    for (i = 0; i < 3; i++) {
    	// check to see where the client is in the server client list
    	if (clientList[i] == real_client_id){
    		// found the client, now set it to keep track
    		client_id = i;
    		// translated client id, now leave loop
    		break;
    	}
    }

    // see if the client has any messages to "get"
    for (i = 0; i < 15; i++){
    	// limit to only 3 clients
    	if (client_id >= 0 && client_id <= 2 && i > currentClientIndex[client_id]){
            // check if the client id is in the list of ids and not equal to this id
			if (messageIDs[i] != real_client_id && messageIDs[i] >= 0){
				// set success status
				*result = 0;
				// update the current client message index
				currentClientIndex[client_id] = i;
				// found a message, now return
				return result;
			}
    	}
    }
	return result;
}

int *
put_1_svc(struct data *argp, struct svc_req *rqstp)
{
	// initialize result in memory to error status
	int *result = (int *)malloc(sizeof(int));
	if (result == NULL) {
		perror("result malloc failed...");
		int n;
		result = &n;
	}
	*result = -1;
	// set the most updated time for the global time array
	getTime();
	printf("[%s] Server received a PUT request from client %d.\n", currentTime, argp->id);
    fflush(stdout);

    // only put new message if the message limit has not been met
	if (currentMessage < 15){
	    // should be between 0 and 2, else client not in list
	    int client_id = -1;
	    int i;
	    // get client id
	    for (i = 0; i < 3; i++) {
	    	// check to see where the client is in the server client list
	    	if (clientList[i] == argp->id){
	    		// found the client, now set it to keep track
	    		client_id = i;
	    		// translated client id, now leave loop
	    		break;
	    	}
	    }

		// store client msg
	    if (strcpy(clientMessages[currentMessage].message, argp->message) == NULL){
	    	perror("failure putting message on server...");
	    } else {
		    // store client id
			clientMessages[currentMessage].id = argp->id;

			// store id in ordered id list
			messageIDs[currentMessage] = argp->id;

			// add user to client list only if necessary
			if (client_id == -1){
				// add this client to the clients list
				clientList[currentClient] = argp->id;
				// increment curr client counter
				currentClient += 1;
			}
			// increment current client msg index
			currentMessage += 1;
			// set success result
			*result = 0;
	    }
	}
	return result;
}
