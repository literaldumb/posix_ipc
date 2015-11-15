#include <iostream>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#define MSG_QUEUE "/myqueue123"
#define MAX_MSG_LEN 10000

int main()
{
	mqd_t msgq_id;
	struct mq_attr msgq_attr;
	int bytes ;
	char msgBuf[MAX_MSG_LEN];
	unsigned priority;

	//open the queue
	msgq_id = mq_open(MSG_QUEUE,O_RDWR);
	if(msgq_id == (mqd_t)-1)
	{
		perror("mq_open():");
		exit(EXIT_FAILURE);
	}

	// get the attributes
	if( mq_getattr(msgq_id,&msgq_attr) < 0 )
	{
		perror("mq_getattr():");
		exit(EXIT_FAILURE);
	}

	// print the attributes
	cout << MSG_QUEUE << "\n\t" ;
	cout << "max messages: " << msgq_attr.mq_maxmsg << "\n\t" ;
	cout << "max msg len: " << msgq_attr.mq_msgsize << "\n\t" ;
	cout << "cur msg cnt: " << msgq_attr.mq_curmsgs << "\n\t" ;

	// receive a message
	bytes = mq_receive(msgq_id,msgBuf,MAX_MSG_LEN,&priority);

	if(bytes==-1)
	{
		perror("mq_receive():");
		exit(EXIT_FAILURE);
	}

	cout << "Received msg : " << msgBuf << " of length " << bytes << " with priority " << priority <<"\n";

	//close the queue
	mq_close(msgq_id);

	// all done
	return 0;
}
