#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

using namespace std;

#define BUFFER_LENGTH 1024

int main(int argc,char *argv[])
{
	int socketfd,portno,bytes;
	char buffer[BUFFER_LENGTH];
	string strBuffer;
	struct sockaddr_in serverAddress;
	struct hostent *server=NULL;

	// get the socket fd from socket()
	socketfd = socket(AF_INET,SOCK_STREAM,0);
	if(socketfd<0)
	{
		perror("socket():");
		exit(EXIT_FAILURE);
	}

	// populate the hostent object
	server = gethostbyname(argv[1]);
	if(server==NULL)
	{
		perror("gethostbyname():");
		exit(EXIT_FAILURE);
	}

	// populate the sockaddr_in object
	portno = atoi(argv[2]);
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(portno);

	// set the header address for hostent object
	memcpy( (char*)server->h_addr,(char*)&serverAddress.sin_addr.s_addr,server->h_length );

	// connect to the server
	if(connect(socketfd,(struct sockaddr*)&serverAddress,sizeof(serverAddress)) < 0 )
	{
		perror("connect():");
		exit(EXIT_FAILURE);
	}

	while(1) {
		cout << "\nEnter a message[quit to quit]: " ;
		getline(cin,strBuffer);

		// write the message
		bytes = write(socketfd,strBuffer.c_str(),strBuffer.length());

		if(bytes<0)
		{
			perror("write():");
			exit(EXIT_FAILURE);
		}
		
		// read the ack from server
		memset(buffer,0,BUFFER_LENGTH);
		bytes = read(socketfd,buffer,BUFFER_LENGTH);

		cout << "ACK from server: " << buffer << "\n" ;

		if(!strcmp(buffer,"quit"))
			break;
	
	}

	// all done
	return 0;
}
