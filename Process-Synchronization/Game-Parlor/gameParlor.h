#ifndef schedulerFunctions
#define schedulerFunctions

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define maxDice 8

extern sem_t mutex, setupMutex, returnMutex;
extern sem_t gameSem[maxDice];
void* game(void* param); 

#endif
