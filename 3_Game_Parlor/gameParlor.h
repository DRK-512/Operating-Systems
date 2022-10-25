#ifndef schedulerFunctions
#define schedulerFunctions

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

// semaphores for each game
extern sem_t Pictionary, Monopoly, Backgammon, Risk; 

#endif
