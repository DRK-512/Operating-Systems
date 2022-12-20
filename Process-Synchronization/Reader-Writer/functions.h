#ifndef functions
#define functions

#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern sem_t writting;
extern pthread_mutex_t studentMutex, teacherMutex;

void *teacherThread(void *param); 
void *studentThread(void *param); 

#endif