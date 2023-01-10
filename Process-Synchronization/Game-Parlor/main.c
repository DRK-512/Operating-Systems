#include "gameProcessing.h"

/* 
    The program is intended to simulate a game parlor with the use of threads and semaphores
    people go up to the gameparlor to borrow a # of dice for thier game
    each game has a different number of dice, and so we have to keep track of how many 
    dice the game parlor has at a time so we know if people can play a game
*/

int main() {

    // create threads and initalize the mutex
	pthread_t game_threads[maxDice];
	sem_init(&mutex, 0, 1);
	sem_init(&returnMutex, 0, 1);
	sem_init(&setupMutex, 0, 1);

	char i, j;

	// the assignment asks for 5, but this is more cool 
	numRounds = 5; 

	for(j=0; j<numRounds; j++){
		for (i = 0; i < maxDice; i++) {
			// add params to struct then call thread with struct		
			char* id = malloc(sizeof(char)); 
            *id = i; 
			// we also pass in the ID of the game so we know which one we are talking about
		    pthread_create(&game_threads[i], NULL, game, id);
		}
		for (i = 0; i < maxDice; i++) {
			pthread_join(game_threads[i], NULL);
		}
	}
	printf("\nAll games have completed their %d rounds!\nThanks for playing!", numRounds); 
}
