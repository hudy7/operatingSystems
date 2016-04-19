#include "mailbox.h"
#include <rpc/rpc.h>

//used to store users and their messages. each user is a node in the list of users
//messages[message_num] is the requested message
struct user {
	char name[20];
	char messages[20][80];
	struct user *next_user;
};

//head of the list
static struct user *first_user = NULL;

//1 if user has been started, 0 otherwise
int user_started(char *name);
//insert the user into the list
void insert_user(char *name);
//remove the user from the list
void remove_user(char *name);
//insert the message into the specified user's messages
void insert_message(char *username, int message_num, char *message);
//retrieve the message from the specified user
char *get_message(char *username, int message_num);
//delete the message from the specified user
void delete_message(char *username, int message_num);

struct rpc_args *action_1_svc(struct rpc_args *args, struct svc_req *rqstp) {

	//rpc arguments struct to return to client
	struct rpc_args *response = malloc(sizeof(struct rpc_args));

	//START user
	if (strcmp(args->action, "START") == 0) {
		if (!user_started(args->user)) {
			insert_user(args->user);
			printf("User %s has been started\n", args->user);
		} else {
			printf("User %s has already been started\n", args->user);
			response = NULL;
		}

	//QUIT user
	} else if (strcmp(args->action, "QUIT") == 0) {
		if (user_started(args->user)) {
			remove_user(args->user);
			printf("User %s has quitted\n", args->user);
		} else {
			printf("User %s was already inactive\n", args->user);
			response = NULL;
		}

	//INSERT_MESSAGE user message_num message
	} else if (strcmp(args->action, "INSERT_MESSAGE") == 0) {
		if (user_started(args->user)) {
			insert_message(args->user, args->message_num, args->message.content);
		} else {
			printf("User %s not found\n", args->user);
			response = NULL;
		}

	//RETRIEVE_MESSAGE user message_num
	} else if (strcmp(args->action, "RETRIEVE_MESSAGE") == 0) {
		if (user_started(args->user)) {
			char *message = get_message(args->user, args->message_num);
			strcpy(response->message.content, message);
		} else {
			printf("User %s not found\n", args->user);
			response = NULL;
		}

	//LIST_ALL_MESSAGES user
	} else if (strcmp(args->action, "LIST_ALL_MESSAGES") == 0) {
		if (user_started(args->user)) {
			for (int i = 0; i < 20; ++i) {
				strcpy(response->messages[i].content, get_message(args->user, i));
			}
		} else {
			printf("User %s not found\n", args->user);
			response = NULL;
		}

	//DELETE_MESSAGE user message_num
	} else if (strcmp(args->action, "DELETE_MESSAGE") == 0) {
		if (user_started(args->user)) {
			delete_message(args->user, args->message_num);
		} else {
			printf("User %s not found\n", args->user);
			response = NULL;
		}
	}
	return response;
}

int user_started(char *name) {
	printf("checking for user %s\n", name);
	struct user *current_user = first_user;
	while (current_user != NULL) {
		if (strcmp(current_user->name, name) == 0) {
			return 1;
		} else {
			current_user = current_user->next_user;
		}
	}
	return 0;
}

void insert_user(char *name) {
	printf("inserting user %s\n", name);
	if (first_user == NULL) {
			first_user = malloc( sizeof(struct user) );
			strcpy(first_user->name, name);
			first_user->next_user = NULL;
	} else {
		struct user *current_user = first_user;

		while (current_user->next_user != NULL) {
			current_user = current_user->next_user;
		}

		current_user->next_user = malloc( sizeof(struct user) );
		strcpy(current_user->next_user->name, name);
		current_user->next_user->next_user = NULL;
	}
}

void remove_user(char *name) {
	printf("removing user %s\n", name);
	if (strcmp(first_user->name, name) == 0) {
		first_user = first_user->next_user;
	} else {
		struct user *previous_user = first_user;
		struct user *current_user = first_user->next_user;
		while (current_user != NULL) {
			if (strcmp(current_user->name, name) == 0) {
				previous_user->next_user = current_user->next_user;
				return;
			} else {
				previous_user = current_user;
				current_user = current_user->next_user;
			}
		}
	}
}

void insert_message(char *username, int message_num, char *message) {
	printf("inserting message %s\n", message);
	struct user *current_user = first_user;
	while (current_user != NULL) {
		if (strcmp(current_user->name, username) == 0) {
			strcpy(current_user->messages[message_num], message);
			return;
		} else {
			current_user = current_user->next_user;
		}
	}
}

char *get_message(char *username, int message_num) {
	struct user *current_user = first_user;
	while (current_user != NULL) {
		if (strcmp(current_user->name, username) == 0) {
			printf("retrieved message %s\n", current_user->messages[message_num]);
			return current_user->messages[message_num];
		} else {
			current_user = current_user->next_user;
		}
	}
	return NULL;
}

void delete_message(char *username, int message_num) {
	struct user *current_user = first_user;
	while (current_user != NULL) {
		if (strcmp(current_user->name, username) == 0) {
			printf("deleting message %s\n", current_user->messages[message_num]);
			strcpy(current_user->messages[message_num], "\0");
			return;
		} else {
			current_user = current_user->next_user;
		}
	}
}