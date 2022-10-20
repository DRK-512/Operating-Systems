#ifndef philosopherFunc
#define philosopherFunc

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
 
#define maxPhil 5
#define left (philNum + 4) % maxPhil
#define right (philNum + 1) % maxPhil
#define think 0
#define hung 1
#define eat 2

extern sem_t mutex;
extern sem_t philSem[maxPhil];
void* philosopher(void* num); 

#endif
