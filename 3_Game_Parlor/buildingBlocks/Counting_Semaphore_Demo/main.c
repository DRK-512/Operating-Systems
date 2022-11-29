#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

/*
	The purpose of this program is show how to run a number of threads with a counting semaphore
	NOTES: 	With gcc please run with -lpthread
			This is building block 1/3 for the game parlor project
			What this program does is that it creates 8 threads, and prints that threads message onto the terminal
*/

#define numT 8
#define threadLimit 8
char *messages[numT];
void *printMessage(); 
sem_t sem;
sem_t semMutex;  

int main() {

	pthread_t threads[numT]; // threads we use for testing
	long i; // made long because pthread_create(x,x,x,VALUE) is expected to be a long
	int pError; // pthreads return value, and the return states if an error as occured 
	
	sem_init(&sem, 1, threadLimit); 
	sem_init(&semMutex, 0, 1); 

	messages[0] = "English: Hello World!";
	messages[1] = "Polish: Witaj Świecie!";
	messages[2] = "Italian: Ciao Mondo!";
	messages[3] = "Ukrainian: Привіт Світ!";
	messages[4] = "German: Hallo Welt!"; 
	messages[5] = "French: Bonjour le Monde!";
	messages[6] = "Japanese:「こんにちは世界」(Kon'nichiwa Sekai!)"; 
	messages[7] = "Latin: Salve Mundus!";

	for(i=0;i<numT;i++) {
		// if pthread does not return 0, we have problems
		pError = pthread_create(&threads[i], NULL, printMessage, (void *) i);
		if (pError) {
			printf("ERROR; return code from pthread_create() is %d\n", pError);
			exit(-1);
		}
	}
	pthread_exit(NULL);
}

void *printMessage(void *threadid) {
	// where value here is the consumer, and the producer will be in the gameparlor 
	long taskid = (long) threadid;
	int needed, value=0; 
	//
	// Once we have a semaphore, we "run the process" aka make it sleep 
	
	//sem_getvalue(&sema, &value);
	//printf("Value: %d\n", value);
	switch (taskid)
	{
	case 0:
	case 1: 
		needed=2; 
		break;
	case 2:
	case 3: 
		needed=4; 
		break;
	case 4:
	case 5: 
		needed=5; 
		break;
	case 6:
	case 7:
		needed=3;  
		break;
	default:
		needed=0;
	} 

	// this is our critical section, we seem to be experienceing the critical section problem as well as the producer / consumer one
	while(value<needed){
		sem_wait(&semMutex);
		sem_wait(&sem);
		sem_getvalue(&sem, &value);
		printf("Value: %d\n", value);
		sem_post(&semMutex); 
	}
	 
	printf("Thread %ld: %s\n", taskid, messages[taskid]);
	sleep(1); 
	
	// Once done with the process (or sleep) we free the semaphore, and free the thread
	sem_post(&sem); 
	//sem_post(&semMutex); 
	pthread_exit(NULL);
	
}
