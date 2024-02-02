#ifndef philosopherFunc
#define philosopherFunc

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
 
#define maxPhil 5
#define left (philNum + 4) % maxPhil
#define right (philNum + 1) % maxPhil
#define EAT_TIME 1
#define THINK_TIME 1
#define allowedBites 3 // max amount of times philosophers are allowed to eat

enum philState {
	think=0, 
	hung=1, 
	eat=2
}; 

extern sem_t mutex;
extern sem_t philSem[maxPhil];

void* philosopher(void* num); 

#endif
