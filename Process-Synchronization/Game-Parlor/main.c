#include "gameParlor.h"

/* 
    The program is intended to simulate a game parlor with the use of threads and semaphores
    people go up to the gameparlor to borrow a # of dice for thier game
    each game has a different number of dice, and so we have to keep track of how many 
    dice the game parlor has at a time so we know if people can play a game
*/

int main() {

    pthread_t gameParlor, group[8];

	// create a semaphore of size 8, and have the game trypost 
    sem_init(&gameDone, 0, 0);
    sem_init(&canPlay, 0, 0);
    sem_init(&addQue, 0, 1);

    pthread_create(&gameParlor,NULL,parlor,NULL);
    char i, j; 

    // we create each thread 5x since each game is played 5x
    for(j=0; j<5; j++){
        
		for(i=0; i<8; i++){
            char* id = malloc(sizeof(char)); 
            *id = i; 
			// we also pass in the ID of the game so we know which one we are talking about
		    pthread_create(&group[i], NULL, game, id);
		}
		
		// here we ensure all games complete before we start the next round of gaming
		for(i=0; i<8; i++){
		    pthread_join(group[i],NULL);
		}
        firstGame=1; 
        count=0; 
		printf("All groups have finished their games, time to play again!\n\n");
    }
    
    // no one is playing games anymore  
    printf("The game parlor is now closing for the day, thanks for playing!\n");

    sem_destroy(&gameDone);
    sem_destroy(&canPlay);
    sem_destroy(&addQue);
    return 0; 
}


