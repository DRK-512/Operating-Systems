#include "functions.h"

int main() {

    pthread_t ptid, ctid[3];

    sem_init(&mutex, 1, 1);
    sem_init(&full, 1, 0);
    sem_init(&empty, 1, 1);
    
    pthread_create(&ptid,NULL,Producer,NULL);
    long i; 
    for(i=0; i<3; i++){
        pthread_create(&ctid[i], NULL, Consumer, (void*)i);
    }
    
    pthread_join(ptid,NULL);
    for(i=0; i<3; i++){
        pthread_join(ctid[i],NULL);
    }
    
    return 0; 
}
