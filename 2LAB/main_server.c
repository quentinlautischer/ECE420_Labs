#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include "timer.h"

#define STRMAX 200
#define NUMTHREADS 1000

typedef struct {
	int readers;
	int writer;
	pthread_cond_t readers_proceed;
	pthread_cond_t writer_proceed;
	int pending_writers;
	pthread_mutex_t read_write_lock;
} mylib_rwlock_t;


int array_size;
char theArray[1024][STRMAX];
pthread_mutex_t mutex;

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
				int err = pthread_create(&t[i],NULL,ServerEcho,(void *)clientFileDescriptor);
				if (err) {
					printf("Creating thread %d encountered an error. Error no. %d\n", i, err);
				}
			}
			for(int i=0;i<NUMTHREADS;i++) {
				int err = pthread_join(t[i], NULL);
				if (err) {
					printf("Joining encountered an error on thread %d. Error no. %d\n", i, err);
				}
			}
			GET_TIME(finish);
			elapsed = finish - start;	
			print_array();
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
		sprintf(str, "String %d has been modified by a write request", index); 
		sprintf(theArray[index], "%s", str);
	} else {
		sprintf(str,theArray[index],STRMAX);	
	}
	pthread_mutex_unlock(&mutex);

	write(clientFileDescriptor,str,STRMAX);
	
	read(clientFileDescriptor,str,STRMAX);

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
