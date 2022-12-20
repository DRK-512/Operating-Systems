
#include "barberShop.h"

int num_chairs;
int haircutTime;
sem_t chairs_mutex;
sem_t sem_customer;
sem_t sem_barber;

void *barber(void *param) {
   int worktime;
  
   while(1) {
		// waiting for a customer to become available
		sem_wait(&sem_customer);
		// waiting for a chair to become available, then increment the chair
		sem_wait(&chairs_mutex);
		num_chairs += 1;
		if(num_chairs==1){
			printf("The Barber is taking in a customer\nThere is %d chair available\n",num_chairs);
		} else {
			printf("The Barber is taking in a customer\nThere are %d chairs available\n",num_chairs);
		}

		// let customer know that the seat is empty, then lock the chair
		sem_post(&sem_barber);
		sem_post(&chairs_mutex);

		// create random number for worktime (from 1-5 seconds) for length of time to cut the hair. 
		worktime = (rand() % 5) + 1;
		printf("The Barber is cutting hair for %d seconds\n", worktime);
		sleep(worktime);
    } 
}

void *customer(void *threadid) {
   int waittime;
	long taskid = (long) threadid;
   while(1) {
		// wait for chairs to be avalible 
		sem_wait(&chairs_mutex);
		// if there are chairs avalible
		if(num_chairs > 0){

		   num_chairs -= 1;
		   if(num_chairs==1){
	  			printf("Client %ld is sitting in the waiting room\nThere is %d chair left\n",taskid,num_chairs);
	  		} else {
				printf("Client %ld is sitting in the waiting room\nThere are %d chairs left\n",taskid,num_chairs);
	  		}
		   // signal that a customer is ready 
		   sem_post(&sem_customer);
		   // free mutex lock on chair count 
		   sem_post(&chairs_mutex);
		   // wait for barber to be ready to cut their hair
		   sem_wait(&sem_barber);
		   // after they wait, the barber can cut their hair
		   printf("Client %ld is getting their haircut\n",taskid);
		} else {
			// free mutex lock on chair count, because our customer was impatient
			printf("Client %ld is leaving without a haircut\n", taskid);
			sem_post(&chairs_mutex);
		}

		// generate random number for the waittime, 
		waittime = (rand() % haircutTime) + 1;
		
		// sleep for waittime seconds 
		printf("Client %lu is waiting %d seconds before attempting another haircut\n",taskid,waittime);
		sleep(waittime);
	}
}

