#ifndef primeFunctions
#define primeFunctions

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>

uint32_t bigRandom();
void sigHandler(int signum);
uint8_t checkPrimeAndPrint(uint32_t toCheck); 

extern uint32_t calculatedPrime;
extern uint16_t process_num;
extern pid_t pid;

#endif



