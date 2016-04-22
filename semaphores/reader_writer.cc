
//writer writers, reader reads
// writer blocks when buffer is full
// reader blocks when buffer is empty 

#include <iostream>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

using namespace std;

#define SEM_THREAD 0
#define SEM_PROCESS 1
#define BUFFER_SIZE 5
#define DATA_LENGTH 15

//thread argument structure
struct argStruct
{
	char buff[BUFFER_SIZE];
	sem_t empty;
	sem_t full;	
};

char getData()
{
	srand(time(NULL));
	int num = rand();	

	num = num % 26 ; //letter of alphabet
	num = num + 65 ; // ASCII of 'A'

	return num;
}

void *Reader(void *arguments)
{
	struct argStruct *args = (struct argStruct*)arguments ;
	int iReadPtr=0;
	char ch;

	for(int i=0; i<DATA_LENGTH; i++)
	{
		if(sem_wait(&args->empty)!=0)
			exit(EXIT_FAILURE);

		//read from buffer
		ch = args->buff[iReadPtr];
		iReadPtr = (iReadPtr+1)%BUFFER_SIZE;
		cout << "Reader reads : " << ch << "\n";

		if(sem_post(&args->full)!=0)
			exit(EXIT_FAILURE); 

		//dummy sleep
		sleep(2);
	}
	
	// exit the thread
	int retStatus=0;
	pthread_exit(&retStatus);
}

void *Writer(void *arguments)
{
	struct argStruct *args = (struct argStruct*)arguments ;
	int iWritePtr=0;
	char ch;
	
	for(int i=0;i<DATA_LENGTH;i++)
	{
		ch = getData();
		
		if(sem_wait(&args->full)!=0)
			exit(EXIT_FAILURE);

		//write to buffer
		args->buff[iWritePtr]=ch;
		iWritePtr = (iWritePtr+1)%BUFFER_SIZE;
		cout << "Writer writes : " << ch << "\n";

		if(sem_post(&args->empty)!=0)
			exit(EXIT_FAILURE);

		//dummy sleep
		sleep(1);
	}

	// exit the thread
	int retStatus=0;
	pthread_exit(&retStatus);
}

int main()
{
	//threads
	pthread_t ptReaderThread;
	pthread_t ptWriterThread;

	// thread argument
	struct argStruct *threadArg=NULL;

	cout << "Start operations...\n";

	//populate arg
	threadArg = (struct argStruct*)malloc(sizeof(struct argStruct));
	if(threadArg==NULL)	
		exit(EXIT_FAILURE);

	//init sem
	if(sem_init(&threadArg->full,SEM_THREAD,BUFFER_SIZE)!=0)
		exit(EXIT_FAILURE);
	if(sem_init(&threadArg->empty,SEM_THREAD,0)!=0)
		exit(EXIT_FAILURE);

	//init threads
	if(pthread_create(&ptWriterThread,NULL,Writer,(void*)threadArg)!=0)
		exit(EXIT_FAILURE);
	if(pthread_create(&ptReaderThread,NULL,Reader,(void*)threadArg)!=0)
		exit(EXIT_FAILURE);

	//join threads	
	if(pthread_join(ptWriterThread,NULL)!=0)
		exit(EXIT_FAILURE);
	if(pthread_join(ptReaderThread,NULL)!=0)
		exit(EXIT_FAILURE);

	//destroy sem
	if(sem_destroy(&threadArg->full)!=0)
		exit(EXIT_FAILURE);	
	if(sem_destroy(&threadArg->empty)!=0)
		exit(EXIT_FAILURE);	

	free(threadArg);

	cout << "Ending stuffs...\n";

	return 0;
}
