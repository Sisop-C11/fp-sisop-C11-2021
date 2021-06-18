/*
	C ECHO client example using sockets
*/
#include <stdio.h>	//printf
#include <string.h>	//strlen
#include <sys/socket.h>	//socket
#include <arpa/inet.h>	//inet_addr
#include <unistd.h>
#include <stdbool.h> 

char user[1024], passw[1024];

void substr(char *s, char *sub, int y, int z) {
   int aa = 0;
   while (aa < z) {
      sub[aa] = s[y + aa];
      aa++;
   }
   sub[aa] = '\0';
}

void handlePass(char filename[]){
	int charCount=0;
	int j = 12;
	memset(user, 0, 1024);
	memset(passw, 0, 1024);
	for(; j<strlen(filename); j++){
  		if(filename[j] == ' '){
  			break;
  		}
  		user[charCount] = filename[j];
  		charCount++;
  	}
  	
  	charCount=0;
  	j = j + 16;
	
	for(; j<strlen(filename); j++){
  		if(filename[j] == ';'){
  			break;
  		}
  		passw[charCount] = filename[j];
  		charCount++;
  	}
  
 	return;
}

void handlePerm(char filename[]){
	int charCount=0;
	int j = 17;
	memset(user, 0, 1024);
	memset(passw, 0, 1024);
	for(; j<strlen(filename); j++){
  		if(filename[j] == ' '){
  			break;
  		}
  		user[charCount] = filename[j];
  		charCount++;
  	}
  	
  	charCount=0;
  	j = j + 6;
	
	for(; j<strlen(filename); j++){
  		if(filename[j] == ';'){
  			break;
  		}
  		passw[charCount] = filename[j];
  		charCount++;
  	}
  
 	return;
}

int main(int argc , char *argv[])
{
	int sock, valread;
	struct sockaddr_in server;
	char message[1000] , server_reply[5000] , username[1024], pass[1024], temp[1024], choice[1000];
	
	//Create socket
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1)
	{
		printf("Could not create socket");
	}
	puts("Socket created");
	
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons( 8887 );

	//Connect to remote server
	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		perror("connect failed. Error");
		return 1;
	}
	
	puts("Please wait.\n");
	
	recv(sock , server_reply , 1024 , 0);
	
	puts("Connected\n");
	if(geteuid() != 0){
		strcpy(username, "l ");
    	strcat(username, argv[2]);
        strcat(username, ":");
        strcat(username, argv[4]);
        printf("%s\n", username);
        send(sock, username, strlen(username), 0);
        memset(server_reply, 0, 5000);
        if( recv(sock, server_reply, 1024 , 0) < 0)
		{
			puts("recv failed");
		}
        if(strcmp(server_reply, "success")==0)
        {
            printf("login success\n");
            send(sock, "sukses", 6, 0);
        }
        else if(strcmp(server_reply, "failure")==0)
        {
            printf("login failed\n");
        }
	}
	else{
		printf("No login needed\n");
	}
	screen1:
    printf("TEMPORARY LANDING PAGE\nwatchu wanna do : ");
    scanf("%[^\n]",choice); getchar();
    memset(temp, 0, 1000);
    memset(username, 0, 1000);
    memset(user, 0, 1024);
    memset(passw, 0, 1024);
    substr(choice, temp, 0, 11);
    printf("%s", temp);
    if(!strcmp(temp, "CREATE USER")) {
    	strcpy(username, "r ");
    	handlePass(choice);
        strcat(username, user);
        strcat(username, ":");
        strcat(username, passw);
        if( send(sock , username , strlen(username) , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
        memset(server_reply, 0, 1024);
        if( recv(sock , server_reply , 1024 , 0) < 0)
		{
			puts("recv failed");
		}
		printf("%s", server_reply);
    }
    memset(temp, 0, 1000);
    substr(choice, temp, 0, 16);
    if(!strcmp(temp, "GRANT PERMISSION")) {
    	strcpy(username, "g ");
    	handlePerm(choice);
        strcat(username, user);
        strcat(username, ":");
        strcat(username, passw);
        if( send(sock , username , strlen(username) , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
        memset(server_reply, 0, 1024);
        if( recv(sock , server_reply , 1024 , 0) < 0)
		{
			puts("recv failed");
		}
		printf("%s", server_reply);
    }
    
    memset(temp, 0, 1000);
    substr(choice, temp, 0, 15);
    if(!strcmp(temp, "CREATE DATABASE")) {
    	strcpy(username, "c ");
    	substr(choice, temp, 16, strlen(choice));
        strcat(username, temp);
        if( send(sock , username , strlen(username) , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
        memset(server_reply, 0, 1024);
        if( recv(sock , server_reply , 1024 , 0) < 0)
		{
			puts("recv failed");
		}
		printf("%s", server_reply);
    }
    goto screen1;
	
	close(sock);
	return 0;
}
