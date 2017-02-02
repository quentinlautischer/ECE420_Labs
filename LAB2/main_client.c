#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>


int port = 0;
int array_size = 0;

void *Talk (void *args);

int main(int argc, char* argv[])
{
	int num_threads = 20;
	pthread_t thread_handles[num_threads];



	if (argc != 3) Usage(argv[0]);
	port = atoi(argv[1]);
	array_size = atoi(argv[2]);

	for(int i=0;i<num_threads;i++)      //can support 20 clients at a time
	{
		printf("\nCreating Talker %d\n",i);
		pthread_create(&thread_handles,NULL,Talk,(void *)i);
	}
	for(int i=0;i<num_threads;i++)      //can support 20 clients at a time
	{
		printf("\nJoining Talker %d\n",i);
		pthread_join(thread_handles[i], NULL);
	}


	return 0;
}


void *Talk (void *args) { //port, array_size, rank

	long rank = (long)args;

	struct sockaddr_in sock_var;
	int clientFileDescriptor=socket(AF_INET,SOCK_STREAM,0);
	char str_clnt[20],str_ser[20];

	sock_var.sin_addr.s_addr=inet_addr("127.0.0.1");
	sock_var.sin_port=port;
	sock_var.sin_family=AF_INET;

	if(connect(clientFileDescriptor,(struct sockaddr*)&sock_var,sizeof(sock_var))>=0)
	{
		printf("Current client is: %d\n",clientFileDescriptor);
		printf("\nEnter String to send for client %d: ", clientFileDescriptor);
		fgets(str_clnt, 20, stdin);
		write(clientFileDescriptor,str_clnt,20);
		read(clientFileDescriptor,str_ser,20);
		printf("String from Server: %s\n",str_ser);
		close(clientFileDescriptor);
	}
	else{
		printf("socket creation failed");
	}
	return 0;
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