#ifndef schedulerFunctions
#define schedulerFunctions

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>


void *parlor(); 
void *game(void *param); 

extern sem_t canPlay, gameDone, addQue; 
extern char count, firstGame; 

#endif
