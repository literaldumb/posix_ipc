
//classic dining philosophers
//think,eat repeat 

#include <iostream>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

#define SEM_THREAD 0
#define SEM_PROCESS 1
#define NUM_PHILOSOPHER 5
#define EAT_TIMES 3

//thread argument structure
struct argStruct
{
	sem_t semLeftFork;
	sem_t semRightFork;
	sem_t semEat;
	sem_t semThink;
	int tid;	
};

int left_fork(int philNum) 
{
	return philNum;
}

int right_fork(int philNum) 
{
	return (philNum+1)%NUM_PHILOSOPHER;
}

void Think(void *arguments)
{
	struct argStruct *args = (struct argStruct*)arguments ;

	if(sem_wait(&args->semThink)!=0)
		exit(EXIT_FAILURE);

	cout << "Philosopher " << args->tid << " thinking...\n" ;

	if(sem_post(&args->semThink)!=0)
		exit(EXIT_FAILURE);
	sleep(1);
}

void Eat(void *arguments)
{
	struct argStruct *args = (struct argStruct*)arguments ;

	if(sem_wait(&args->semEat)!=0)
		exit(EXIT_FAILURE);
	if(sem_wait(&args->semLeftFork)!=0)
		exit(EXIT_FAILURE);
	if(sem_wait(&args->semRightFork)!=0)
		exit(EXIT_FAILURE);

	cout << "Philosopher " << args->tid << " eating.\n";
	sleep(3);

	if(sem_post(&args->semRightFork)!=0)
		exit(EXIT_FAILURE);
	if(sem_post(&args->semLeftFork)!=0)
		exit(EXIT_FAILURE);
	if(sem_post(&args->semEat)!=0)
		exit(EXIT_FAILURE);
}

void *Philosopher(void *arguments)
{
	struct argStruct *args = (struct argStruct*)arguments ;

	for(int i=0; i<EAT_TIMES; i++)
	{
		Think(arguments);
		Eat(arguments);
	}

	// exit the thread
	int retStatus=0;
	pthread_exit(&retStatus);
}

int main()
{
	//semaphores
	sem_t semPhil[NUM_PHILOSOPHER];
	sem_t eat;
	sem_t think;

	//threads
	pthread_t ptPhil[NUM_PHILOSOPHER];

	// thread argument
	struct argStruct *threadArg=NULL;

	cout << "Start operations...\n";

	//populate arg
	threadArg = (struct argStruct*)malloc(sizeof(struct argStruct)*NUM_PHILOSOPHER);
	if(threadArg==NULL)	
		exit(EXIT_FAILURE);

	//init sem
	for(int i=0; i<NUM_PHILOSOPHER; i++)
	{
		if(sem_init(&semPhil[i],SEM_THREAD,1)!=0)
			exit(EXIT_FAILURE);
	}
	if(sem_init(&eat,SEM_THREAD,NUM_PHILOSOPHER-1)!=0)
		exit(EXIT_FAILURE);
	if(sem_init(&think,SEM_THREAD,1)!=0)
		exit(EXIT_FAILURE);

	//init threads
	for(int i=0; i<NUM_PHILOSOPHER; i++)
	{
		threadArg[i].tid = i;
		threadArg[i].semLeftFork = semPhil[left_fork(i)];
		threadArg[i].semRightFork = semPhil[right_fork(i)];
		threadArg[i].semEat = eat;
		threadArg[i].semThink = think;

		if(pthread_create(&ptPhil[i],NULL,Philosopher,(void*)(&threadArg[i]))!=0)
			exit(EXIT_FAILURE);
	}

	//join threads	
	for(int i=0; i<NUM_PHILOSOPHER; i++)
	{
		if(pthread_join(ptPhil[i],NULL)!=0)
			exit(EXIT_FAILURE);
	}

	//destroy sem
	for(int i=0; i<NUM_PHILOSOPHER; i++)
	{
		if(sem_destroy(&semPhil[i])!=0)
			exit(EXIT_FAILURE);	
	}
	if(sem_destroy(&threadArg->semEat)!=0)
		exit(EXIT_FAILURE);	

	free(threadArg);

	cout << "Ending stuffs...\n";

	return 0;
}
