#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

#define MSG_QUEUE_NAME "/myqueue2"
#define MAX_MSG 10
#define MAX_MSG_SIZE 10
#define CUR_MSGS 10

int main()
{
	mqd_t msgq_id;
	struct mq_attr pushed_attr, received_attr;

	//populate the attributes
	pushed_attr.mq_flags = 0;
	pushed_attr.mq_maxmsg = MAX_MSG;
	pushed_attr.mq_msgsize = MAX_MSG_SIZE;
	pushed_attr.mq_curmsgs = CUR_MSGS;

	// create a queue
	msgq_id = mq_open(MSG_QUEUE_NAME , O_RDWR | O_CREAT | O_EXCL , S_IRWXU | S_IRWXG , &pushed_attr);
	if(msgq_id==(mqd_t)-1)
	{
		perror("mq_open():");
		exit(EXIT_FAILURE);
	}

	// queue created, now fetch them
	if( mq_getattr(msgq_id,&received_attr) < 0 )
	{
		perror("mq_getattr():");
		exit(EXIT_FAILURE);
	}

	// print them
	cout << MSG_QUEUE_NAME << "\n\t" ;	
	cout << "flags: " << received_attr.mq_flags << "\n\t" ;
	cout << "maxmsg: " << received_attr.mq_maxmsg << "\n\t" ;
	cout << "msgsize: " << received_attr.mq_msgsize << "\n\t" ;
	cout << "curmsgs: " << received_attr.mq_curmsgs << "\n\t" ;

	//re-populate the attributes
	pushed_attr.mq_flags = O_NONBLOCK;
	pushed_attr.mq_maxmsg = MAX_MSG+10;
	pushed_attr.mq_msgsize = MAX_MSG_SIZE+10;
	pushed_attr.mq_curmsgs = CUR_MSGS+10;

	// set the new attributes
	if( mq_setattr(msgq_id,&pushed_attr,NULL) < 0 )
	{
		perror("mq_setattr():");
		exit(EXIT_FAILURE);
	}

	// get the new attributes
	if( mq_getattr(msgq_id,&received_attr) < 0 )
	{
		perror("mq_getattr():");
		exit(EXIT_FAILURE);
	}
	
	// print them
	cout << MSG_QUEUE_NAME << "\n\t" ;	
	cout << "flags: " << received_attr.mq_flags << "\n\t" ;
	cout << "maxmsg: " << received_attr.mq_maxmsg << "\n\t" ;
	cout << "msgsize: " << received_attr.mq_msgsize << "\n\t" ;
	cout << "curmsgs: " << received_attr.mq_curmsgs << "\n\t" ;
	
	// close the msg queue
	mq_close(msgq_id);

	// all done
	return 0;
}
