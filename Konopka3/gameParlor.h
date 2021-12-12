#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

// semaphores for each game
sem_t Pictionary,Monopoly,Backgammon,Risk; 

// prototype functions
void* gameParlor(void* gameID); 
int checkGame(); 
void frontDesk();
void printMesssage(int state, int gameGroupID); 

// defines used for the code
#define AVAILABLE -1
