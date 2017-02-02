#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>

void *ServerEcho(void *args)
{
	int clientFileDescriptor=(int)args;
	char str[20];

	read(clientFileDescriptor,str,20);
	printf("\nreading from client %d: %s",clientFileDescriptor,str);
	write(clientFileDescriptor,str,20);
	printf("\nechoing back to client %d", clientFileDescriptor);
	close(clientFileDescriptor);
}


int main(int argc, char* argv[])
{
	struct sockaddr_in sock_var;
	int serverFileDescriptor=socket(AF_INET,SOCK_STREAM,0);
	int clientFileDescriptor;
	int i;
	pthread_t t[20];

	if (argc != 3) Usage(argv[0]);

	sock_var.sin_addr.s_addr=inet_addr("127.0.0.1");
	sock_var.sin_port=atoi(argv[1]);
	sock_var.sin_family=AF_INET;
	if(bind(serverFileDescriptor,(struct sockaddr*)&sock_var,sizeof(sock_var))>=0)
	{
		printf("\nsocket has been created");
		listen(serverFileDescriptor,2000); 
		while(1)        //loop infinity
		{
			for(i=0;i<20;i++)      //can support 20 clients at a time
			{
				clientFileDescriptor=accept(serverFileDescriptor,NULL,NULL);
				printf("\nConnected to client %d\n",clientFileDescriptor);
				pthread_create(&t,NULL,ServerEcho,(void *)clientFileDescriptor);
			}
		}
		close(serverFileDescriptor);
	}
	else{
		printf("\nsocket creation failed\n");
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