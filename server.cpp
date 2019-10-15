#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
using namespace std;


//TO DISPLAY ERROR
void error(const char* err)
{
	perror(err);
	exit(1);
}

//MAIN PROGRAM
int main(int argc,char* argv[])
{
//handling no port condition
	if(argc<2)
	{
		fprintf(stderr,"Port no not provided");
		exit(1);
	}
	int sockfd,newsockfd,portno,n;
	char buffer[256];
	struct sockaddr_in serv_addr,cli_addr;
	socklen_t clilen;
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd < 0)
	{
		error("Error in opening socket");
	}
	portno=atoi(argv[1]);
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=INADDR_ANY;
	serv_addr.sin_port=htons(portno);
	int bd=bind(sockfd,(struct sockaddr *) &serv_addr , sizeof(serv_addr));
	if(bd < 0)
	error("Binding failed");
//listening -------------------------------------------------	
	listen(sockfd,4);
	clilen=sizeof(cli_addr);
	newsockfd=accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if(newsockfd < 0)
	error("error on accept");
	FILE *fp = fopen ( "file1.txt"  , "wb" );
	int file_size;
	recv(newsockfd , &file_size,sizeof(file_size),0);
	while ( ( n = recv( newsockfd , buffer , sizeof(buffer), 0) ) 	> 0  &&  file_size > 0)
	{	
	fwrite(buffer , sizeof(char), n, fp);
	memset( buffer , '\0', sizeof(buffer));
	file_size = file_size - n;
	}
	/*int words;
	int ch=0;
	read(newsockfd, &words ,sizeof(words));
	n=read(newsockfd,buffer,256);
	while(ch !=words)
{
	read(newsockfd , buffer ,sizeof(buffer));
	fprintf(fp,"%s",buffer);
		//printf("error on reading");
	ch++;
}*/
printf("successfully received");
fclose(fp);
	close(sockfd);
	close(newsockfd);
	
}
