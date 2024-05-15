#include "schedulerFunctions.h"

int main(int argc, char *argv[]) 
{
	struct sigaction sa;
	struct itimerval timer;
	
	FILE *in_file  = fopen(argv[1], "r");

	if (in_file == NULL) {
		printf("Error! Could not open file\n"); 
		exit(EXIT_FAILURE);
	} else {
		createData(in_file);  
	}

	// start timer at 1 second, don't want start time at 0
	timer.it_value.tv_sec = 1;
	timer.it_value.tv_usec = 0;

	// increament the timer by 1 sec
	timer.it_interval.tv_sec = 1;
	timer.it_interval.tv_usec = 0;

	setitimer(ITIMER_REAL, &timer, NULL);

	// setup the signal to be handled
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = &scheduleHandler;
	sigaction(SIGALRM, &sa, NULL);

	while(1);

}
