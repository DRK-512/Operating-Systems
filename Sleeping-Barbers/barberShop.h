#ifndef barberShop
#define barberShop

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

extern sem_t chairs_mutex;
extern sem_t sem_client;
extern sem_t sem_barber;
extern int num_chairs;
extern int clientWait;

void *barber(void *param); 
void *client(void *param); 
#endif
