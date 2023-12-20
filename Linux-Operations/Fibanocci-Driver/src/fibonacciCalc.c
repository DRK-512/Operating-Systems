
#include "fibonacciCalc.h"

/*
 * The efficiency can be improved, but I kept it this way since the format
 * Of the functions shall be this way for the main.c
 * mpz > mprf because mprf is for floating point, mpz is easier to use for this application
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

void fibonacciCalcHigh(mpz_t result, unsigned int n) 
{
	int i;
	unsigned long long a = 0, b = 1, temp=0;
	mpz_t bA, bB, bTemp;
	if (n < 94) {
		printf("Input must be greater than 93"); 
		return; 
	} 
	
	// Due to overflows, we no longer use llu but mpz
	for (i = 2; i <= 93; i++) {
		temp = a + b;
		a = b;
		b = temp;
	}
	// Initialize variables
	mpz_init(bB);
	mpz_init_set_ui(bA, temp);
	mpz_init_set_ui(bTemp, a);
	mpz_add(bB, bA, bTemp); 

	for (i = 94; i <= n; i++) {
		mpz_add(bTemp, bA, bB);
		mpz_set(bA, bB);
		mpz_set(bB, bTemp);
	}

	mpz_set(result, bA);
	mpz_clears(bA, bB, bTemp, NULL);
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

// I could use mpz_sizeinbase, but fib(130) should be 27 # long, but it returns 28, so I want more accurate numbers
int countDigitsHigh(mpz_t num) 
{	
	int count = 0;

	while (mpz_cmp_ui(num, 0) > 0) {
		mpz_tdiv_q_ui(num, num, 10);
		count++;
	}

	return count;
}
