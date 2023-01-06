#include "gameParlor.h"



/* 
We made an array to hold all the gameIDs 
so we can check which game we are playing
the groups are as follows: 
1 & 2	Backgammon (4 dice)
3 & 4	Risk (5 dice)
5 & 6	Monopoly (2 dice)
7 & 8	Pictionary (1 die)
gCount will count the number of games we played
if the group played 5x, they are no longer allowed to play
*/

int available=maxDice;

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

char getNumDice(char amount){
    switch(amount){
        case 0: 
        case 1: 
            return 4;
            break;  
        case 2: 
        case 3: 
            return 5; 
            break; 
        case 4: 
        case 5: 
            return 2; 
            break; 
        case 6: 
        case 7: 
            return 1; 
            break; 
    }
}

char* getName(char amount){
    switch(amount){
        case 0: 
        case 1: 
            return "Backgammon";
            break;  
        case 2: 
        case 3: 
            return "Risk"; 
            break; 
        case 4: 
        case 5: 
            return "Monopoly"; 
            break; 
        case 6: 
        case 7: 
            return "Pictionary"; 
            break;    
    }
}

char* getRound(char amount){
    switch(amount){
        case 0: 
            return "first";
            break;  
        case 1: 
            return "second"; 
            break; 
        case 2: 
            return "third"; 
            break; 
        case 3: 
            return "fourth"; 
            break; 
        case 4: 
            return "fifth"; 
            break; 
        default: 
            return "TOO MANY ROUNDS!!"; 
            break; 
    }
}

void parlor(char gameID) {
    // check if dice are available
    printf("Front Desk: I have %d dice available\n", available); 

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
 
    printf("Group %d: Requesting %d dice for %s\n", gameID+1, games[gameID].numDice, games[gameID].name);
 
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
