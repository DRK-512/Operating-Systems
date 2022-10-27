#include "gameParlor.h"

int num_die=8;
sem_t dice_mutex; 
sem_t sem_parlor; 
sem_t sem_game; 
pthread_t gameID[8]; 
char gCount = (char*)calloc(8, sizeof(char));

void *parlor(void *param){

	while(1){
		// wait for someone to ask to play game 
		sem_wait(&sem_game);
		// wait for dice to be available
		sem_getvalue()

	}
}


void *game(void *param){

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

	while(1){
		//
		sem_wait(&dice_mutex); 
	}

}
