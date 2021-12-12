
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>

long unsigned int bigRandom();
long unsigned int calculatedPrime;
void sigHandler(int signum);
int process_num;
pid_t pid;
int checkPrimeAndPrint(unsigned long int toCheck);