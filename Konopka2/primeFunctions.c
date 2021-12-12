#include "primeFunctions.h"

/*
    Function Name: toCheck
    Input to the method: toCheck (The number we are checking if it is prime)
    Output(Return value): prime (Prime number if prime, or 0 if not)
    Brief description of the task: This will check if a number is prime, then return it if it is
*/
int checkPrimeAndPrint(unsigned long int toCheck){
      unsigned long int i = 2; 
      int prime = 1; 
      while (prime == 1&& i < toCheck/2){ 
              if (toCheck % i == 0) 
                prime = 0;
              i++;
      } 
      return (prime);
}

/*
    Function Name:sigHandler
    Input to the method: Signal
    Output(Return value): None 
    Brief description of the task: This is the signal handler for the children 
*/
void sigHandler(int signum){
    // look at what kind of signal we have so we can do the appropriate operation 
    switch(signum){
        case SIGTSTP:
          printf("Process %d: my PID is <%d>: I am suspending." , process_num, pid); 
          printf("Largest prime I found is %lu. \n", calculatedPrime);
          pause();
          break;
        case SIGCONT:
          printf("Process %d: my PID is <%d>: I am restarting.", process_num, pid); 
          printf("Largest prime I found is %lu.\n", calculatedPrime);
          break;
        case SIGTERM:
          printf("Process %d: my PID is <%d>: I am terminating.", process_num, pid); 
          printf("Largest prime I found is %lu. \n", calculatedPrime);
          exit(EXIT_SUCCESS);
          break;
        default:
          break;
    }
    

}

/*
    Function Name: bigranddom
    Input to the method: None 
    Output(Return value): Random long unsigned int
    Brief description of the task: Returns a random long unsigned int
*/
long unsigned int bigRandom() {
     time_t t;
     long unsigned int n = 0;
     srand((unsigned) time(&t));
     do
     {
         n += rand();
     } while(n < 1000000000);
     return n % 10000000000;
}
