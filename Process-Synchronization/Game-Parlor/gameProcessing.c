#include "gameProcessing.h"

/* 
   This file is used to store the processing functions for this project
*/

// this is only global for the parlor
int available=maxDice;

void parlor(char gameID, char state) {
    if(state == START_GAME) {
        sem_wait(&mutex);
 
        // check if we can play
        // check if dice are available
        printf("Front Desk: I have %d dice available\n", available); 
        printf("Group %d: Requesting %d dice for %s\n", gameID+1, games[gameID].numDice, games[gameID].name);

        while(available<games[gameID].numDice); 

        available = available-games[gameID].numDice; 


        printf("Group %d: We are playing our %s game of %s\n", gameID+1, getRound(games[gameID].round), games[gameID].name); 

        sem_post(&mutex);
    } else if (state==FINISH_GAME) {
        sem_wait(&returnMutex);
 
        printf("Group %d: We are done playing our %s game of %s\n", gameID+1, getRound(games[gameID].round), games[gameID].name);  

        // put dice back 
        available = available+games[gameID].numDice; 

        sem_post(&returnMutex);
    }
}

void *game(void *param) {

    sem_wait(&setupMutex);   

    mainGame stats = *(mainGame*)param;  

    games[*stats.id].round = *stats.round; 
    games[*stats.id].name = getName(*stats.id); 
    games[*stats.id].numDice = getNumDice(*stats.id); 

    sem_post(&setupMutex); 

    parlor(*stats.id, START_GAME); 
    sleep(1);
    parlor(*stats.id, FINISH_GAME);
    
}
