#ifndef functions
#define functions

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

void *Producer(void* param); 
void *Consumer(void* param);

extern sem_t empty, full, mutex; 

#endif