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


sem_t diceCheck, canPlay, gameDone; 

#define maxDice 8
char playingGames=1; 
char gameID; 
// initalize all to 0 (0 means not in use)
char available[maxDice] = {0}; 

char fetchNumDice(){
    // find out how many dice we are working with 
    char dice=0; 
    switch(gameID){
        case 0: 
        case 1: 
            dice=4;
            break;  
        case 2:
        case 3:  
            dice=5;
            break; 
        case 4:
        case 5:  
            dice=2;
            break; 
        case 6: 
        case 7:  
            dice=1;
            break; 
        default: 
            printf("ERROR: Too many gameID's");
            exit(1);  
    }   
    return dice; 
}

void *parlor() {
    // will repeate int max = (5*(5+4+2+1)); // each game 5x 
    // probably should lock before we start
    // check which game is requesting dice, then take (will stay in while until we can)
    // once game finishes, place dice back 
    int i, parlorDice;
    int diceSlots[maxDice]; 
    char reqDie;  

    while(playingGames){
        
        sem_wait(&diceCheck);
        reqDie = fetchNumDice();

        // wait until available 
        while(reqDie<=parlorDice){
            parlorDice=0; 
            for(i=0; i<maxDice; i++){
                parlorDice++; 
                diceSlots[parlorDice]=i; 
            }
            // now we set dice to 1 since they are in use
            for(i=0; i<parlorDice; i++){
                available[diceSlots[i]] == 1;      
            }
        }    

        // post canWait
        sem_post(&canPlay); 
        // wait till game done
        sem_wait(&gameDone); 

        // place dice back 
        for(i=0; i<maxDice; i++){
            if(diceSlots[i]==1){
                available[i] == 0; 
            }
        }
            sem_post(&diceCheck);
        
    }
}

void *game(void *param) {
    
    // set gloabl var (whoseAsking) to the ID
    char ID = *(char*)param;
    printf(" Our ID is: %d \n", ID); 
        
    // sem_wait until can play
    sem_wait(&canPlay); 
    // if can play, so youre playing, then sleep
    printf("Group %d is now playing\n", gameID); 
    sleep(1);
    sem_post(&gameDone); 
}
