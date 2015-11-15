/*
 * Run the first time with -q, to create the queue
 * Subsequent runs should be run without the -q option
 * Regardless of the run number, execute with -p for priority
*/

#include <iostream>
#include <mqueue.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

using namespace std;

#define MSG_QUEUE "/myqueue123"
#define MAX_MSG_LEN 1024

int main(int argc, char *argv[])
{
	mqd_t msgq_id;
	pid_t pid = getpid();
	bool createQueue=false;
	char msgBuffer[MAX_MSG_LEN];
	char ch;
	time_t currentTime;
	int priority;
	string strMsg;

	// get the options from command line
	while( (ch = getopt(argc,argv,"qp:")) != -1 )
	{
		switch(ch)
		{
			case 'q':
				createQueue=true;
				break;
			case 'p':
				priority=atoi(optarg);
				break;
		}
	}

	// create or open the message queue
	if(createQueue)
	{
		msgq_id = mq_open(MSG_QUEUE,O_RDWR | O_CREAT | O_EXCL, S_IRWXU | S_IRWXG, NULL);
	}
	else
	{
		msgq_id = mq_open(MSG_QUEUE,O_RDWR);
	}

	if(msgq_id == (mqd_t)-1)
	{
		perror("mq_open():");
		exit(EXIT_FAILURE);
	}

	// construct a message
	currentTime = time(NULL);
	char pbuf[10];
	sprintf(pbuf,"%d",pid);
	strMsg = "Process " + string(pbuf) + " at " + string(ctime(&currentTime)) ;

	// push the message on the queue
	if(mq_send(msgq_id,strMsg.c_str(),strMsg.length(),priority) < 0)
	{
		perror("mq_send():");
		exit(EXIT_FAILURE);
	}

	//close the queue
	mq_close(msgq_id);

	// all done
	return 0;
}
