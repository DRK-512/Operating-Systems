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
    
	mainGame s_game;
	int i, j;

	for(j=0; j<5; j++){
		for (i = 0; i < maxDice; i++) {
			// add params to struct then call thread with struct
			char* id1 = malloc(sizeof(char)); 
			char* id2 = malloc(sizeof(char)); 
			*id1 = i;
			*id2 = j;
			s_game.id = id1;
			s_game.round = id2;

			pthread_create(&game_threads[i], NULL, game, &s_game);
		}

		for (i = 0; i < maxDice; i++) {
			pthread_join(game_threads[i], NULL);
		}
	}
}
