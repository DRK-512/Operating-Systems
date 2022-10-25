#include "barberShop.h"

/*
Enchance this to work with 5 chairs to wait, and one barber
*/

#define waitChairs 10

int main(int argc, char *argv[]) {
	pthread_t barberid;
	pthread_t clientids[waitChairs];
	
	printf("Main thread beginning\n");
   /* 1. Get command line arguments */
   int runTime,clients,i;
   if (argc != 5){
	   printf("Please enter 4 arguments: <Program run time> <Number of clients>\n");
	   printf("<Number of chairs> <Client wait time>\n");
	   exit(0);
   }
   
   runTime = atoi(argv[1]);
   clients = atoi(argv[2]);
   num_chairs = atoi(argv[3]);
   clientWait = atoi(argv[4]);
   
   /* 2. Initialize semaphores */
   sem_init(&chairs_mutex,0,1);
   sem_init(&sem_client,0,0);
   sem_init(&sem_barber,0,0);
   
   /* 3. Create barber thread. */
   pthread_create(&barberid, NULL, barber, NULL);
   printf("Creating barber thread with id %lu\n",barberid);
   
   /* 4. Create client threads.  */
   for (i = 0; i < clients; i++){
	   pthread_create(&clientids[i], NULL, client, NULL);
	   printf("Creating client thread with id %lu\n",clientids[i]);
   }
   
   /* 5. Sleep. */
   printf("Main thread sleeping for %d seconds\n", runTime);
   sleep(runTime);
   
   /* 6. Exit.  */
   printf("Main thread exiting\n");
   exit(0);
}


