#include "functions.h"

#define numTeachers 2
#define numStudents 5

int main() {   

    // In our case, the teacher is the writter and the students are the readers 
    pthread_t teachers[numTeachers], students[numStudents];

    pthread_mutex_init(&studentMutex, NULL);
    pthread_mutex_init(&teacherMutex, NULL);
    sem_init(&writting,0,1);

    char i; 

    // create the threads, and pass in the values with respect to them 
    for(i=0; i<numTeachers; i++){
        char* id = malloc(sizeof(char));
        *id = i; 
        pthread_create(&teachers[i], NULL, teacherThread, id);
    }
    for(i=0; i<numStudents; i++){
        char* id = malloc(sizeof(char));
        *id = i; 
        pthread_create(&students[i], NULL, studentThread, id);
    }

    for(i = 0; i < numTeachers; i++) {
        pthread_join(teachers[i], NULL);
    }
    for(i = 0; i < numStudents; i++) {
        pthread_join(students[i], NULL);
    }

    pthread_mutex_destroy(&studentMutex);
    pthread_mutex_destroy(&teacherMutex);
    
    sem_destroy(&writting);

    return 0;
    
}
