#include "../include/gameProcessing.h"

/* 
 * This file is used to store the processing functions for this project
 */

void gameParlor(char gameID, char state) {
	if (state == START_GAME) {
		sem_wait(&mutex); // wait until we can request dice

		printf("Front Desk: I have %d dice available\n", available); 
		printf("Group %d: Requesting %d dice for %s\n", gameID+1, games[gameID].numDice, games[gameID].name);

		// wait for dice to be available
		while(available < games[gameID].numDice); 

		// take dice
		available = available - games[gameID].numDice; 

		printf("Group %d: We are playing our %s game of %s\n", gameID+1, getRoundName(games[gameID].round), games[gameID].name); 

		sem_post(&mutex);
	} else if (state == FINISH_GAME) {
		sem_wait(&returnMutex);

		printf("Group %d: We are done playing our %s game of %s\n", gameID+1, getRoundName(games[gameID].round), games[gameID].name);  

		// put dice back 
		available = available + games[gameID].numDice; 

		sem_post(&returnMutex);
	}
}

void *game(void *param) {

	// When using input to make globals, mutex's are needed
	sem_wait(&setupMutex);   

	char ID = *(char*)param;

	// Game thread only knows which round they are on, their game, and the number of dice they need to play
	games[ID].round = rounds[ID]; 
	games[ID].name = getName(ID); 
	games[ID].numDice = getNumDice(ID); 

	sem_post(&setupMutex); 

	// The game parlor does the rest of the work
	gameParlor(ID, START_GAME); 
	sleep(PLAY_TIME);
	gameParlor(ID, FINISH_GAME);
	rounds[ID]++; 
}
