#ifndef philosopherFunc
#define philosopherFunc

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
 
#define N 5
#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define LEFT (phnum + 4) % N
#define RIGHT (phnum + 1) % N

#endif
