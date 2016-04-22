
// sell x tickets by y threads

#include <iostream>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;


#define SEM_THREAD 0
#define SEM_PROCESS 1
#define NUM_THREADS 6
#define MAX_VALUE 100

//global decs
sem_t sem;
int gVal=MAX_VALUE;

void *threadFunc(void *args)
{
	int tid = (int)args;	
	int iTicketsSold=0;

	while(gVal>0)
	{
		if(sem_wait(&sem)==-1)
			exit(EXIT_FAILURE);

		gVal--;
		iTicketsSold++;
		cout << "Thread" << tid << " sold one. Tickets left = " << gVal << "\n";

		if(sem_post(&sem)==-1)
			exit(EXIT_FAILURE);

		//dummy sleep
		sleep(1);
	}

	if(sem_wait(&sem)==-1)
		exit(EXIT_FAILURE);

	cout << "-----------------------\n";
	cout << "Thread" << tid << " sold " << iTicketsSold <<" tickets \n";

	if(sem_post(&sem)==-1)
		exit(EXIT_FAILURE);

	// exit the thread
	int retStatus=0;
	pthread_exit(&retStatus);
}

int main()
{
	pthread_t threads[NUM_THREADS];

	cout << "Tickets available " << MAX_VALUE <<" Start selling...\n";

	//init sem
	if(sem_init(&sem,SEM_THREAD,1)==-1)
		exit(EXIT_FAILURE);


	//init threads
	int incr;
	for(incr=0; incr<NUM_THREADS; incr++)
	{
		if(pthread_create(&threads[incr],NULL,threadFunc,(void*)incr)!=0)
			exit(EXIT_FAILURE);
	}

	//join threads	
	for(incr=0; incr<NUM_THREADS; incr++)
	{
		if(pthread_join(threads[incr],NULL)!=0)
			exit(EXIT_FAILURE);
	}

	//destroy sem
	if(sem_destroy(&sem)!=0)
		exit(EXIT_FAILURE);	

	cout << "Ending stuffs...\n";

	return 0;
}
