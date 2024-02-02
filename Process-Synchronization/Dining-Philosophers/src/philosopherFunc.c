#include "../include/philosopherFunc.h"
 
int state[maxPhil];
 
sem_t mutex;
sem_t philSem[maxPhil];
 
// checks if there are chopsticks avaliablke so the philospher can eat
void test(int philNum) {
	if (state[philNum] == hung && state[left] != eat && state[right] != eat) {
		state[philNum] = eat;
		sleep(EAT_TIME);

		sem_post(&philSem[philNum]);
		printf("Philosopher %d takes chopsticks %d and %d\n", philNum + 1, left + 1, philNum + 1); 
		printf("Philosopher %d is now Eating\n", philNum + 1);
	}
}

void take_chopstick(int philNum) {
    sem_wait(&mutex);
 
    // now that the philosopher is no longer thinking, they are now hungry
    state[philNum] = hung;

    printf("Philosopher %d is Hungry\n", philNum + 1);
 
    // check if we can eat
    test(philNum);
 
    sem_post(&mutex);
 
    // if we cant eat, wait until chopsticks are availble 
    sem_wait(&philSem[philNum]);
}
 
// put down chopsticks
void place_chopstick(int philNum) {

	sem_wait(&mutex);

	// we are now placing our chopsticks to think
	state[philNum] = think;

	printf("Philosopher %d putting chopsticks %d and %d down\n", philNum + 1, left + 1, philNum + 1);
	printf("Philosopher %d is thinking\n", philNum + 1);

	// let left are right know chopsticks are availble
	test(left);
	test(right);

	sem_post(&mutex);
}
 
// This is the main thread for the philosophers
void* philosopher(void* num) {
	int* i = num;
	int eatCount=0; // you can always remove it and make it a while(1) for infitine eating
	while (eatCount<allowedBites) {
		// by default all philosophers are in the think state
		sleep(THINK_TIME); 
		take_chopstick(*i);
		// sleep(1); // ? why are we sleeping here?
		sleep(THINK_TIME);
		place_chopstick(*i);
		eatCount++; 
	}
	printf("Philosopher %d is no longer allowed to eat\n", *i+1); 
}
