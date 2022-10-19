#include "philosopherFunc.h"

/* 
   This is the main function
   Basically we have a max of 5 philosophers who sit at a round table, 
   and there is one chopstick in between each one, but a philosopher 
   needs 2 in order to eat their food
*/
int main() {
 
	// create threads and initalize the mutex
	pthread_t thread_id[N];
	sem_init(&mutex, 0, 1);

	// 
	int i;
	for (i = 0; i < N; i++){
		sem_init(&S[i], 0, 0);
	}

	for (i = 0; i < N; i++) {
	  // create philosopher processes
	  pthread_create(&thread_id[i], NULL, philosopher, &phil[i]);
	  printf("Philosopher %d is thinking\n", i + 1);
	}

	for (i = 0; i < N; i++) {
		pthread_join(thread_id[i], NULL);
	}
}
