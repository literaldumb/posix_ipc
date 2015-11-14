#include <iostream> // cout 
#include <unistd.h> // pipe()
#include <stdio.h> // perror
#include <string.h> // strcmp()
#include <stdlib.h> // exit()

using namespace std ; // do away with std::**

#define MSG "Hello Pipes"
#define READ 0
#define WRITE 1

int main()
{
	int fd[2];
	pid_t pid;
	int iStatus ;
	string strMsg(MSG) ;
	unsigned int uiMsgLength = strMsg.length();

	// create a pipe
	if( pipe(fd) < 0 )
	{
		perror("pipe():");
		exit(EXIT_FAILURE);
	}

	// fork a child
	pid = fork();
	if(pid<0)
	{
		perror("fork():");
		exit(EXIT_FAILURE);
	}
	else if(pid>0)
	{
		//parent
		//close the read end
		close(fd[READ]);

		// write msg to the write end
		cout << "msg sent to child: " << strMsg << endl ;
		write(fd[WRITE],strMsg.c_str(),uiMsgLength);
	}
	else if(pid==0)
	{
		//child

		//close the write end
		close(fd[WRITE]);

		// read msg from the read end
		char clocalBuf[uiMsgLength];
		read(fd[READ],clocalBuf,uiMsgLength);

		// print and validate the msg received
		cout << "msg received from parent: " << clocalBuf << "\n";
		if(!strcmp(clocalBuf,strMsg.c_str()))
		{
			cout << "\nSuccessfully received...\n";
		}
		else
		{
			cout << "\nErroneously received...\n";
		}

	}

	// all done
	return 0;
}

