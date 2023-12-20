#include <stdio.h>
#include "fibonacciCalc.h"

/*
* If you think this is not correct 
* reference up to 500: https://planetmath.org/listoffibonaccinumbers
*/
int main(int argc, int** argv) 
{
	printf("Enter Fibonacci number you want: ");
	int number; 
	
	if (scanf("%d", &number)) {
		// Overloading would be very nice right now
		if(number>93) {
			
			mpfr_t result; // [1] so technically * already
			printf("Calculating Fibonacci Number for %d ... \n", number);  
			fibonacciCalcHigh(result, number);
			mpfr_printf("Fibonacci Number of %d is: %.*Rf\n", number, 0, result);
			printf("The # digits is: %d", countDigitsHigh(result));  	
		} else {
			long long unsigned result; 
			printf("Fibonacci Number of %d: ", number); 
			fibonacciCalcLow(&result, number);
			printf("%llu\n", result);
			int length = countDigitsLow(result); 
			printf("The # digits is: %d", length);   
		}
	} else {
		printf("Invalid input. Please enter a number below 93 .\n");
	}
	return 0; 
}

