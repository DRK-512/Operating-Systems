#include "gameParlor.h"

/* 
    The program is intended to simulate a game parlor with the use of threads and semaphores
    people go up to the gameparlor to borrow dice for thier game
    each game has a different number of dice, and so we have to keep track of how many 
    dice the game parlor has at a time so we know if people can play a game
*/

// NOTE: here we make sure all games finish before starting the next round
// the reason for this is because it would take more memory to make pictionaries's 3rd x to compete with monopolies 1st x
int main() {

    pthread_t gameParlor, group[8];

	// create a semaphore of size 8, and have the game trypost 
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, 8);
    pthread_mutex_init(&mutex, NULL);
    
    pthread_create(&gameParlor,NULL,parlor,NULL);
    long i; 
    int j; 
    
    // we create each thread 5x since each game is played 5x
    for(j=0; j<5; j++){
		for(i=0; i<8; i++){
			// we also pass in the ID of the game so we know which one we are talking about
		    pthread_create(&group[i], NULL, game, (void*)i);
		}
		
		// here we ensure all games complete before we start the next round of gaming
		for(i=0; i<8; i++){
		    pthread_join(group[i],NULL);
		}
		printf("All groups have finished their games, time to play again!\n\n");
    }
   
   	// now we close the gameparlor
	pthread_join(gameParlor,NULL);
    
    sem_destroy(&full);
    sem_destroy(&empty);
    pthread_mutex_destroy(&mutex);
    
    return 0; 
}


