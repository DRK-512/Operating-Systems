#include "gameProcessing.h"

/* 
   This file is used to store the processing functions for this project
*/

int available=maxDice;

void parlor(char gameID) {
    // check if dice are available
    printf("Front Desk: I have %d dice available\n", available); 
    printf("Group %d: Requesting %d dice for %s\n", gameID+1, games[gameID].numDice, games[gameID].name);

    if (games[gameID].state == NOT_PLAYED && available>games[gameID].numDice) {
        // since dice are avalibale, the game will start 
        games[gameID].state = PLAYING;
        available = available-games[gameID].numDice; 

        sem_post(&gameSem[gameID]);
        printf("Group %d: We are playing our %s game of %s\n", gameID+1, getRound(games[gameID].round), games[gameID].name); 
        
    }
}

void request_dice(char gameID) {
    
    sem_wait(&mutex);
 
    // check if we can play
    parlor(gameID);
 
    sem_post(&mutex);
 
    // if we cant play, wait until we can 
    sem_wait(&gameSem[gameID]);
 
}
void return_dice(char gameID){
    sem_wait(&returnMutex);
 
    // we are now placing our chopsticks to think
    games[gameID].state = DONE_PLAYING;
 
    printf("Group %d: We are done playing our %s game of %s\n", gameID+1, getRound(games[gameID].round), games[gameID].name);  
 
    // put dice back 
    available = available+games[gameID].numDice; 
 
    sem_post(&returnMutex);
}

void *game(void *param) {

    char i=0; 

    for(i=0; i<5; i++){
        sem_wait(&setupMutex);   
          
        char num = *(char*)param;
        games[num].round = i; 
        games[num].name = getName(num); 
        games[num].numDice = getNumDice(num); 
        games[num].state = NOT_PLAYED; 

        sem_post(&setupMutex); 

        request_dice(num); 
        sleep(1);
        return_dice(num);
    }
}
