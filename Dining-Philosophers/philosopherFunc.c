#include "philosopherFunc.h"
 
int state[maxPhil];
 
sem_t mutex;
sem_t philSem[maxPhil];
 
// philNum is which philosopher we are taking about now
void test(int philNum) {
    // check if chopsticks are available
    if (state[philNum] == hung && state[left] != eat && state[right] != eat) {
        // since chopsticks are avalibale, the philospher will start eating
        state[philNum] = eat;
        sleep(2);

        sem_post(&philSem[philNum]);
        printf("Philosopher %d takes chopsticks %d and %d\n", philNum + 1, left + 1, philNum + 1); 
        printf("Philosopher %d is now Eating\n", philNum + 1);
    }
}
 
// pickup chopsticks
void take_chopstick(int philNum) {
    
    sem_wait(&mutex);
 
    // once mutex done waiting, the philosopher is hungry
    state[philNum] = hung;

    printf("Philosopher %d is Hungry\n", philNum + 1);
 
    // check if we can eat
    test(philNum);
 
    sem_post(&mutex);
 
    // if we cant eat, wait until chopsticks are availble 
    sem_wait(&philSem[philNum]);
 
    sleep(1);
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
 
// this is the philosopher who takes the chopsitck, eats, then places the chopstick
void* philosopher(void* num) {
    while (1) {
        int* i = num;
        sleep(1);
        take_chopstick(*i);
        sleep(0);
        place_chopstick(*i);
    }
}
