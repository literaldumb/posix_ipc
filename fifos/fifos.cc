#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

int main()
{

	// make a fifo using myfifo /tmp/myfifo
	// execute echo "**strings**" > /tmp/myfifo type commands
	// read them over here
	
	pid_t pid;
	int iFIFOfd;
	string strMsg;

	char ch[1024];

	while(1)
	{
		iFIFOfd = open("/tmp/myfifo",O_RDONLY);
		read(iFIFOfd,&ch,1024);
		cout << ch ;
		close(iFIFOfd);
	}

	// all done
	return 0;
}

