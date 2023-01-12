#include "functions.h"

#define NUM_TEACHERS 2
#define NUM_STUDENTS 5

int main() {   

    // In our case, the teacher is the writter and the students are the readers 
    pthread_t teachers[NUM_TEACHERS], students[NUM_STUDENTS];

    pthread_mutex_init(&studentMutex, NULL);
    pthread_mutex_init(&teacherMutex, NULL);
    sem_init(&writting,0,1);

    uint8_t i; 
    // create the threads, and pass in the values with respect to them 
    for(i=0; i<NUM_TEACHERS; i++){
        uint8_t* id = malloc(sizeof(uint8_t));
        *id = i; 
        pthread_create(&teachers[i], NULL, teacherThread, id);
    }
    for(i=0; i<NUM_STUDENTS; i++){
        uint8_t* id = malloc(sizeof(uint8_t));
        *id = i; 
        pthread_create(&students[i], NULL, studentThread, id);
    }

    for(i = 0; i < NUM_TEACHERS; i++) {
        pthread_join(teachers[i], NULL);
    }
    for(i = 0; i < NUM_STUDENTS; i++) {
        pthread_join(students[i], NULL);
    }

    // make sure to get rid of the semaphores and threads
    pthread_mutex_destroy(&studentMutex);
    pthread_mutex_destroy(&teacherMutex);
    sem_destroy(&writting);

    return 0;
    
}
