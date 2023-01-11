#include "barberShop.h"

#define NUM_BARBERS 3
#define NUM_CHAIRS 5
#define NUM_CUSTOMERS 10

int main() {

	// set global and local varibles
	num_chairs = 5; // we have 5 chairs in our barber shop 
	haircutTime = 2; // customers will wait 2
	int sleepTime = 4; // it takes us 4 to cut someones hair
	int customers = 8; // we have 8 people wanting haircuts

	// Initialize semaphores, chairs will start at 1, where the others start at 0 
	sem_init(&chairs_mutex,0,1);
	sem_init(&sem_customer,0,0);
	sem_init(&sem_barber,0,NUM_BARBERS);

	// Create threads for our customers and barber
	pthread_t barberThread[NUM_BARBERS];
    pthread_t customerThread[NUM_CUSTOMERS];
	
	uint8_t i; 
	for (i = 0; i < NUM_BARBERS; i++) {
		uint8_t* ID = malloc(sizeof(uint8_t)); 
		*ID = i;  
        pthread_create(&barberThread[i], NULL, barber,  ID);
    }
    for (i = 0; i < NUM_CUSTOMERS; i++) {
		uint8_t* ID = malloc(sizeof(uint8_t)); 
		*ID = i; 
        pthread_create(&customerThread[i], NULL, customer, ID);
    }

    for (i = 0; i < NUM_BARBERS; i++) {
        pthread_join(barberThread[i], NULL);
    }
    for (i = 0; i < NUM_CUSTOMERS; i++) {
        pthread_join(customerThread[i], NULL);
    }

    sem_destroy(&chairs_mutex);
    sem_destroy(&sem_customer);
    sem_destroy(&sem_barber);

	// Sleep
	printf("The barber is sleeping for %d seconds\n", sleepTime);
	sleep(sleepTime);

	return 0; 
}
