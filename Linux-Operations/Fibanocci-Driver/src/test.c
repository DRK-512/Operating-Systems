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
			mpz_t result; 
			printf("Fibonacci Number of %d: ", number);  
			fibonacciCalcHigh(result, number);
			gmp_printf("%Zd", result);  	
		} else {
			long long unsigned result; 
			printf("Fibonacci Number of %d: ", number); 
			fibonacciCalcLow(&result, number);
			printf("%llu", result);  
		}
	} else {
		printf("Invalid input. Please enter a number below 93 .\n");
	}
	
	return 0; 
}

