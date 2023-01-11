#ifndef barberShop
#define barberShop

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

extern sem_t chairs_mutex;
extern sem_t sem_customer;
extern sem_t sem_barber;
extern uint8_t num_chairs;
extern uint8_t haircutTime;

void *barber(void *param); 
void *customer(void *param); 
#endif
