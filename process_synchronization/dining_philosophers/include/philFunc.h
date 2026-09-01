#ifndef philFunc
#define philFunc

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
 
#define maxPhil 5      // max number of philosophers
#define left (philNum + (maxPhil-1)) % maxPhil
#define right (philNum + 1) % maxPhil
#define EAT_TIME 1     // Time it takes to eat
#define THINK_TIME 1   // Time it takes to think 
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

