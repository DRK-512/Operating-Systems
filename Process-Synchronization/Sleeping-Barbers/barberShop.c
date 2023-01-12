
#include "barberShop.h"

uint8_t num_chairs;
uint8_t num_customers; 
sem_t chairs_mutex;
sem_t sem_customer;
sem_t sem_barber;

#define MAX_WAIT 5

void *barber(void *param) {  
	uint8_t worktime; 
	while(num_customers > 0) {
		// wait for a customer, then for them to sit in the barbers seat
		sem_wait(&sem_customer);
		sem_wait(&chairs_mutex);
		num_chairs += 1;

		if(num_chairs==1){
			printf("The Barber is starting to cut a customers hair\nThere is %d chair available\n",num_chairs);
		} else {
			printf("The Barber is starting to cut a customers hair\nThere are %d chairs available\n",num_chairs);
		}

		// let customers know that a waiting seat is free
		sem_post(&sem_barber);
		sem_post(&chairs_mutex);

		// create random number for time the barber cuts the hair
		worktime = (rand() % MAX_WAIT) + 1;
		printf("The Barber is cutting hair for %d seconds\n", worktime);
		sleep(worktime);
	} 
}

void *customer(void *param) {
	uint8_t waittime;
	uint8_t taskid = *(uint8_t*) param;
	
	// taskid will be 0-NUM_CUSTOMERS, but we want our first client to be client #1 not #0
	taskid++; 
	
	// wait for chairs to become avalible 
	sem_wait(&chairs_mutex);

	if(num_chairs > 0){

		num_chairs -= 1;
		if(num_chairs==1) {
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
		// generate random number for time they are willing to wait for a seat
		waittime = (rand() % MAX_WAIT) + 1;

		// sleep for waittime seconds 
		printf("Client %u is waiting %d seconds for a seat\n",taskid,waittime);
		sleep(waittime);

		if(num_chairs > 0){

			num_chairs -= 1;
			if(num_chairs==0) {
				printf("Client %d is sitting in the waiting room\nAll chairs are taken\n",taskid);
			} else if(num_chairs==1){
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
	num_customers--; 	
}
