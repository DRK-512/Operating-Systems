#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

int main() {
	/* Instead of making flags for these parameters, we instead do scanf
	 * The reason is because these parameters are required for every run
	 * and should be adjustable, and changed every run
	 */
	uint8_t num_people; 
	uint8_t num_floors; 
	uint8_t num_elevators; 
	uint8_t waitTime; 
	
	printf("\nHow many people are waiting? "); 
	scanf("%hhd", &num_people); 
	
	printf("How many floors does the building have? "); 
	scanf("%hhd", &num_floors); 
	
	printf("How long is everyone willing to wait? "); 
	scanf("%hhd", &waitTime); 
	
    pthread_t riderT[num_people]; 
    
    uint8_t i; 
    for(i=0; i<num_people; i++){
        uint8_t* ID = malloc(sizeof(uint8_t));
        *ID = i; 
    //    pthread_create(&riderT[i], NULL, people, ID); 
    }


    for(i=0; i<num_people; i++){
    //    pthread_join(riderT[i], NULL); 
    }

    printf("Everyone has been sent to the floor they requested\n"); 
    printf("There are %d people, %d floors, and %d seconds", num_people, num_floors, waitTime); 
    return 0; 
}
