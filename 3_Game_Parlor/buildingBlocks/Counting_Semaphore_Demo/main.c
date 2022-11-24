#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

/*
  The purpose of this program is show how to run a number of threads with a counting semaphore
  NOTES: With gcc please run with -lpthread
  		 This is building block 1/3 for the game parlor project
  		 What this program does is that it creates 8 threads, and prints that threads message onto the terminal
*/

#define NUM_THREADS	8
char *messages[NUM_THREADS];

void *PrintHello(void *threadid) {

	long taskid = (long) threadid;

	// add a sleep so there is some time for the process to run 
	sleep(1);

	printf("Thread %ld: %s\n", taskid, messages[taskid]);
	pthread_exit(NULL);
}

int main() {

	pthread_t threads[NUM_THREADS];
	long i; 
	int pError; 
	
	messages[0] = "English: Hello World!";
	messages[1] = "Polish: Witaj Świecie!";
	messages[2] = "Italian: Ciao Mondo!";
	messages[3] = "Ukrainian: Привіт Світ!";
	messages[4] = "German: Hallo Welt!"; 
	messages[5] = "French: Bonjour le Monde!";
	messages[6] = "Japanese:「こんにちは世界」(Kon'nichiwa Sekai!)"; 
	messages[7] = "Latin: Salve Mundus!";

	for(i=0;i<NUM_THREADS;i++) {
	
		// if pthread does not return 0, we have problems
		pError = pthread_create(&threads[i], NULL, PrintHello, (void *) i);
		if (pError) {
			printf("ERROR; return code from pthread_create() is %d\n", pError);
			exit(-1);
		}
	}

	pthread_exit(NULL);
}
