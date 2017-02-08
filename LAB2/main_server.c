#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include "timer.h"

#define STRMAX 64
#define NUMTHREADS 20

int array_size;
pthread_mutex_t mutex;
char theArray[1024][STRMAX];

void Usage (char* prog_name);
void *ServerEcho(void *args);
void initialize_string_array();
void print_array();

int main(int argc, char* argv[])
{
	double start, finish, elapsed;	
	struct sockaddr_in sock_var;
	int serverFileDescriptor=socket(AF_INET,SOCK_STREAM,0);
	int clientFileDescriptor;
	pthread_t t[NUMTHREADS];
	pthread_mutex_init(&mutex, NULL);

	if (argc != 3) Usage(argv[0]);
	
	array_size = atoi(argv[2]);

	sock_var.sin_addr.s_addr=inet_addr("127.0.0.1");
	sock_var.sin_port=atoi(argv[1]);
	sock_var.sin_family=AF_INET;

	initialize_string_array();
	
	if(bind(serverFileDescriptor,(struct sockaddr*)&sock_var,sizeof(sock_var))>=0) {
		printf("\nServer Socket has been created\n");
		listen(serverFileDescriptor,2000); 
		while(1) {       //loop infinity... why? I think maybe... sure
			GET_TIME(start);
			for(int i=0;i<NUMTHREADS;i++) {
				clientFileDescriptor=accept(serverFileDescriptor,NULL,NULL);
				pthread_create(&t[i],NULL,ServerEcho,(void *)clientFileDescriptor);
			}
			for(int i=0;i<NUMTHREADS;i++) {
				pthread_join(t[i], NULL);
			}
			GET_TIME(finish);

			print_array();
			
			elapsed = finish - start;	
	 		printf("The elapsed time is %e seconds\n", elapsed);
		}
		close(serverFileDescriptor);
	}
	else {
		printf("Socket creation failed\n");
	}
	return 0;
}

/*------------------------------------------------------------------
 * Function:  Usage
 * Purpose:   print a message showing what the command line should
 *            be, and terminate
 * In arg :   prog_name
 */
void *ServerEcho(void *args) {
	int clientFileDescriptor=(int)args;
	char str[STRMAX];

	read(clientFileDescriptor,str,STRMAX);

	char* pos = str;
	pos++;
	int index = atoi(pos);

	pthread_mutex_lock(&mutex); 
	if (str[0] == 'w') {
		sprintf(theArray[index], "String %d has been modified by a write request", index);
		sprintf(str, "%s", theArray[index]);
	} else {
		sprintf(str, "%s", theArray[index]);
	}
	pthread_mutex_unlock(&mutex);

	printf("SERVER: writing message to client: %s\n", str);
	write(clientFileDescriptor,str,STRMAX);
	
	// char str2[STRMAX];
	// read(clientFileDescriptor,str2,STRMAX);

	// printf("SERVER: read message from client: %s\n", str2);
	close(clientFileDescriptor);
}

/*------------------------------------------------------------------
 * Function:  Usage
 * Purpose:   print a message showing what the command line should
 *            be, and terminate
 * In arg :   prog_name
 */
void Usage (char* prog_name) {
    fprintf(stderr, "usage: %s <port number> <array length>\n", prog_name);
    exit(0);
}  /* Usage */

void initialize_string_array() {
	for (int i = 0; i < array_size; i ++) {
		sprintf(theArray[i], "String %d: the initial value", i);
	}
	print_array();
}

void print_array(){
	for (int i = 0; i < array_size; i ++) {
		printf("%s\n", theArray[i]);
	}
}

