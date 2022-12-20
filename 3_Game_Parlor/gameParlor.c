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


sem_t diceCheck, canPlay, gameDone, changeQue, addQue; 

#define maxDice 8
char playingGames=1, count=0; 

// initalize all to 0 (0 means not in use)
char available = maxDice; 
char buffer[8] = {0}; // will hold which team is going 

char fetchNumDice(char gameID){
    // find out how many dice we are working with 
    char dice=0; 
    switch(gameID){
        case 1: 
        case 2: 
            dice=4;
            break;  
        case 3:
        case 4:  
            dice=5;
            break; 
        case 5:
        case 6:  
            dice=2;
            break; 
        case 7: 
        case 8:  
            dice=1;
            break; 
        default: 
            // gameID = null, we skip for now
            break;  
    }   
    return dice; 
}

char* getGame(char numDice){
        // find out how many dice we are working with 
    char* gameName; 
    switch(numDice){
        case 1: 
        case 2: 
            gameName="Backgammon";
            break; 
        case 3:
        case 4: 
            gameName="Risk";
            break; 
        case 5:
        case 6:
            gameName="Monopoly";
            break; 
        case 7: 
        case 8: 
            gameName="Pictionary"; 
            break;  
        default: 
            gameName=""; 
            break;  
    }   
    return gameName; 
}

void *parlor() {
    // will repeate int max = (5*(5+4+2+1)); // each game 5x 
    // probably should lock before we start
    // check which game is requesting dice, then take (will stay in while until we can)
    // once game finishes, place dice back 
    char i, checkNum; 
    printf("Front Desk: I have %d dice available\n", available);
    while(playingGames){
        
        sem_wait(&diceCheck);
        
        checkNum = fetchNumDice(buffer[0]);
        printf("Group %d is requsting %d for %s\n", buffer[0], checkNum, getGame(buffer[0])); 
        // check if available 
        if(available>=checkNum){
            sem_post(&diceCheck);
            sem_wait(&changeQue);

            // remove dice from availble 
            available = available-checkNum; 
            printf("Front Desk: I have %d dice available\n", available);
            // play the game
            sem_post(&canPlay); 
            // wait till game done
            sem_wait(&gameDone);

            // remove group from buffer
            for(i = 0; i < maxDice-1; i++){        
                buffer[i]=buffer[i+1];
            }

            // put dice back 
            available = available+checkNum;

            sem_post(&changeQue);
        } else {
            sem_post(&diceCheck);
        }
         
    }
}

void *game(void *param) {
    
    char ID = *(char*)param;

    sem_wait(&addQue);
    buffer[count] = ID+1;
    count++; 
    sem_post(&addQue);

    // sem_wait until can play
    sem_wait(&canPlay); 
    // if can play, so youre playing, then sleep
    printf("Group %d is now playing %s\n", buffer[0], getGame(buffer[0])); 
    sleep(1);
    sem_post(&gameDone); 
}
