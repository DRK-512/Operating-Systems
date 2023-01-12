#ifndef functions
#define functions

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

extern sem_t elavatorMutex, req_sem, modBuffer; 

#define NUM_PEOPLE 5
#define NUM_ELEVATORS 2
#define NUM_FLOORS 5

void *elavator(void *param); 
void *people(void *param); 

#endif