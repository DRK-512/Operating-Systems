#include "schedulerFunctions.h"

int main(int argc, char *argv[]) { 

	// Check if the input file exists, if so, grab its data
    FILE *in_file  = fopen(argv[1], "r");
  	if (in_file == NULL) {   
		printf("Error! Could not open file\n"); 
  		exit(-1); 
    } else {
        createData(in_file);  
    }

    struct itimerval timer;

    // timer goes off 1 second after installation of the timer. 
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;

    // timer increments 1 sec at a time 
    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;

    // this will count down whenever this process is executing
    setitimer(ITIMER_REAL, &timer, NULL);

    // setup the signal to be handled
    struct sigaction sa;
    memset(&sa, 0, sizeof (sa));
    sa.sa_handler = &scheduleHandler;
    sigaction(SIGALRM, &sa, NULL);

    // The threads will exit
    while(1);

}
