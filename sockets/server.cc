#include <iostream>
#include <stdio.h> //perror
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

#define BUFFER_LENGTH 1024

int main(int argc, char *argv[])
{
	int socketfd, newsocketfd, portno,bytes ;
	struct sockaddr_in serverAddress, fromAddress;
	socklen_t fromAddrLen;
	char buffer[BUFFER_LENGTH];

	// create a socket socket(domain,type,protocol)
	// domain - AF_INET(internet)/AF_UNIX(unix)
	// type - continuous stream(SOCK_STREAM)/datagram(SOCK_DGRM)
	// protocol - if zero, the system chooses the most appropriate protocol according to domain and type passed.
	
	socketfd = socket(AF_INET, SOCK_STREAM,0);
	if(socketfd<0)
	{
		perror("socket():");
		exit(EXIT_FAILURE);
	}

	// fill the attributes to sockaddr_in object
	// sin_family - address family, use AF_INET
	// sin_port - convert the port no to network byte order with htons()
	// sin_addr - IP address of the server on which it is running, INADDR_ANY will do that
	// sin_zero - always zero fill the field
	portno = atoi(argv[1]);	
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(portno);
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	memset(&serverAddress.sin_zero,0,sizeof(serverAddress.sin_zero));

	// bind the socket to the sockaddr_in object
	if( bind(socketfd,(struct sockaddr*)&serverAddress,sizeof(serverAddress)) < 0 )
	{
		perror("bind():");
		exit(EXIT_FAILURE);
	}

	// listen on the socket for connections, with a backlog queue of at most 5 connections
	listen(socketfd,5);

	// accept a connection. Block until a connection is received.
	// Use the new socket fd thus received for read and write
	fromAddrLen = sizeof(fromAddress);
	newsocketfd = accept(socketfd,(struct sockaddr*)&fromAddress,&fromAddrLen);
	if(newsocketfd<0)
	{
		perror("accept():");
		exit(EXIT_FAILURE);
	}

	while(1) {
		// read from the socket
		memset(buffer,0,BUFFER_LENGTH);
		bytes = read(newsocketfd,buffer,BUFFER_LENGTH);

		if(bytes>0)
		{
			cout << "\nReceived " << bytes << " bytes from " << inet_ntoa(fromAddress.sin_addr) << " on port " << ntohs(fromAddress.sin_port) << "\n";
			cout << "Message: " << buffer << "\n";
		}
		
		// send the ack
		string strMsg(buffer);
		bytes = write(newsocketfd,strMsg.c_str(),strMsg.length());
		
		// manipulate
		if ( !strMsg.compare("quit") )
			break;

	}

	// close the socket fd's
	close(newsocketfd);
	close(socketfd);

	// all done
	return 0;
}
