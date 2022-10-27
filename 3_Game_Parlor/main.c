#include "gameParlor.h"

/* 
    The program is intended to simulate a game parlor with the use of threads and semaphores
    people go up to the gameparlor to borrow dice for thier game
    each game has a different number of dice, and so we have to keep track of how many 
    dice the game parlor has at a time so we know if people can play a game
*/

int main() {

    // create semaphores for the dice 
    num_dice=8;
    sem_init(&dice_mutex, 0, 8); // only the parlor can know how many dice are availble, so that is why we have 0 
    
    // create you parlor thread
    pthread_t parlorID; 
    pthread_create(&parlorID, NULL, parlor, NULL); 

    // create all the game threads   
    int i; 
    for(i=0; i<8;i++){
        pthread_create(&gameID[i],NULL,game,NULL);
    }

    free(gCount); 
    return 0; 
}



