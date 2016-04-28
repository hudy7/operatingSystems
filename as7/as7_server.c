/**
 * The function templates in this code were developed by rpcgen.
 * The functionality was programmed in for as7, eecs 338.
 * @author Shaun Howard
 */

#include "as7.h"
#include <time.h>

// set message limit for server
#define MSG_LIMIT 15

// track whether the client data has been initialized
int is_initialized = 0;

// store the current time in an array of 26 chars
char curr_time[26];

// allocate space for each of 3 clients to put 5 messages on the server
struct client_data client_msgs[MSG_LIMIT];

// initialize all client msg ids to -1, incrementally change based on messages put in fcfs order
int client_msg_ids[15] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

// store the current client msg index, which should be at max equal to MSG_LIMIT
int curr_index = 0;

// store the current client index to maintain a new client addition to client list
int curr_client = 0;

// track msg indices for each client
int curr_client_indices[3] = {-1, -1, -1};

// store the current client ids in a list, only support 3 clients
int client_list[3];

// sets the current time and date to the "curr_time" char buffer
void set_time() {
	time_t rawtime;
	struct tm * timeinfo;

	// reinitialize curr_time buff if it is null
	if (curr_time == NULL) {
		curr_time[0] = 0;
	}

	// get the current time in ms
	time (&rawtime);
	if (rawtime == ((time_t)-1)) {
		perror("error generating time information...");
		curr_time[0] = 0;
		return;
	}

	// convert time to local time
	timeinfo = localtime (&rawtime);
    if (timeinfo == NULL) {
        perror("error converting time to local time...");
        curr_time[0] = 0;
        return;
    }

    // convert time to asctime
    asctime_r(timeinfo, curr_time);
	if (curr_time == NULL) {
		perror("error converting time to asc time...");
		curr_time[0] = 0;
		return;
	}

	// remove newline from end, just put terminator
	int new_line = strlen(curr_time) - 1;
	if (curr_time[new_line] == '\n') {
		curr_time[new_line] = '\0';
	}
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
    set_time();
	printf("[%s] Server received a GET request from client %d.\n", curr_time, *argp);
    fflush(stdout);

	//check if there are any messages received from clients other than the one specified
    int i;
    
    // should be between 0 and 2 if in id list, else cannot see any messages
    int client_id = -1;
    int real_client_id = *argp;
    // get client id
    for (i = 0; i < 3; i++) {
    	// check to see where the client is in the server client list
    	if (client_list[i] == real_client_id){
    		// found the client, now set it to keep track
    		client_id = i;
    		// translated client id, now leave loop
    		break;
    	}
    }

    // see if the client has any messages to "get"
    for (i = 0; i < MSG_LIMIT; i++){
    	// limit to only 3 clients
    	if (client_id >= 0 && client_id <= 2 && i > curr_client_indices[client_id]){
            // check if the client id is in the list of ids and not equal to this id
			if (client_msg_ids[i] != real_client_id && client_msg_ids[i] >= 0){
				// set success status
				*result = 0;
				// update the current client message index
				curr_client_indices[client_id] = i;
				// found a message, now return
				return result;
			}
    	}
    }
	return result;
}

int *
put_1_svc(struct client_data *argp, struct svc_req *rqstp)
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
	set_time();
	printf("[%s] Server received a PUT request from client %d.\n", curr_time, argp->id);
    fflush(stdout);

    // only put new message if the message limit has not been met
	if (curr_index < MSG_LIMIT){
	    // should be between 0 and 2, else client not in list
	    int client_id = -1;
	    int i;
	    // get client id
	    for (i = 0; i < 3; i++) {
	    	// check to see where the client is in the server client list
	    	if (client_list[i] == argp->id){
	    		// found the client, now set it to keep track
	    		client_id = i;
	    		// translated client id, now leave loop
	    		break;
	    	}
	    }

		// store client msg
	    if (strcpy(client_msgs[curr_index].message, argp->message) == NULL){
	    	perror("failure putting message on server...");
	    } else {
		    // store client id
			client_msgs[curr_index].id = argp->id;

			// store id in ordered id list
			client_msg_ids[curr_index] = argp->id;

			// add user to client list only if necessary
			if (client_id == -1){
				// add this client to the clients list
				client_list[curr_client] = argp->id;
				// increment curr client counter
				curr_client += 1;
			}
			// increment current client msg index
			curr_index += 1;
			// set success result
			*result = 0;
	    }
	}
	return result;
}
