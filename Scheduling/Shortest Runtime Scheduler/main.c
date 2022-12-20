#include "schedulerFunctions.h"

/*
    Description of the program: The program was designed to simulate a 
    shortest remaining time first scheduler, using parameters from an input file, 
    and the prime.c main function as a child process
*/
int main(int argc, char *argv[]){ 

    // Get the input file 
	FILE *in_file  = fopen(argv[1], "r"); // read only 

	// Test for files not existing. 
  	if (in_file == NULL) {   
        // The file does not exist
		printf("Error! Could not open file\n"); 
  		exit(-1); // must include stdlib.h 
    } else {
    	// Take data from input file and put into data struct
        createData(in_file);  
    }

    // Set up timer
    struct itimerval timer;
    // Install timer_handler as the signal handler for SIGALRM. 

    // The timer goes off 1 second after installation of the timer. 
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec =0;

    // and every 1 second after that. 
    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec =0;
    
    // Start a reak timer (It counts down whenever this process is executing) 
    setitimer (ITIMER_REAL, &timer, NULL);
    // Set up handler 
    struct sigaction sa;
    // Install timer_handler as the signal handler for SIGALRM. 
    memset (&sa, 0, sizeof (sa));
    sa.sa_handler = &scheduleHandler;
    sigaction (SIGALRM, &sa, NULL);

    while(1);

}
