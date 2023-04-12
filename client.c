
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <pthread.h>
#include <signal.h>

struct arg_thread{
	int fd;
	char *client;
};




void *receive(void *data)
{
	char buffer[1024];

	char *name;
	int sock,temp;
	struct arg_thread *send_s;
	send_s =(struct arg_thread *) data;
	sock = send_s->fd;
	name = send_s->client;
	

	while(recv(sock,buffer,sizeof(buffer),0))
	{

		if(strncmp(buffer,name,sizeof(*name)))
		{	
			printf("# %s ",buffer);
			printf("\n");
		}

		memset(buffer,0,sizeof(buffer));
	}
}


void* send_(void *data){
	char txbuffer[1024],message[1024];
	
	char *name;
	int sock,temp;
	struct arg_thread *send_s;
	send_s =(struct arg_thread *) data;
	sock = send_s->fd;
	name = send_s->client;
	
	
	while(1){
		read(0,(void*)message,1024);
		printf("_______________________________\n");
		write(1,"YOU : ",6);
		//printf("%40s", message);	

		memset(txbuffer,0,sizeof(txbuffer));
		strcpy(txbuffer,name);
		strcat(txbuffer, message);
		printf("%s\n", txbuffer);
		
		temp = send (sock,txbuffer,sizeof(txbuffer),0);
		if (temp == 0)
			printf("%s\n", "Sending Error");

		memset(txbuffer,0,sizeof(txbuffer));
		memset(message,0,sizeof(message));
		printf("_______________________________\n");
	}
}


int main(int argc, char const *argv[])
{
	int sock,temp;
	struct sockaddr_in server;
	struct hostent *hp;
	char buffer[1024];
	char name[20],txbuffer[2048];
	char ch;
		printf("%s\n", "Enter Your Name : ");
		scanf("%s",name);
		strcat(name," --> ");
	


	printf("%s\n", "Initializing First Chat ...");
	sleep(1);

	printf("%s\n", "Creating Socket ...");
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == -1){
		perror("Socket Create Error : ");
		exit(1);
	}
	printf("%s\n", "Created Socket ...");
	
	server.sin_family = AF_INET;


	//Ip Adress
	char IP[20] = "127.0.0.1" ;
	printf("communating to : %s\n", IP);
	hp = gethostbyname(IP);
	memcpy(&server.sin_addr,hp->h_addr,hp->h_length);


     //Port
	short int port = 8888;
    printf("communating to port: %hd\n", port);

	server.sin_port=htons(port);


	printf("%s\n", "Connecting To My Server ...");
	temp = connect(sock, (struct sockaddr *) &server, sizeof(server));
	if(temp != 0){
		perror("Connection  Failed");
		exit(0);
	}
	printf("%s\n", "Connected to Server ...");

	pthread_t sendID,recvID;
	struct arg_thread send;
	send.fd = sock;
	send.client = name;

	pthread_create(&sendID,NULL,send_,(void *)&send);
	pthread_create(&recvID,NULL,receive,(void *)&send);


	pthread_join(sendID,NULL);
	pthread_join(recvID,NULL);

	close(sock);
	return 0;
}		
