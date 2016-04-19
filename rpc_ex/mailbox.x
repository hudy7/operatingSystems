struct message {
	char content[80];
};

struct rpc_args {
	char action[20];
	char user[20];
	struct message message;
	struct message messages[20];
	int message_num;
};

program MAILBOX {
    version ASSIGNMENT_6 {
    	rpc_args ACTION(struct rpc_args) = 1;
	} = 1;
} = 0x20fff100;

