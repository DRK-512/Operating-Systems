#include "gameParlor.h"

/* 
    The program is intended to simulate a game parlor with the use of threads and semaphores
    people go up to the gameparlor to borrow dice for thier game
    each game has a different number of dice, and so we have to keep track of how many 
    dice the game parlor has at a time so we know if people can play a game
*/
#define numDie 8
int main() {

    // create threads and initalize the mutex
    pthread_t gameGroup[numDie+1];

    // array of threads (-1 is for the front desk)
    static int gameTypes[] = {-1, 0, 1, 2, 3, 4, 5, 6, 7};
 
    // create threads for each game
    int i; 
    for (i = 0; i < 9; i++) {
        pthread_create(&gameGroup[i], NULL, gameParlor, &gameTypes[i]);
    }

    // join all gamer groups
    for (int i = 0; i <9; i++){
        pthread_join(gameGroup[i], NULL);
    }

    return 0;
}



