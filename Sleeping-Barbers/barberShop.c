
#include "barberShop.h"

sem_t chairs_mutex;
sem_t sem_client;
sem_t sem_barber;
int num_chairs;
int clientWait;

void *barber(void *param) {
   int worktime;
  
   while(1) {
      /* wait for a client to become available (sem_client) */
	  sem_wait(&sem_client);
      /* wait for mutex to access chair count (chair_mutex) */
	  sem_wait(&chairs_mutex);
      /* increment number of chairs available */
	  num_chairs += 1;
	  printf("Barber: Taking a client. Number of chairs available = %d\n",num_chairs);
      /* signal to client that barber is ready to cut their hair (sem_barber) */
	  sem_post(&sem_barber);
      /* give up lock on chair count */
	  sem_post(&chairs_mutex);
      /* generate random number, worktime, from 1-4 seconds for length of haircut.  */
	  worktime = (rand() % 4) + 1;
      /* cut hair for worktime seconds (really just call sleep()) */
	  printf("Barber: Cutting hair for %d seconds\n", worktime);
	  sleep(worktime);
    } 
}

void *client(void *param) {
   int waittime;

   while(1) {
      /* wait for mutex to access chair count (chair_mutex) */
	  sem_wait(&chairs_mutex);
      /* if there are no chairs */
	  if(num_chairs <= 0){
           /* free mutex lock on chair count */
		   printf("Client: Thread %u leaving with no haircut\n", (unsigned int)pthread_self());
		   sem_post(&chairs_mutex);
	  }
      /* else if there are chairs */
	  else{
           /* decrement number of chairs available */
		   num_chairs -= 1;
		   printf("Client: Thread %u Sitting to wait. Number of chairs left = %d\n",(unsigned int)pthread_self(),num_chairs);
           /* signal that a customer is ready (sem_client) */
		   sem_post(&sem_client);
           /* free mutex lock on chair count */
		   sem_post(&chairs_mutex);
           /* wait for barber (sem_barber) */
		   sem_wait(&sem_barber);
           /* get haircut */
		   printf("Client: Thread %u getting a haircut\n",(unsigned int)pthread_self());
	  }
      /* generate random number, waittime, for length of wait until next haircut or next try.  Max value from command line. */
	  waittime = (rand() % clientWait) + 1;
      /* sleep for waittime seconds */
	  printf("Client: Thread %u waiting %d seconds before attempting next haircut\n",(unsigned int)pthread_self(),waittime);
	  sleep(waittime);
     }
}

