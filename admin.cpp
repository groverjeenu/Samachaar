// Assignment 1
// News Service System

// Objective
// In this assignment we develop a news service system. There is a centralized news server which maintains a library of news.
// There are three types of clients, namely reporters, who contribute news items, readers, who read news items, and administrator.


// Group Details
// Member 1: Jeenu Grover (13CS30042)
// Member 2: Ashish Sharma (13CS30043)

// File: admin.cpp

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
int PORT_NO;
#define MESSAGE_LENGTH 2000

using namespace std;
struct sockaddr_in si_other;



string recieve_msg(int connfd,int bytes) {


    char buf[MESSAGE_LENGTH];
    socklen_t s_len = sizeof(si_other);

    int bytes_received = recvfrom(connfd, buf, MESSAGE_LENGTH, 0, (struct sockaddr *)&si_other, &s_len);
    if (bytes_received<0) {
        std::cerr << "Failed to read data from socket.\n";
        return "";
    }

    string output(buf);
    output.resize(bytes_received);
    char* cstr = new char[output.length() +1];
    strcpy(cstr, output.c_str());
    string msg(cstr);
    cout<<"Message Received:\n"<<msg<<endl;
    return msg;
}

void send_msg(int connfd,string dataToSend){

    if (sendto(connfd, dataToSend.c_str(), dataToSend.length(), 0, (struct sockaddr*) &si_other, sizeof(si_other)) == -1)
        {
            //die("sendto()");
            cout<<"send Fail"<<endl;
        }
}


int main(int argc, char * argv[])
{
    PORT_NO = atoi(argv[2]);
    int adminFD = socket(AF_INET,SOCK_DGRAM,0),n;
    if(adminFD < 0)printf("Socket formation Failed\n");
    string sendmsg;
    string recmsg;

    /*struct sockaddr_in saddr;
    printf("Tryint to Connect ...\n");

    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(PORT_NO);*/

    //if(inet_pton(AF_INET,argv[1],&saddr.sin_addr) <= 0)printf("Addr conv Failed\n");

    //if(connect(adminFD,(struct  sockaddr*)&saddr,sizeof(saddr)) < 0)printf("Connection Failed\n");


    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT_NO);

    if (inet_aton(argv[1] , &si_other.sin_addr) == 0)
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }


    printf("Connection established ...\n");
    send_msg(adminFD,"Client Details Sent\n");
    recmsg.clear();
    recmsg = recieve_msg(adminFD,MESSAGE_LENGTH);


    while(recmsg!="\nEnter Password: "){
        sendmsg.clear();
        cin>>sendmsg;
        send_msg(adminFD,sendmsg);
        recmsg.clear();
        recmsg = recieve_msg(adminFD,MESSAGE_LENGTH);
    }

    while(recmsg!="\nSuccessfully Logged In\nSend 1 for Academic Group, 2 for Non-Academic Group, # to Logout\n"){
        sendmsg.clear();
        cin>>sendmsg;
        send_msg(adminFD,sendmsg);

        recmsg.clear();
        recmsg = recieve_msg(adminFD,MESSAGE_LENGTH);
    }

    int flag = 0;

    while(1){

        if(flag == 1){
            recmsg.clear();
            recmsg = recieve_msg(adminFD, MESSAGE_LENGTH);
        }

        sendmsg.clear();
        cin>>sendmsg;
        send_msg(adminFD,sendmsg);
        cout<<sendmsg<<endl;
        if(sendmsg == "#") break;
        if(sendmsg == "1" || sendmsg == "2"){

            recmsg.clear();
            recmsg = recieve_msg(adminFD, MESSAGE_LENGTH);

            sendmsg.clear();
            cin>>sendmsg;
            send_msg(adminFD,sendmsg);

            recmsg.clear();
            recmsg = recieve_msg(adminFD, MESSAGE_LENGTH);
            cout<<"Recieve done"<<endl;
            sendmsg.clear();
            sendmsg = "Done";
            send_msg(adminFD,sendmsg);
            cout<<"Send Done"<<endl;
        }
        flag = 1;
    }
    return 0;
}
