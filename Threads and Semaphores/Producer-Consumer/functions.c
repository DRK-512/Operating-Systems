#include "functions.h"

sem_t empty, full, mutex; 
int data=0; 

void *Producer(void *param) {
    int produced;
    long threadID = (long)param;
    printf("Producer %ld has been created\n",threadID);
    
    // we have it go up to 13 because sum(nConsume[0:2]) is 13
    for(produced=0;produced<13;produced++) {
        sem_wait(&empty);
        sem_wait(&mutex);
        data=produced;
        sem_post(&mutex);
        sem_post(&full);
        printf("Produced: %d\n",data);
    }
}

void *Consumer(void *param) {
    int consumed, nConsume, total=0;
    long threadID = (long)param;
    printf("Consumer %ld has been created\n",threadID);

    switch(threadID){
        case 0: 
            nConsume=4;
            break;  
        case 1: 
            nConsume=6;
            break; 
        case 2: 
            nConsume=2;
            break; 
        default: 
            nConsume=1;
            break; 

    }

    for(consumed=0;consumed<nConsume;consumed++) {
        sem_wait(&full);
        sem_wait(&mutex);
        total=total+data;
        printf("Thread: %ld, Consumed: %d / %d\n", threadID, consumed+1, nConsume);
        sem_post(&mutex);
        sem_post(&empty);
    }

    printf("The total amount of data consumed after thread %ld is %d\n",threadID, total);
}