#ifndef schedulerFunctions
#define schedulerFunctions

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>


void *parlor(void *param); 
void *game(void *param); 

extern sem_t empty, full; 
extern pthread_mutex_t mutex;

#endif
