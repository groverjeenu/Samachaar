// Assignment 1
// News Service System

// Objective
// In this assignment we develop a news service system. There is a centralized news server which maintains a library of news.
// There are three types of clients, namely reporters, who contribute news items, readers, who read news items, and administrator.


// Group Details
// Member 1: Jeenu Grover (13CS30042)
// Member 2: Ashish Sharma (13CS30043)

// File: reporter.cpp


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

string recieve_msg(int connfd,int bytes) {
    string output;
    output.clear();
    output.resize(bytes);

    int bytes_received = read(connfd, &output[0], bytes-1);
    if (bytes_received<0) {
        std::cerr << "Failed to read data from socket.\n";
        return "";
    }

    output.resize(bytes_received);
    char* cstr = new char[output.length() +1];
    strcpy(cstr, output.c_str());
    string msg(cstr);
    cout<<"Message Received: \n"<<msg<<endl;
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
	PORT_NO = atoi(argv[2]);
    int reporterFD = socket(AF_INET,SOCK_STREAM,0),n;
    if(reporterFD < 0)printf("Socket formation Failed\n");
    string sendmsg;
    string recmsg;

    struct sockaddr_in saddr;
    printf("Tryint to Connect ...\n");

    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(PORT_NO);

    if(inet_pton(AF_INET,argv[1],&saddr.sin_addr) <= 0)printf("Addr conv Failed\n");

    if(connect(reporterFD,(struct  sockaddr*)&saddr,sizeof(saddr)) < 0)printf("Connection Failed\n");
    printf("Connection established ...\n");

    while(1){

	    recmsg.clear();
	    recmsg = recieve_msg(reporterFD,MESSAGE_LENGTH);

	    sendmsg.clear();
	    cin>>sendmsg;
	    send_msg(reporterFD,sendmsg);
	    if(sendmsg == "#") break;
	    if(sendmsg == "1" || sendmsg == "2"){


	    	recmsg.clear();
		    recmsg = recieve_msg(reporterFD, MESSAGE_LENGTH);

		    sendmsg.clear();
		    cin>>sendmsg;
		    send_msg(reporterFD,sendmsg);

		    recmsg.clear();
		    recmsg = recieve_msg(reporterFD, MESSAGE_LENGTH);

		    sendmsg.clear();
		    string text;
	//	    cin.ignore();
		    char c;
		    do
		    {
		    	text.clear();
		    	cin>>text;
		    	sendmsg += text;
		    	c = getchar();
		    	if(c!='\n')
		    	sendmsg += c;
		    }while(c!='\n');

		    send_msg(reporterFD,sendmsg);

		    recmsg.clear();
		    recmsg = string(recieve_msg(reporterFD, MESSAGE_LENGTH));

		    sendmsg.clear();

		    do
		    {
		    	text.clear();
		    	cin>>text;
		    	sendmsg += text;
		    	c = getchar();
		    	sendmsg += c;
		    }while(text!="-1");

		    send_msg(reporterFD,sendmsg);
		}
		else{
			recmsg.clear();
		    recmsg = recieve_msg(reporterFD, MESSAGE_LENGTH);
		    sendmsg = "Error\n";
		    send_msg(reporterFD,sendmsg);
		}
	}
    return 0;
}
