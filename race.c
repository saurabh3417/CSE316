#include<cstdio.h>		//for perror and printf
#include<pthread.h>
#include<stdlib.h>		//for exit() & rand()
#include<unistd.h>		//for sleep()
#include<fcntl.h>
#include<sys/types.h>
#include<semaphore.h>	//for mutex

#define MAX_RESOURCES 5		//After the program get executed, it must report 5 available resources. If resources are less or more than the code is not exact solution.

/*
Semaphores should be declared globally so all threads can share them.
Semaphore is declared as:
	Semaphore s;
Initialized as
	semaphore_init(&s, value);	//where value initialize the semaphore to 1 for a mutex semaphore
The 2 operations provided are-
	semaphore_down( &s );	//wait() and decrements the semaphore
	semaphore_up( &s );		//signal() and increments the semaphore
*/

int avail_resource = MAX_RESOURCES;

int deCount(int count)
{
	if(avail_resource < count)		//complexity = n
	{        
		return -1;
	}
	else
	{
		avail_resource = avail_resource - count;
		return 0;
	}
}

int inCount(int count)
{
	avail_resource = avail_resource + count;
	return 0;
}

	/*implement the increment and decrement function using binary semaphore*/
	/*avoid race condition on avail_resources*/
void decrement(int count)
{}

void increment(int count)
{}

//A thread() function is created that will be called by threads
void * thread(void * id)
{
	int t_id = *((int*) id);
	int value = ((int) rand() % 3) + 1;	//random no. of thread
/*
Race conditions is involved in deCount() and inCount() functions
Binary semaphore (mutex lock) is used to modify deCount and inCount to prevent the race conditon
Thread instance created by thread() needs to request resources and continue, causes wait
So a counting semaphore is used to avoid wait
*/
	while(deCount(value) == -1 );
	printf("Thread %d starting instances is %d.\n", t_id, value);		//Use the application instance
	sleep(rand() % 2);						//Release the resource
	inCount( value );
	decrement(value);
	printf("Thread %d starting instances is %d.\n", t_id, value);		//Use the thread for a while
	sleep(rand() % 2);						//Release the resource
	increment( value );
}
int main(int argI, char** argC)
{
	pthread_t threads[10];					//Declaring threads
	int ids[10];						//seed the random number generator
	srand((unsigned)time(0));
/*
initialise the semaphore for the binary semaphore
initialise the semaphore for the counting semaphore
*/
	for(int n = 0; n < 10; n++)		//creating thread
	{
		ids[n] = n;			//assigning the thread id
		if (pthread_create(&threads[n], NULL, thread, ids[n]))
		{
			perror("error creating thread");
			exit(1);
		}
	}				//wait for threads to finish
	for(int n = 0; n < 10; n++)
	{
		pthread_join(threads[n], 0);
	}
	printf("Available Resource: %d\n", avail_resource);
}
