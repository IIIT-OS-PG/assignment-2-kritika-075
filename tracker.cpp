
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
#include <vector>
#include <algorithm>
#include <map>
using namespace std;

// int newsockfd;



vector<pair<string, string>> client_info;
map<string, vector<int>> tracker_map;
vector<pair<int, int>> group_info;


//TO DISPLAY ERROR
void error(const char *err)
{
    perror(err);
    //return 0;
    exit(1);
}



// int create_socket()
// {
   

//     return sockfd;
// }



//}client_log;
int main()
{

    //  int sock_fd=create_socket();
      int portno, n;
    //int newsockfd;
    //server socket creation
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        error("Error in opening socket");
    }

    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    memset(&serv_addr, '\0', sizeof(serv_addr));

    char buffer[512];

    portno = 9898;
    cout << portno << endl;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    //serv_addr.sin_addr.s_addr=INADDR_ANY;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    //bind the socket
    int bd = bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    cout << bd;
    if (bd < 0)
        error("Binding failed");

    //listening
    listen(sockfd, 4);
    clilen = sizeof(cli_addr);
        //cout<<sock_fd;

     while (1)
    { 
        int type;
       int  newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0)
            error("error on accept");

        read(newsockfd,&type, sizeof(type));
        cout<<"HERE AGAIN\n";
        
        if (type==1)
        {
            //---------------------------------------------login----------------------------------------------------------------------

            char user_name[512];
            char pass[512];
            char user[512];
            char pass_word[512];

            int i;
            int s;


                recv(newsockfd, user_name, sizeof(user_name), 0);
                //read(newsockfd,)user_name,sizeof(user_name));
                memset(buffer, '\0', sizeof(buffer));
                //send(newsockfd, buffer, sizeof(buffer), 0);
                recv(newsockfd, pass, sizeof(pass), 0);
                s = 0;
                for (i = 0; i < client_info.size(); i++)
                {
                    strcpy(user, (client_info[i].first).c_str());
                    if (strcmp(user, user_name) == 0)
                    {
                        //s=1;
                        //buffer[0]=1;
                        strcpy(pass_word, (client_info[i].second).c_str());
                        if (strcmp(pass_word, pass) == 0)
                        {
                            printf("user logged in");
                            int s=1;
                            write(newsockfd,&s,sizeof(s));
                            break;

                        }
                    }
                }
                if(s==0)
                write(newsockfd,&s,sizeof(s));
            

            // //copying into client file
            // client_info.push_back(make_pair(user, pass));
            // FILE *fp = fopen("client_log.txt", "ab");

            // fputs(user_name, fp);
            // fputs(" | ", fp);

            // fputs(pass, fp);
            // fputs("\n", fp);

            // fclose(fp);
            // printf("username entered\n");
        }

        else if (type==2)
        {

            char user_name[512];
            char pass[512];
            char user[512];
            int i;
            int s;

            do
            {
                recv(newsockfd, user_name, sizeof(user_name), 0);
                memset(buffer, '\0', sizeof(buffer));
                recv(newsockfd, pass, sizeof(pass), 0);
                s = 0;
                for (i = 0; i < client_info.size(); i++)
                {
                    strcpy(user, (client_info[i].first).c_str());
                    if (strcmp(user, user_name) == 0)
                    {
                        int s=1;
                        break;
                    }
                }
                write(newsockfd,&s,sizeof(s));

            } while (s);


            //copying into client file
            client_info.push_back(make_pair(user, pass));
            FILE *fp = fopen("client_log.txt", "ab");
//cout<<user_name<<" "<<pass<<endl;
            fputs(user_name, fp);
            fputs(" | ", fp);

            fputs(pass, fp);
            fputs("\n", fp);

            fclose(fp);
            printf("username entered\n");
        }
        //----------------------------------login complete------------------------------------------------------------------
    }
    return 0;
}