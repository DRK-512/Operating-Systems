#include "../include/philosopherFunc.h"

/* 
 * Description: 
 * Basically we have a max of 5 philosophers who sit at a round table. 
 * There also is only one chopstick in between each philosopher, and each
 * philosopher needs 2 chopsticks in order to eat their food. So we
 * simualte this senario with the use of semaphores 
 * NOTE: THIS IS AN INFINETE LOOP
 * THEY NEVER STOP EATING!
 */

int main() {
	int i;
	int philArr[maxPhil] = { 0, 1, 2, 3, 4 };
	pthread_t thread_id[maxPhil];

	// create threads and initalize the mutex
	sem_init(&mutex, 0, 1);

	for (i = 0; i < maxPhil; i++)
		sem_init(&philSem[i], 0, 0);

	for (i = 0; i < maxPhil; i++) {
		// create philosopher processes
		pthread_create(&thread_id[i], NULL, philosopher, &philArr[i]);
		printf("Philosopher %d is thinking\n", i + 1);
	}

	for (i = 0; i < maxPhil; i++) 
		pthread_join(thread_id[i], NULL);
	printf("All philosophers have stopped eating"); 
	return 0; 
}
