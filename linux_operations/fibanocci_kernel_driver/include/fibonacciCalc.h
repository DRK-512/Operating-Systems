#ifndef fibonacciCalc
#define fibonacciCalc

#include <gmp.h>
#include <stdio.h>

void fibonacciCalcLow(unsigned long long *result, unsigned int n);
void fibonacciCalcHigh(mpz_t result, unsigned int n);
int  countDigitsLow(unsigned long long int num);
int  countDigitsHigh(mpz_t num);
#endif
