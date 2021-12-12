#include "primeFunctions.h"

/*
    Function Name: main.c
    Input to the method:PID
    Output(Return value): prime number
    Brief description of the task: This is the child that is created from the main.c function
*/

int main(int argc, char** argv){
    // first we get the random number, and get the PID 
    calculatedPrime = bigRandom();
    process_num = atoi(argv[1]);
    int numPrinted = 0;
    pid = getpid();

    struct sigaction sa;
    memset (&sa, 0, sizeof(sa));
    sa.sa_handler = sigHandler;
    sigaction (SIGTSTP, &sa, NULL);
    sigaction (SIGCONT, &sa, NULL);
    sigaction (SIGTERM, &sa, NULL);

    printf("Process %d: my PID is <%d>: I just got started.", process_num, pid); 
    printf("I am starting with the number %lu to find the next prime number.\n", calculatedPrime);
    long unsigned int numberToCheck = calculatedPrime +1; 
    // now we check if our number is prime 
    while (numPrinted <10) {
        if (checkPrimeAndPrint(numberToCheck) ==1){ 
            numPrinted++;
       }
       numberToCheck++; 
    } 
        
}
