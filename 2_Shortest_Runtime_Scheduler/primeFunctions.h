#ifndef primeFunctions
#define primeFunctions

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>

long unsigned int bigRandom();
void sigHandler(int signum);
int checkPrimeAndPrint(unsigned long int toCheck);

extern long unsigned int calculatedPrime;
extern int process_num;
extern pid_t pid;

#endif



