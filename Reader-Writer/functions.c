#include "functions.h"

sem_t writting;
pthread_mutex_t studentMutex, teacherMutex;

unsigned char val = 0;
unsigned char studentsReading = 0;

// Remember the terachers are the writters in this senario
void *teacherThread(void *param) {   

    sem_wait(&writting);
    char ID = *(char*)param;
    char moreAssistance=0; 

    // two teachers should not be writting at the same time 
    pthread_mutex_lock(&teacherMutex);  
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

    char ID = *(char*)param;
    
    // We put a mutex here to process this one thread at a time
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