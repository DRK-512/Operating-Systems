
#include "barberShop.h"

uint8_t num_chairs;
uint8_t haircutTime;
sem_t chairs_mutex;
sem_t sem_customer;
sem_t sem_barber;

void *barber(void *param) {  
	uint8_t worktime; 
	while(1) {
		// wait for a customer
		sem_wait(&sem_customer);
		// waiting for a chair to become available
		sem_wait(&chairs_mutex);
		num_chairs += 1;

		if(num_chairs==1){
			printf("The Barber is taking in a customer\nThere is %d chair available\n",num_chairs);
		} else {
			printf("The Barber is taking in a customer\nThere are %d chairs available\n",num_chairs);
		}

		// let customer know that the seat is empty
		sem_post(&sem_barber);
		sem_post(&chairs_mutex);

		// create random number for how long the barber cuts the hair (from 1-5 seconds) 
		worktime = (rand() % 5) + 1;
		printf("The Barber is cutting hair for %d seconds\n", worktime);
		sleep(worktime);
	} 
}

void *customer(void *param) {
	uint8_t waittime;
	uint8_t taskid = *(uint8_t*) param;
	
	// wait for chairs to become avalible 
	sem_wait(&chairs_mutex);

	if(num_chairs > 0){

		num_chairs -= 1;
		if(num_chairs==1){
			printf("Client %d is sitting in the waiting room\nThere is %d chair left\n",taskid,num_chairs);
		} else {
			printf("Client %d is sitting in the waiting room\nThere are %d chairs left\n",taskid,num_chairs);
		}
		// signal that a customer is ready 
		sem_post(&sem_customer);
		sem_post(&chairs_mutex);
		sem_wait(&sem_barber);

		// after they wait, the barber can cut their hair
		printf("Client %d is getting their haircut\n",taskid);
	} else {

		// generate random number for the waittime, 
		waittime = (rand() % haircutTime) + 1;

		// sleep for waittime seconds 
		printf("Client %u is waiting %d seconds for a seat\n",taskid,waittime);
		sleep(waittime);

		if(num_chairs > 0){

			num_chairs -= 1;
			if(num_chairs==1){
				printf("Client %d is sitting in the waiting room\nThere is %d chair left\n",taskid,num_chairs);
			} else {
				printf("Client %d is sitting in the waiting room\nThere are %d chairs left\n",taskid,num_chairs);
			}
			// signal that a customer is ready 
			sem_post(&sem_customer);
			sem_post(&chairs_mutex);
			sem_wait(&sem_barber);

			// after they wait, the barber can cut their hair
			printf("Client %d is getting their haircut\n",taskid);
		} else {
			// free mutex lock on chair count, because our customer was impatient
			printf("Client %d is leaving without a haircut\n", taskid);
			sem_post(&chairs_mutex);
		}	
	}	
}
