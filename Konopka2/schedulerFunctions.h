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

int totalTimeValue;
PIDData data[100];
int dataSize;
pid_t children[10];

