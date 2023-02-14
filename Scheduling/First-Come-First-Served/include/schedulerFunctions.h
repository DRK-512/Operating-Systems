#ifndef schedulerFunctions
#define schedulerFunctions

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>

typedef struct inputData
{
	uint16_t PID;
	int16_t AT; 
	int16_t Burst; 
}PIDData;

void createData(FILE* file);

void resumeChild(pid_t child);
void suspendChild(pid_t child);
void terminateChild(pid_t child); 

void createChild(uint16_t new_process);
void scheduleHandler(int signum);

PIDData findNextPID();
void checkBurst(); 

extern uint8_t dataSize;
extern uint8_t totalTimeValue;

extern PIDData data[100];
extern pid_t children[10];

#endif
