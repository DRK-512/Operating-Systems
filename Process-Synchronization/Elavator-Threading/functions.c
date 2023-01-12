#include "functions.h"

uint8_t num_people = NUM_PEOPLE;
uint8_t num_floors = NUM_ELEVATORS; 

sem_t elavatorMutex, req_sem, modBuffer; 

typedef struct elavatorRequest{
    uint8_t cFloor; 
    uint8_t rFloor; 
}request_struct; 

request_struct request[NUM_PEOPLE]; 
uint8_t buffer[NUM_PEOPLE] = {0}; 

void requestElavator(uint8_t reqID){

    printf("Person %d is waiting for an elavator\n", reqID);  
    sem_wait(&modBuffer);

    if(buffer[0] == 0){
        buffer[0] = reqID; 
    } else {
        uint8_t i=0; 
        while(buffer[i]!=0){
            i++; 
            if(i>=NUM_PEOPLE){
                for(i=0; i<5; i++){
                    printf("ERROR: PERSON BUFFER IS FULL!!\n"); 
                }
                break; 
            }
        }
        buffer[i] = reqID; 
    }
    sem_post(&modBuffer);

    // now that a person is part of the buffer, we can call the elavator
    sem_post(&req_sem); 

}
void *elavator(void *param){
    uint8_t ID = *(uint8_t*)param; 
    uint8_t i, pID;
    while(num_people>0){
        
        sem_wait(&req_sem);
        if(buffer[0]==0){
            for(i=0; i<5; i++){
                printf("ERROR: PERSON BUFFER IS EMPTY!!\n"); 
            }
        }

        sem_wait(&modBuffer);
        pID = buffer[0];
        printf("Person %d is now going to floor %d\n", pID, request[pID].rFloor);
        for(i=0;i<NUM_PEOPLE-1;i++) {
            buffer[i]=buffer[i+1];
        }

        sem_post(&modBuffer);

        // simulate # time to go to floor
        sleep(1); 
        printf("Person %d has made it to floor %d\n", pID, request[pID].rFloor);  
            
        num_people--; 

    }
}

void *people(void *param){
    uint8_t ID = (*(uint8_t*)param) + 1; 
    // randomly generate floor numbers
    uint8_t currentFloor=0, requestFloor=0;

    while(currentFloor==requestFloor){
        currentFloor = rand() % (num_floors+1);  // comment this line if you want everyone to start on floor 0
        requestFloor = rand() % (num_floors+1); 
    }

    if(currentFloor==0){
        printf("Person %d is at the lobby, and wants to go to floor %d\n", ID, requestFloor); 
    } else if(requestFloor==0){
        printf("Person %d is on floor %d, and wants to go to the lobby\n", ID, currentFloor); 
    } else {
        printf("Person %d is on floor %d, and wants to go to floor %d\n", ID, currentFloor, requestFloor); 
    }

    // we now have the info needed for the struct
    request[ID]; 
    request[ID].cFloor = currentFloor; 
    request[ID].rFloor = requestFloor; 

    requestElavator(ID);  
} 