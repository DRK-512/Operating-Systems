#include "functions.h"

sem_t writting;
pthread_mutex_t studentMutex, teacherMutex;

uint8_t val = 0;
uint8_t studentsReading = 0;

// Remember the terachers are the writters in this senario
void *teacherThread(void *param) {   

    // wait until no students are reading the board
    sem_wait(&writting);
    uint8_t ID = *(uint8_t*)param;
    uint8_t moreAssistance=0; 

    // two teachers should not be writting at the same time 
    pthread_mutex_lock(&teacherMutex);  

    // check if our teacher has teach assistant's
    if(ID!=0){
        moreAssistance=1;
    }

    // here I change the value to simulate some writting on the board
    val++; 
    // if there are more than 
    if(moreAssistance){
        printf("The assistant: %d modified the value on the board to %d\n", ID, val);
    } else {
        printf("The teacher modified the value on the board to %d\n", val);
    }

    pthread_mutex_unlock(&teacherMutex); 
    sem_post(&writting);

}

// Remember the students are the readers in this senario
void *studentThread(void *param) {   

    uint8_t ID = *(uint8_t*)param;
    
    // We put a mutex here to process this one student at a time
    pthread_mutex_lock(&studentMutex);
    studentsReading++;

    // Since students are now reading the board, we stop the teacher from writting
    if(studentsReading == 1) {
        sem_wait(&writting); 
    }
    pthread_mutex_unlock(&studentMutex);

    // can put a sleep here to simulate students writting, but for this example I won't 
    printf("Student: %d reads the value %d from the board\n",ID,val);
    
    pthread_mutex_lock(&studentMutex);

    studentsReading--;
    // if all students have finished taking notes, we let the teacher know 
    if(studentsReading == 0) {
        sem_post(&writting); 
    }
    pthread_mutex_unlock(&studentMutex);
}