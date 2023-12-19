#ifndef fibonacciCalc
#define fibonacciCalc

#include <stdio.h>
#include <gmp.h>

void fibonacciCalcLow(unsigned long long* result, unsigned int n); 
void fibonacciCalcHigh(mpz_t result, unsigned int n); 

#endif

