#ifndef gameStats
#define gameStats

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define START_GAME 1
#define FINISH_GAME 2
#define maxDice 8

sem_t mutex, setupMutex, returnMutex;

struct gameStruct {
    char* name; 
    char numDice; 
    char round; 
}; 

typedef struct s_mainGame{
    char* id; 
    char* round; 
} mainGame; 

struct gameStruct games[maxDice]; 

char getNumDice(char amount); 
char* getName(char amount); 
char* getRound(char amount); 

#endif
