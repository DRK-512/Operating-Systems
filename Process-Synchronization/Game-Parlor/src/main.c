#include "../include/gameProcessing.h"

/* 
 * The program is intended to simulate a game parlor with the use of threads  
 * and semaphores people go up to the game parlor to borrow a number of dice for 
 * for their game each game has a different number of dice, and so we have to 
 * keep track of the dice so we know if a game can be played at that time or not
 */

int main() {
	char i, j; // switch this to a uint8
	pthread_t game_threads[maxDice];
	
	sem_init(&mutex, 0, 1);
	sem_init(&returnMutex, 0, 1);
	sem_init(&setupMutex, 0, 1);

	for(j=0; j<NUM_ROUNDS; j++){
		for (i = 0; i < maxDice; i++) {
			// add params to struct then call thread with struct		
			char* id = malloc(sizeof(char)); 
			*id = i; 
			// we also pass in the ID of the game so we know which one we are talking about
			pthread_create(&game_threads[i], NULL, game, id);
		}
		for (i = 0; i < maxDice; i++) 
			pthread_join(game_threads[i], NULL);
	}
	printf("\nAll games have completed their %d rounds!\nThanks for playing!\n", NUM_ROUNDS); 
}
