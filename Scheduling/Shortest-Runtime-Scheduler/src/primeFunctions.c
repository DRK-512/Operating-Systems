#include "primeFunctions.h"

uint32_t calculatedPrime;
uint16_t process_num;
pid_t pid;

// This will check if a number is prime, then return it if it is
uint8_t checkPrimeAndPrint(uint32_t toCheck) {
    unsigned long int i = 2; 
    int prime = 1; 
    while (prime == 1&& i < toCheck/2){ 
        if (toCheck % i == 0) 
            prime = 0;
        i++;
    } 
    return (prime);
}

// signal handler for child processes 
void sigHandler(int signum){
    // first figure out which signal we are working with, then process it
    switch(signum) {
        case SIGTSTP:
            printf("Process %d: my PID is <%d>, and I am suspending.\n" , process_num, pid); 
            printf("Largest prime I found was %u. \n", calculatedPrime);
            pause();
            break;
        case SIGCONT:
            printf("Process %d: my PID is <%d>, and I am restarting.\n", process_num, pid); 
            printf("Largest prime I found was %u.\n", calculatedPrime);
            break;
        case SIGTERM:
            printf("Process %d: my PID is <%d>, and I am terminating.\n", process_num, pid); 
            printf("Largest prime I found was %u. \n", calculatedPrime);
            exit(EXIT_SUCCESS);
            break;
        default:
            break;
    }
}

uint32_t bigRandom() {
    time_t t;
    uint32_t n = 0;
    srand((unsigned) time(&t));
    while(n < 1000000000)
        n += rand();

    return n % 10000000000;
}
