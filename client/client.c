/*
	C ECHO client example using sockets
*/
#include <stdio.h>	//printf
#include <string.h>	//strlen
#include <sys/socket.h>	//socket
#include <arpa/inet.h>	//inet_addr
#include <unistd.h>
#include <stdbool.h> 

void takeFile(char msg[], char namez[]){
  	char totalfile[1024];
  	strcpy(totalfile, namez);
  	
	FILE *fp;
	fp = fopen (totalfile, "w");
	fprintf(fp, "%s", msg);
	fclose(fp);
	puts("File terdownload");
}

int main(int argc , char *argv[])
{
	int sock, valread, choice;
	struct sockaddr_in server;
	char message[1000] , server_reply[5000] , username[1024], pass[1024], temp[1024];
	
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
	query_user[strlen(query_user)-1] = '\0';
	if (strstr(query_user, "CREATE USER")!=NULL)
	{
		if (strstr(query_user, "IDENTIFIED BY")==NULL) {
			 printf("Invalid syntax for create user.\n");
		return;
	    }
	    split_string(query_user);
		strcpy(username, "r ");
		strcat(username, argv[2]);
		strcat(username, ":");
		strcat(username, argv[4]);
		printf("%s\n", username);
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
		printf("%s\n", server_reply);
	}
	
	screen1:
    printf("TEMPORARY LANDING PAGE\nwatchu wanna do : ");
    scanf("%d", &choice);
    
	
	close(sock);
	return 0;
}
