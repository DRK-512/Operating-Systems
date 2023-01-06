#ifndef gameStats
#define gameStats

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define maxDice 8

sem_t mutex, setupMutex, returnMutex;
sem_t gameSem[maxDice];
 
enum states {NOT_PLAYED, PLAYING, DONE_PLAYING}; 

struct gameStruct {
    char* name; 
    char numDice; 
    char state; 
    char round; 
}; 

struct gameStruct games[maxDice]; 

char getNumDice(char amount); 
char* getName(char amount); 
char* getRound(char amount); 

#endif
