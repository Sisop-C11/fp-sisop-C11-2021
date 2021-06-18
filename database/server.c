#include<stdio.h>
#include<string.h>	//strlen
#include<stdlib.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write
#include<pthread.h> //for threading , link with lpthread
#include <stdbool.h> 
#include <sys/stat.h>
#include <sys/types.h>

char akuns[200][1024] = {0}, tosend[5000], logined[200];
int akuncount = 0, filecount = 0;
bool someone = false;

//the thread function
void *connection_handler(void *);

void registerAccount(char msg[]){
	for(int i=2; i<strlen(msg); i++){
		akuns[akuncount][i-2]=msg[i];
	}
	akuncount++;
	FILE *fp;
	fp = fopen ("akun.txt", "a");
	fprintf(fp, "%s\n", akuns[akuncount-1]);
	fclose(fp);
}

bool loginAccount(char msg[]){
	char checker[strlen(msg)-1];
	memset(checker, 0, strlen(msg)-1);
	for(int i=2; i<strlen(msg); i++){
		checker[i-2]=msg[i];
	}
	for(int j=0; j<akuncount; j++){
		if(strcmp(akuns[j], checker)==0){
			strcpy(logined, checker);
			return true;
		}
	}
	return false;
}

int main(int argc , char *argv[])
{
	int socket_desc , client_sock , c , *new_sock;
	struct sockaddr_in server , client;
	
	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}
	puts("Socket created");
	
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( 8887 );
	
	//Bind
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
		//print the error message
		perror("bind failed. Error");
		return 1;
	}
	puts("bind done");
	
	//Listen
	listen(socket_desc , 10);
	
	FILE *filez;
    char perfile[1024];
    filez = fopen("akun.txt", "a+");
    if(filez == NULL) exit(0);
    while(fscanf(filez, "%s\n", perfile) != EOF)
    {
        strcpy(akuns[akuncount], perfile);
        akuncount++;
    }
    fclose(filez);
	
	int result = mkdir("/home/solxius/Desktop/Sisop/FP/Server/database", 0777);
	
	//Accept and incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);
	
	while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
	{
		puts("Connection accepted");
		
		pthread_t sniffer_thread;
		new_sock = malloc(1);
		*new_sock = client_sock;
		
		if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
		{
			perror("could not create thread");
			return 1;
		}
		
		//Now join the thread , so that we dont terminate before the thread
		//pthread_join( sniffer_thread , NULL);
		puts("Handler assigned");
	}
	
	if (client_sock < 0)
	{
		perror("accept failed");
		return 1;
	}
	
	return 0;
}

/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
	//Get the socket descriptor
	int sock = *(int*)socket_desc;
	int read_size;
	char *message , client_message[5000], apple[100];
	
	while(someone);
	send(sock, "ye", 2, 0 );
	
	someone = true;
	memset(client_message, 0, 5000);
	memset(tosend, 0, 5000);
	
	//Receive a message from client
	while( (read_size = recv(sock , client_message , 1024 , 0)) > 0 )
	{
		if(client_message[0]=='r'){
			registerAccount(client_message);
			send(sock, "registered", 10, 0 );
		}
		else if(client_message[0]=='l'){
			bool yo = loginAccount(client_message);
			if(yo){
				send(sock, "success", 10, 0 );
			}
			else{
				send(sock, "failure", 10, 0 );
			}
		}
		memset(client_message, 0, 5000);
		memset(tosend, 0, 5000);
	}
	
	if(read_size == 0){
		puts("Client disconnected");
		fflush(stdout);
	}
	else if(read_size == -1){
		perror("recv failed");
	}
		
	//Free the socket pointer
	someone=false;
	free(socket_desc);
	
	return 0;
}
