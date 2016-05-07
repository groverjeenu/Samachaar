// Assignment 1
// News Service System

// Objective
// In this assignment we develop a news service system. There is a centralized news server which maintains a library of news.
// There are three types of clients, namely reporters, who contribute news items, readers, who read news items, and administrator.


// Group Details
// Member 1: Jeenu Grover (13CS30042)
// Member 2: Ashish Sharma (13CS30043)

// File: client.cpp

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <fcntl.h>
#include <fstream>
#include <sstream>
//#define PORT_NO 5000
#define MESSAGE_LENGTH 200000
#define PACKET_SIZE 8
int PORT_NO;

using namespace std;

// string recieve_msg(int connfd,int bytes) {
//     string output;
//     output.clear();
//     output.resize(bytes);
//     int bytes_read;

//     /*int bytes_received = read(connfd, &output[0], bytes-1);
//     if (bytes_received<0) {
//         std::cerr << "Failed to read data from socket.\n";
//         return NULL;
//     }*/

//     int counter = 0;
//     while( (bytes_read =  read(connfd,&output[counter],PACKET_SIZE))>0 )
//         counter += bytes_read;

//     output[output.length()] = '\0';


//     //output.resize(bytes_received);
//     char* cstr = new char[output.length() +1];
//     strcpy(cstr, output.c_str());
//     string msg(cstr);
//     cout<<"Message Received at Server:\n"<<msg<<endl;
//     return msg;
// }
// void send_msg(int connfd,string dataToSend){


//     /*uint32_t dataLength = htonl(dataToSend.size()); // Ensure network byte order
//                                                 // when sending the data length
//     send(connfd,&dataLength ,/sizeof(uint32_t) ,MSG_CONFIRM); // Send the data length
//     cout<<"Sending strft"<<endl;
//     send(connfd,dataToSend.c_str(),dataToSend.size(),MSG_CONFIRM); // Send the string*/

//     int counter = 0, byte_sent;
//     cout<<dataToSend.length();

//     while(counter < dataToSend.length())
//     {
//             byte_sent = send(connfd,&dataToSend[counter],PACKET_SIZE,MSG_CONFIRM) ;
//         counter += byte_sent;
//         cout<<"c ids : "<<counter<<endl;
//     }
//     cout<<"Sending done"<<endl;
// }


string recieve_msg(int connfd,int bytes,int flag = 0) {
    string output;
    output.clear();
    output.resize(bytes);

    int bytes_received = read(connfd, &output[0], bytes-1);
    if (bytes_received<0) {
        std::cerr << "Failed to read data from socket.\n";
        return NULL;
    }

    output.resize(bytes_received);
    char* cstr = new char[output.length() +1];
    strcpy(cstr, output.c_str());
    string msg(cstr);
    if(flag == 0)
    cout<<"Message Received:\n"<<msg<<endl;
    else
        cout<<"File Downloaded Succesfully\n";
    return msg;
}

void send_msg(int connfd,string dataToSend){

    uint32_t dataLength = htonl(dataToSend.size()); // Ensure network byte order
                                                // when sending the data length

    //send(connfd,&dataLength ,sizeof(uint32_t) ,MSG_CONFIRM); // Send the data length
    send(connfd,dataToSend.c_str(),dataToSend.size(),MSG_CONFIRM); // Send the string
}

int main(int argc, char * argv[])
{

    int recfd = socket(AF_INET,SOCK_STREAM,0),n;
    PORT_NO = atoi(argv[2]);
    if(recfd < 0)printf("Socket formation Failed\n");
    string sendmsg;
    string recmsg,msg;

    struct sockaddr_in saddr;
    printf("Tryint to Connect ...\n");

    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(PORT_NO);

    if(inet_pton(AF_INET,argv[1],&saddr.sin_addr) <= 0)printf("Addr conv Failed\n");

    if(connect(recfd,(struct  sockaddr*)&saddr,sizeof(saddr)) < 0)printf("Connection Failed\n");
    printf("Connection established ...\n");

    //recieve_msg(recfd,MESSAGE_LENGTH);


    //recieve_msg(recfd, MESSAGE_LENGTH);

    //if(sendmsg == "1" || sendmsg == "")
    //cout<<recmsg<<endl;

    //recmsg = string(recieve_msg(recfd, MESSAGE_LENG
    //cout<<"jeenu\n";
    /*while ( (n = read(recfd, recmsg, sizeof(recmsg)-1)) > 0)
        {
            printf("I am in\n");
            recmsg[n] = 0;
            if(fputs(recmsg, stdout) == EOF)
            {
                printf("\n Error : Fputs error\n");
            }
        }*/

    int connfd = recfd;
    //int flags = fcntl(connfd, F_GETFL, 0);
    //fcntl(connfd, F_SETFL, flags | O_NONBLOCK);
    while(1)
    {
        //msg.clear();
        //msg = "\n\nHi Reader!!\nSend 1 for Academic News or 2 for Non-Academic News or # to exit\n";
        //cout<<"Start Again"<<endl;
        recieve_msg(connfd, MESSAGE_LENGTH);
        //cout<<"fdhgf"<<endl;
        cin>>sendmsg;
        //cout<<"Message Read"<<endl;
        send_msg(recfd,sendmsg);
        //cout<<"Message Sent"<<endl;

        if(sendmsg == "1")
        {
            recieve_msg(connfd,MESSAGE_LENGTH);
            cin>>sendmsg;
            send_msg(recfd,sendmsg);


            msg = recieve_msg(recfd,MESSAGE_LENGTH,1);
            ofstream file_reader;
            string line = msg;
            int ii = 0;

            while(line[ii]!='\n'){
                ii++;
            }

            ii++;

            string headline;

            while(line[ii]!='\n'){
                if(line[ii] != ' '){
                headline += line[ii];

            }
            ii++;
            }
            headline = headline.substr(8);
            string filename = string(getenv("HOME"))+"/"+headline+".txt";
            printf("%s\n",filename.c_str());



            file_reader.open(filename.c_str(),ios::out|ios::trunc);
            file_reader<<msg<<endl;


            int npid = fork();
            if(npid == 0)
            {
                //system(("xterm -hold -e more "+(filename)).c_str());
                execlp("xterm" ,"xterm" , "-hold","-e","more",(filename).c_str(),(const char *)NULL);

                exit(1);
            }


        }
        else if (sendmsg == "2")
        {
            recieve_msg(connfd,MESSAGE_LENGTH);
            cin>>sendmsg;
            send_msg(recfd,sendmsg);


            msg = recieve_msg(recfd,MESSAGE_LENGTH,1);
            ofstream file_reader;
            string line = msg;
            int ii = 0;

            while(line[ii]!='\n'){
                ii++;
            }

            ii++;

            string headline;

            while(line[ii]!='\n'){
                if(line[ii] != ' '){
                headline += line[ii];

            }
            ii++;
            }
            headline = headline.substr(8);
            string filename = string(getenv("HOME"))+"/"+headline+".txt";
            printf("%s\n",filename.c_str());



            file_reader.open(filename.c_str(),ios::out|ios::trunc);
            file_reader<<msg<<endl;


            int npid = fork();
            if(npid == 0)
            {
                execlp("xterm" ,"xterm" , "-hold","-e","more",(filename).c_str(),(const char *)NULL);
                exit(1);
            }
        }
        else if(sendmsg == "#")
            {
                recieve_msg(connfd,MESSAGE_LENGTH);
                //send_msg(connfd,"Reading Portal closed Succesfully\n");
                break;

            }
            send_msg(connfd,"Done");
        //send_msg(connfd,"Command Not recognized\n");

        msg.clear();
        //msg = "\n\nHi Reader!!\nSend 1 for Academic News or 2 for Non-Academic News\n";
        //send_msg(connfd,msg);

        //scanf("%d",&n);
    }

    return 0;


}
