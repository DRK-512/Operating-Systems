
#include "fibonacciCalc.h"

/*
 * The efficiency can be improved, but I kept it this way since the format
 * Of the functions shall be this way for the main.c
 */
 
// Here I calculate the n-th Fibonacci number
void fibonacciCalcLow(unsigned long long* result, unsigned int n) 
{
	// 94 -> 19740274219868223167 > 18446744073709551615 unsigned long long max
	if (n <= 1 || n > 93) {
		printf("Input must be greater than 0, or less than 94"); 
		return; 
	}

	int i;
	unsigned long long a = 0, b = 1, temp=0;

	for (i = 1; i <= n; i++) {
		temp = a + b;
		a = b;
		b = temp;
	}

	*result=a; 
	return; 
}

void fibonacciCalcHigh(mpfr_t result, unsigned int n) 
{
	if (n < 94) {
		printf("Input must be greater than 93"); 
		return; 
	} 
	
	// Due to overflows, we no longer use llu but mpz
	int i;
	unsigned long long a = 0, b = 1, temp=0;

	for (i = 2; i <= 93; i++) {
		temp = a + b;
		a = b;
		b = temp;
	}

	mpfr_t bA, bB, bTemp;
	
	// as numbers increase, accuracy decreases
	if (n<500) {
		mpfr_init2(bA, 1024); // Set precision to 1024 bits
		mpfr_init2(bB, 1024); // Set precision to 1024 bits
		mpfr_init2(bTemp, 1024); // Set precision to 1024 bits
		mpfr_init2(result, 1024); // Set precision to 1024 bits
	} else {
		mpfr_init2(bA, n*2); // Set precision to 1024 bits
		mpfr_init2(bB, n*2); // Set precision to 1024 bits
		mpfr_init2(bTemp, n*2); // Set precision to 1024 bits
		mpfr_init2(result, n*2); // Set precision to 1024 bits
	}
	
	mpfr_set_ui(bA, temp, MPFR_RNDN);
	mpfr_set_ui(bTemp, a, MPFR_RNDN);
	mpfr_add(bB, bA, bTemp, MPFR_RNDN); 

	for (i = 94; i <= n; i++) {
		mpfr_add(bTemp, bA, bB, MPFR_RNDN);
		mpfr_set(bA, bB, MPFR_RNDN);
		mpfr_set(bB, bTemp, MPFR_RNDN);
	}

	mpfr_set(result, bA, MPFR_RNDN);
	mpfr_clear(bA);
	mpfr_clear(bB);
	mpfr_clear(bTemp);
	return; 
}

int countDigitsLow(unsigned long long int num) 
{
	unsigned int count=0; 
	do {
		num /= 10;
		++count;
	} while (num != 0);
	printf("\nCount is: %d", count); 
	return count; 
}

// I could use mpz_sizeinbase, but fib(130) should be 27 # long, but mpz_size returns 28, so I want more accurate numbers
int countDigitsHigh(mpfr_t num) 
{	
	int count=0;  
	while (mpfr_cmp_d(num, 1.0) >= 0) {
		mpfr_div_ui(num, num, 10, MPFR_RNDN);
		count++;
	}
	return count;
}
