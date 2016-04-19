#include "mailbox.h"

void print_time();

int main(int argc, char const *argv[]) {

	CLIENT *clnt;

	//the server running the server program
	char *server_hostname = argv[1];

	clnt = clnt_create(server_hostname, MAILBOX, ASSIGNMENT_6, "udp");
	
	if (clnt == NULL) {
		clnt_pcreateerror("Error creating client");
		exit(EXIT_FAILURE);
	}

	//rpc arguments struct to pass to server
	struct rpc_args *args = malloc(sizeof(struct rpc_args));

	//START user
	if (strcmp(argv[2], "START") == 0) {

		strcpy(args->action, argv[2]);
		strcpy(args->user, argv[3]);

		struct rpc_args *response = action_1(args, clnt);

		if (response == NULL) {
			printf("User %s was already started\n", argv[3]);
		} else {
			printf("User %s has been started\n", argv[3]);
		}

	//QUIT user
	} else if (strcmp(argv[2], "QUIT") == 0) {

		strcpy(args->action, argv[2]);
		strcpy(args->user, argv[3]);

		struct rpc_args *response = action_1(args, clnt);

		if (response == NULL) {
			printf("User %s was already inactive\n", argv[3]);
		} else {
			printf("User %s has quitted\n", argv[3]);
		}

	//INSERT_MESSAGE user message_num message
	} else if (strcmp(argv[2], "INSERT_MESSAGE") == 0) {

		strcpy(args->action, argv[2]);
		strcpy(args->user, argv[3]);
		args->message_num = atoi(argv[4]);
		strcpy(args->message.content, argv[5]);

		struct rpc_args *response = action_1(args, clnt);

		if (response == NULL) {
			printf("Insertion failed\n");
		} else {
			printf("Insertion succeeded\n");
		}

	//RETRIEVE_MESSAGE user message_num
	} else if (strcmp(argv[2], "RETRIEVE_MESSAGE") == 0) {

		strcpy(args->action, argv[2]);
		strcpy(args->user, argv[3]);
		args->message_num = atoi(argv[4]);

		struct rpc_args *response = action_1(args, clnt);

		if (response == NULL) {
			printf("Retrieval failed\n");
		} else {
			printf("Message retrieved: %s\n", response->message.content);
		}
	
	//LIST_ALL_MESSAGES user
	} else if (strcmp(argv[2], "LIST_ALL_MESSAGES") == 0) {
	
		strcpy(args->action, argv[2]);
		strcpy(args->user, argv[3]);

		struct rpc_args *response = action_1(args, clnt);

		if (response == NULL) {
			printf("Message listing failed\n");
		} else {
			for (int i = 0; i < 20; ++i) {
				printf("Message %d: %s\n", i, response->messages[i].content);
			}
		}
	
	//DELETE_MESSAGE user message_num
	} else if (strcmp(argv[2], "DELETE_MESSAGE") == 0) {
	
		strcpy(args->action, argv[2]);
		strcpy(args->user, argv[3]);
		args->message_num = atoi(argv[4]);

		struct rpc_args *response = action_1(args, clnt);

		if (response == NULL) {
			printf("Message deletion failed\n"); 
		} else {
			printf("Message deletion succeeded\n");
		}
	}

	print_time();

	free(args);
	clnt_destroy(clnt);

	return EXIT_SUCCESS;
}

void print_time() {
	time_t current_time;
    char* c_time_string;

    current_time = time(NULL);

    /* Convert to local time format. */
    c_time_string = ctime(&current_time);

    printf("Current time is %s", c_time_string);
}