#include "functions.h"

// All global values are in functions.h

int main() {

    pthread_t peopleT[NUM_PEOPLE], elavatorT[NUM_ELEVATORS]; 
    sem_init(&elavatorMutex, 0, 1); 
    sem_init(&req_sem, 0, 0);  
    sem_init(&modBuffer, 0, 1); 

    uint8_t i; 
    for(i=0; i<NUM_PEOPLE; i++){
        uint8_t* ID = malloc(sizeof(uint8_t));
        *ID = i; 
        pthread_create(&peopleT[i], NULL, people, ID); 
    }
    for(i=0; i<NUM_ELEVATORS; i++){
        uint8_t* ID = malloc(sizeof(uint8_t));
        *ID = i; 
        pthread_create(&elavatorT[i], NULL, elavator, ID); 
    }

    for(i=0; i<NUM_PEOPLE; i++){
        pthread_join(peopleT[i], NULL); 
    }
    for(i=0; i<NUM_ELEVATORS; i++){
        pthread_join(elavatorT[i], NULL); 
    }
    printf("Everyone has been sent to the floor they requested\n"); 
    return 0; 
}