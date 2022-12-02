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

sem_t empty, full; 
pthread_mutex_t mutex;

void *parlor(void *param) {
    int produced;
    long threadID = (long)param;
    int max = (5*(5+4+2+1)); // each game 5x 
     
    printf("Welcome to the Game Parlor!\nThe groups will start their games shortly\n"); 
    
    for(produced=0;produced<max;produced++) {
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        //printf("Produced: %d\n",produced);
    }
}

void *game(void *param) {
    int consumed, nConsume, total=0;
    long threadID = (long)param;

    switch(threadID){
        case 0: 
        case 1: 
            nConsume=4;
            break;  
        case 2:
        case 3:  
            nConsume=5;
            break; 
        case 4:
        case 5:  
            nConsume=2;
            break; 
        case 6: 
        case 7:  
            nConsume=1;
            break;
        default: 
        	printf("ERROR: WE GOT MORE THAN 8 THREADS!!"); 
        	exit(-1);  

    }
    sem_wait(&full);
    for(consumed=0;consumed<nConsume;consumed++) {
        
        pthread_mutex_lock(&mutex);
        sleep(1);  
        pthread_mutex_unlock(&mutex);
        
    }
	sem_post(&empty);
    printf("Thread %ld has finished for now\n",threadID);
}
