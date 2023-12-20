#include <stdio.h>

/*
 * overflow of unsinged long long is bound to happen
 * so we instead have a overflow counter
 * The reason for this and not using gmp is because gmp is a GNU library 
 * AKA you cant link it in kernel code
 */

struct overLong_t {
	unsigned long long overCount;
	unsigned long long remainder;
};

// Here is the arithmetic function I use to add two numbers from bigNum_t
struct overLong_t addBigNums(struct overLong_t a, struct overLong_t b) 
{
	struct overLong_t result = {0, 0};  // Default values

	result.remainder = a.remainder + b.remainder;

	// Check for overflow
	if (result.remainder < a.remainder || result.remainder < b.remainder) {
		result.overCount = 1;
	}

	result.overCount += a.overCount + b.overCount;

	// Adjust remainder if overflow occurred
	if (result.overCount > 0) {
		result.remainder -= (unsigned long long)(-1); // subtract max value to get correct remainder
	}

	return result;
}

// returns the fibonacci number with remainder and overflow counter
struct overLong_t fibonacci(unsigned long long n) 
{
	// initalize
	unsigned long long i; 
	struct overLong_t a = {0, 0};
	struct overLong_t b = {0, 1};
	struct overLong_t temp;

	// no need to calc such small numbers
	if (n == 0) {
		return a;
	} else if(n <= 2) {
		return b; 
	}

	for (i = 2; i <= n; ++i) {
		temp = b;
		b = addBigNums(a, b);
		a = temp;
	}

	return b;
}

int main() {
	unsigned long long index;

	// Get user input
	printf("Enter the index of the Fibonacci sequence: ");
	scanf("%llu", &index);

	// Calculate Fibonacci number
	struct overLong_t result = fibonacci(index);

	// Display the result
	if (result.overCount == 0) {
		printf("Fibonacci number at index %llu: %llu\n", index, result.remainder);
	} else {
		printf("Fibonacci number at index %llu: %llu + 18446744073709551615 * %llu\n", index, result.remainder, result.overCount);
		printf("18446744073709551615 is the max of unsinged long long, so overflow will occur here"); 
	}
	return 0;
}
