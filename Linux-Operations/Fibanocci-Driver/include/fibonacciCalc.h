#ifndef fibonacciCalc
#define fibonacciCalc

#include <mpfr.h>
#include <stdlib.h>
#include <string.h>
void fibonacciCalcLow(unsigned long long* result, unsigned int n); 
void fibonacciCalcHigh(mpfr_t result, unsigned int n); 
int countDigitsLow(unsigned long long int num);
int countDigitsHigh(mpfr_t num);
#endif

