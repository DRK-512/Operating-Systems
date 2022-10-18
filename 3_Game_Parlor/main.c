#include "gameParlor.h"

/* 
    Author: Darek Konopka & Tejas Bolar 
    Assignment Number: 3
    Date of Submission: 11/24/2021
    Name of this file: main.c
    Description of the program: The program is intended to simulate a 
    game parlor with the use of threads and semaphores
*/

// Here we import these from the gameParloc.h
sem_t Pictionary,Monopoly,Backgammon,Risk,mutex; ; // semaphore for mutex & games

int main() {
    // semaphores have a return value for error checking, so we decided to throw that in 
    int semError[5]; 
    // initalize the games (pshared is 1 since we want threads to see it)
    // (decement process should start at 0 since we have sleeps)
    semError[0] = sem_init(&Pictionary,1,0);
    semError[1] = sem_init(&Monopoly,1,0);
    semError[2] = sem_init(&Backgammon,1,0);
    semError[3] = sem_init(&Risk,1,0);

    for(int i=0; i<5; i++) {
        if(semError[i]==-1){
            printf("Semaphore Error"); 
            return -1; 
        }
    }
    // array of threads we will be making (-1 is for the front desk)
    int gameTypes[9] = {-1, 0, 1, 2, 3, 4, 5, 6, 7};

    // initalize threads
    pthread_t gameGroup[9];

    // create threads for each game
    for (int i = 0; i < 9; i++) {
        pthread_create(&gameGroup[i], NULL, gameParlor, &gameTypes[i]);
    }

    // join all gamer groups
    for (int i = 0; i <9; i++){
        pthread_join(gameGroup[i], NULL);
    }

    return 0;
}



