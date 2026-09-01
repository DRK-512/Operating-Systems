#include "primeFunctions.h"

// This is the child that is created from the main.c function
int main(int argc, char *argv[]){

    // generate random number, and get the PID 
    calculatedPrime = bigRandom();
    process_num = (uint16_t)atoi(argv[1]);
    int numPrinted = 0;
    pid = getpid();

    // initalize the signal
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sigHandler;
    sigaction(SIGTSTP, &sa, NULL);
    sigaction(SIGCONT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    printf("Process %d: I just got started and my PID is: %d\n", process_num, pid); 
    printf("Process %d: I'll be finding the next prime number from: %d\n", process_num, calculatedPrime);
    u_int64_t numberToCheck = calculatedPrime +1; 

    // now we check if our number is prime 
    while (numPrinted <10) {
        if (checkPrimeAndPrint(numberToCheck) ==1)
            numPrinted++;
        numberToCheck++; 
    }   
}
