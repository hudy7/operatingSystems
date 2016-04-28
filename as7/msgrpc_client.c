/*
 * Nicholas Hudeck asn7
 * msgrpc_client.c
 */

#include "msgrpc.h"




/**Commenting out everything for now **/


void display_prg_1(char *host) {
    CLIENT *clnt;
    int  *result_1;
    char  get_1_arg;
    
    int  *result_2;
    struct data  put_1_arg;

#ifndef	DEBUG
    clnt = clnt_create (host, DISPLAY_PRG, DISPLAY_VER, "udp");

    if (clnt == NULL) {
      clnt_pcreateerror (host);
      exit (1);
    }

#endif	/* DEBUG */

    result_1 = get_1(&get_1_arg, clnt);
	
    if (result_1 == (int *) NULL) {
      clnt_perror (clnt, "call failed");
    }

    result_2 = put_1(&put_1_arg, clnt);
	
    if (result_2 == (int *) NULL) {
      clnt_perror (clnt, "call failed");
    }

#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}

int main (int argc, char *argv[]){
	CLIENT *client;
	char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	display_prg_1 (host);
exit (0);
} 
