#include "barberShop.h"

int main() {

	// set global and local varibles
	num_chairs = 5; // we have 5 chairs in our barber shop 
	haircutTime = 2; // customers will wait 2
	int sleepTime = 4; // it takes us 4 to cut someones hair
	int customers = 8; // we have 8 people wanting haircuts

	// Initialize semaphores, chairs will start at 1, where the others start at 0 
	sem_init(&chairs_mutex,0,1);
	sem_init(&sem_customer,0,0);
	sem_init(&sem_barber,0,0);

	// Create thread for barber
	pthread_t barberID;
	pthread_create(&barberID, NULL, barber, NULL);
	printf("Creating barber with ID: %lu\n",barberID);

	// Create threads for customers
	pthread_t customerIDs[20]; 
	long i; 
	for (i = 0; i < customers; i++) {
      	pthread_create(&customerIDs[i], NULL, customer, (void *) i);
        printf("Creating customer with ID: %ld\n",i);
	}

	// Sleep
	printf("The barber is sleeping for %d seconds\n", sleepTime);
	sleep(sleepTime);

	return 0; 
}


