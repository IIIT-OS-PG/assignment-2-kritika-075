#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fstream>
#include <arpa/inet.h>
using namespace std;

int newsockfd;
   
    //TO DISPLAY ERROR
    void error(const char *err)
    {
        perror(err);
        //return 0;
        exit(1);
    }

    void* msg_passing(void *agr)
    {
        int portno, n;
        struct sockaddr_in serv_addr;
        struct hostent *server;

        //port from command line argument
        int *st=(int*)agr;
        portno =*st;

        //establishing socket connection
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
            error("error opening socket");
        else
            printf("client_socket created\n");

        //serv_Addr
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(portno);
        serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

        //connecting
        newsockfd=connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
        if (newsockfd < 0)
            error("client connection establishment failed");
        else
            printf("connected to server-\n");

        //send(sockfd,"send data",sizeof("send data"),0);    


        //opening file
         FILE *fp=fopen("pal.txt","a+");
        //FILE *fp=fopen("pal.txt","a+");
            
        //receiving data from server of other client
        int file_size;    
        char buffer[512];
        recv(sockfd , &file_size,sizeof(file_size),0);
        while ( ( n = recv(sockfd , buffer , sizeof(buffer), 0) ) 	> 0  &&  file_size > 0)
        {	
           // cout<<"BUffer"<<buffer<<endl;
           fputs(buffer,fp);
            //fwrite(buffer , sizeof(char), n, fp);
               //memset( buffer , '\0', sizeof(buffer));
          // fputs("\n",fp);
            file_size = file_size - n;
          
        }
//memset( buffer , '\0', sizeof(buffer));
        printf("successfully received");
        
        fclose(fp);
        close(sockfd);

       pthread_exit(0);
    }
//-----------------------------------------server thread for chuncks transfer----------------------------------------------    

void* file_chuncks(void *value)
{
        char buffer1[512];
        int size=sizeof(buffer1);
        // int n=recv( newsockfd , buffer1 , sizeof(buffer1), 0);

        // if(n<0)
        // error("error on reading");
        // printf("client %s\n",buffer1);

        //value of the offset
        int *s=(int *)value;
        int i=*s;

        //file passing 
        FILE *fp = fopen("client.cpp", "rb");
        //int words = 0;
        //char c;
        fseek(fp, 0, i*512);
        int file_size = 512;

        //sending data
        char buffer[512];
        int n;
        send(newsockfd, &file_size, sizeof(file_size), 0);
        while ((n = fread(buffer, sizeof(char), 512, fp)) > 0 && file_size > 0)
        {
            send(newsockfd, buffer, n, 0);
            memset(buffer, '\0', sizeof(buffer));
            file_size = file_size - n;
        }

        fclose(fp);
        //pthread_exit(0);
        
}

//---------------------------------------------client program------------------------------------------------------------

    void *client(void *)
    {
        int a;
        cin>>a;
       
        if(a==1)
        {

        int p1=8000;
        int p2=8001;
        int p3=8002;
        pthread_t peer_1[3];
    
        //create attribute
        pthread_attr_t attr;

        //initialize attribute 
        pthread_attr_init(&attr);

        //create server and client thread
        pthread_create(&peer_1[0],&attr,msg_passing,&p1);
        pthread_create(&peer_1[1],&attr,msg_passing,&p2); 
        pthread_create(&peer_1[2],&attr,msg_passing,&p3);



        pthread_join(peer_1[0],NULL);
        pthread_join(peer_1[1],NULL);
        pthread_join(peer_1[2],NULL);

        
        }
      
        pthread_exit(0);
    }

//--------------------------------------------------------------server program----------------------------------------

 void * server(void* argc)
    {
        
        //command line arguement
        if(argc==NULL)
        {
            fprintf(stderr,"Port no not provided");
            exit(1);
        }

        int portno,n;

        // char buffer[256];
        struct sockaddr_in serv_addr,cli_addr;
        socklen_t clilen;

        //server socket creation
        int sockfd=socket(AF_INET,SOCK_STREAM,0);

        if(sockfd < 0)
        {
            error("Error in opening socket");
        }

        char buffer[512];


        //command line arguement
        int *st=(int*) argc;
        portno=*st;
        cout<<portno<<endl;

        serv_addr.sin_family=AF_INET;
        //serv_addr.sin_addr.s_addr=INADDR_ANY;
        serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
        serv_addr.sin_port=htons(portno);

        //binding
        int bd=bind(sockfd,(struct sockaddr *) &serv_addr , sizeof(serv_addr));

        if(bd < 0)
        error("Binding failed");

        //listening	
        listen(sockfd,4);
         clilen=sizeof(cli_addr);

        //accepting connection from client
        newsockfd=accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if(newsockfd < 0)
        error("error on accept");


        pthread_t piece[3];
        int i1=0;
        int i2=1;
        int i3=2;

     //create attribute
        pthread_attr_t attr;

        //initialize attribute 
        pthread_attr_init(&attr);

        //create server and client thread
        pthread_create(&piece[0],&attr,file_chuncks,&i1);
        pthread_create(&piece[1],&attr,file_chuncks,&i2); 
        pthread_create(&piece[2],&attr,file_chuncks,&i3);

        
            //main should wait until both threads complete
        pthread_join(piece[0],NULL);
        pthread_join(piece[1],NULL);
            //main should wait until both threads complete
        pthread_join(piece[2],NULL);
        
        printf("file succesfully transfered\n");

        close(newsockfd);
        close(sockfd);
	pthread_exit(0);
}    

 //----------------------------------------------------------pthread creation------------------------------------------------
    //MAIN PROGRAM

    int main(int argc,char** argv)
    {

        //string s;
        //cin>>s;

        pthread_t peer[2];

        //tracker-port
        //int trac_port=9950;

        //create attribute
        pthread_attr_t attr;

        //initialize attribute
        pthread_attr_init(&attr);

        int port=atoi(argv[1]);

        //create srever and client thread and tracker thread
        //pthread_create(&peer[0],&attr,tracker,&trac_port);
        pthread_create(&peer[1],&attr,client,NULL); 
        pthread_create(&peer[0],&attr,server,&port);
     

        //main should wait until both threads complete
        pthread_join(peer[0],NULL);
        pthread_join(peer[1],NULL);
    

        return 0;       
    }
