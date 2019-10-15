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

int sockfd;

//TO DISPLAY ERROR
void error(const char *err)
{
    perror(err);
    //return 0;
    exit(1);
}


void create_socket()
{
        int n;
        struct sockaddr_in serv_addr;
        struct hostent *server;

        //port from command line argument

        //establishing socket connection
         sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
            error("error opening socket");
        else
            printf("client_socket created\n");

        int port = 9898;

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);
        serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

        //connecting
        int newsock = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
        if (newsock < 0)
            error("client connection establishment failed");
        else
            printf("connected to server-\n");
}
//---------------------------------------------login()-----------------------------------------------------------------

//MAIN PROGRAM

int main()
{

    create_socket();
    cout<<sockfd;//establishing connection
     while (1)
     {

       
        //---------------------------------------------------login----------------------------------------------------------
        string str;
        getline(cin, str);//taking command

        if (str == "login")
        {
          
     

            int type=1;
            //send(sockfd, "1", sizeof("1"), 0);
            write(sockfd,&type,sizeof(type));


            int s;
            string user;
            string password;
            char user_1[512];
            char password_1[512];
            char buffer[512];
            cout << "hii  ";
                  cout << "hello";

                getline(cin, user);
                getline(cin, password);

                int n;
                memset(buffer, '\0', sizeof(buffer));
                strcpy(user_1, user.c_str());
                strcpy(password_1, password.c_str());

                send(sockfd, &user_1, sizeof(user_1), 0);
                //recv(sockfd, buffer, sizeof(buffer), 0);
                memset(buffer, '\0', sizeof(buffer));
                send(sockfd, &password_1, sizeof(password_1), 0);
                //recv(sockfd, buffer, sizeof(buffer), 0);
                read(sockfd,&s,sizeof(s));
                if(s==1)
            printf("successfully logged in\n");
            else
            {
                printf("user does not exist");
            }
        }
        else if (str == "create user")
        {
            int s;
            string user;
            string password;
            char user_1[512];
            char password_1[512];
            char buffer[512];
            //cout<<"hii  ";
            do
            {
                //cout<<"hello";
                getline(cin, user);
                getline(cin, password);
                int type=2;
                write(sockfd, &type, sizeof(type));
                int n;
                memset(buffer, '\0', sizeof(buffer));
                strcpy(user_1, user.c_str());
                strcpy(password_1, password.c_str());
                send(sockfd, &user_1, sizeof(user_1), 0);
                memset(buffer, '\0', sizeof(buffer));
                send(sockfd, &password_1, sizeof(password_1), 0);
                //recv(sockfd, buffer, sizeof(buffer), 0);
                read(sockfd,&s,sizeof(s));
            } while (s);
            printf("account created\n");
        }
        // else if (str == "create_group")
        // {
        //     int s;
        //    // char id[128];
        //     string username;
        //     char user_1[512];
        //     char id[10];
        //     //char buffer[512];
        //     char password_1 [512];

        //     char buffer[512];
        //     printf("enter groupid\n");
        //     cin.getline(id,10);
        //     printf("enter username\n");
        //     cin.getline(user_1,512);

        //     send(sockfd, "3", sizeof("3"), 0);
        //     //int n;
        //     int n;
        //     memset(buffer, '\0', sizeof(buffer));

        //     //strcpy(user_1,user.c_str());
        //     //strcpy(password_1,password.c_str());
        //     send(sockfd, &user_1, sizeof(user_1), 0);
        //     recv(sockfd, buffer, sizeof(buffer), 0);
        //     memset(buffer, '\0', sizeof(buffer));
        //     send(sockfd,password_1 , sizeof(password_1), 0);
        //     recv(sockfd, buffer, sizeof(buffer), 0);
        // }
    }
}
