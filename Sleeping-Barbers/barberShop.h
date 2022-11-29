#ifndef barberShop
#define barberShop

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

extern sem_t chairs_mutex;
extern sem_t sem_customer;
extern sem_t sem_barber;
extern int num_chairs;
extern int haircutTime;

void *barber(void *param); 
void *customer(void *threadid); 
#endif
