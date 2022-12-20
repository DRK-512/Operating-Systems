#ifndef schedulerFunctions
#define schedulerFunctions

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>


typedef struct inputData
{
	int PID; 
	int AT; 
	int Burst; 
}PIDData;


void createData(FILE* file);
void suspendChild(pid_t child);
void resumeChild(pid_t child);
void createChild(int new_process);
void scheduleHandler(int signum);
PIDData findSmallestPID();
void checkBurst();

extern int totalTimeValue;
extern PIDData data[100];
extern int dataSize;
extern pid_t children[10];

#endif
