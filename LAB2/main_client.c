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

int port = 0;
int array_size = 0;
int* seed;

void *Talk (void *args);
void Usage (char* prog_name);

int main(int argc, char* argv[])
{
	double start, finish, elapsed;
	pthread_t *thread_handles;

	if (argc != 3) Usage(argv[0]);

	port = atoi(argv[1]);
	array_size = atoi(argv[2]);

	thread_handles = malloc (NUMTHREADS * sizeof(pthread_t));

	/* Intializes random number generators */
	seed = malloc(NUMTHREADS*sizeof(int));
	for (int i = 0; i < NUMTHREADS; i++)
		seed[i] = i;

	GET_TIME(start);

	for(int i=0;i<NUMTHREADS;i++) {
		int err = pthread_create(&thread_handles[i],NULL,Talk,(void *)i);
		if (err) {
			printf("Creating thread %d encountered an error. Error no. %d\n", i, err);
		}
	}

	for(int i=0;i<NUMTHREADS;i++) {
		int err = pthread_join(thread_handles[i], NULL);
		if (err) {
			printf("Joining encountered an error on thread %d. Error no. %d\n", i, err);
		}
	}

	GET_TIME(finish);
			
	elapsed = finish - start;	
	printf("The elapsed time is %e seconds\n", elapsed);

	free(seed);
	free(thread_handles);

	return 0;
}

void *Talk (void *args) { //port, array_size, rank
	long rank = (long)args;

	struct sockaddr_in sock_var;
	int clientFileDescriptor=socket(AF_INET,SOCK_STREAM,0);
	char str_clnt[STRMAX],
	     str_ser[STRMAX];

	sock_var.sin_addr.s_addr=inet_addr("127.0.0.1");
	sock_var.sin_port=port;
	sock_var.sin_family=AF_INET;

	// Find a random position in theArray for read or write
	int pos = rand_r(&seed[rank]) % 100;
	int randNum = rand_r(&seed[rank]) % 100;	// write with 5% probability

	int connect_result = connect(clientFileDescriptor,(struct sockaddr*)&sock_var,sizeof(sock_var));
	if(connect_result >= 0) {
		if (randNum <= 95) { // read request
			snprintf(str_clnt, STRMAX, "r%d", pos);
		}
		else { //write request
			snprintf(str_clnt, STRMAX, "w%d", pos);
		}
		write(clientFileDescriptor,str_clnt,STRMAX);
		if (!read(clientFileDescriptor, str_ser, STRMAX))
		{
			printf("Read error was encountered in %d\n");
		}
		printf("%s\n", str_ser);
		write(clientFileDescriptor,str_clnt,STRMAX);
		close(clientFileDescriptor);
	}
	else {
		printf("Socket creation failed for thread %d. Client file descriptor: %d. Connect error no. %d\n", rank, clientFileDescriptor, connect_result);
	}

	return 0;
}

void Usage (char* prog_name) {
    fprintf(stderr, "Improper Usage/Arguments: %s <port number> <array length>\n", prog_name);
    exit(0);
}
