#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
 * The reason for this and not using gmp is because gmp is a GNU library 
 * AKA you cant link it in kernel code
 */

// Here is the arithmetic function I use to add two numbers 
char* addStrings(char* num1, char* num2, char* result) 
{
	unsigned long long len1 = strlen(num1), len2 = strlen(num2);
	unsigned long long rS = len2+1; 
	unsigned int diff=0, i, sum=0, carry = 0, digit1=0, digit2=0;

	// in case len1<len2, this will cuase issues later
	if(len1!=len2)
		diff=1; 

	// Traverse both strings from right to left
	for (i = rS-1; i > 0; i--) {
		
		if((i-diff)>0){
			digit1 = num1[i-diff-1] - '0';
			digit2 = num2[i-1] - '0';
		} else {
			digit1 = 0;
			digit2 = num2[i-1] - '0';
		}

		sum = digit1 + digit2 + carry;
		
		// sum can only be a single digit, so that is why we have the carry
		if(sum>9) {
			sum-=10; 
			carry=1; 
		} else {
			carry=0; 
		}

		if(i!=1){
			result[i] = sum + '0';
		} else {
			result[1] = sum + '0';
			if(carry==0){
				memmove(result, result + 1, rS);
				rS--; 
			} else {
				result[0] = carry + '0';
			}
		}
	}
	return result; 
}

char* my_strdup(const char* str) 
{
    size_t len = strlen(str) + 5;
    char* new_str = (char*)malloc(len);
    if (new_str != NULL) {
        strcpy(new_str, str);
    }
    return new_str;
}

// returns the fibonacci number with remainder and overflow counter
char* fibonacci(unsigned long long n) 
{
	// initalize
	unsigned long long i; 
	char *a, *b, *temp;  

	// no need to calc such small numbers
	if (n == 0)
		return "0"; 
	else if(n <= 2) 
		return "1"; 

	a = my_strdup("0");
	b = my_strdup("1");
	temp = (char*)malloc(2);
	
	for (i = 2; i <= n; i++) {
		// always reallocate before assignment
		free(temp); 
		temp = addStrings(a, b, (char *)calloc(strlen(b)+2, sizeof(char)));

		free(a);
		a = my_strdup(b);

		free(b);
		b = my_strdup(temp);

	}
	free(a); 
	free(temp); 
	return b;
}

int main() 
{
	unsigned long long index;

	// Get user input
	printf("Enter the index of the Fibonacci sequence: ");
	scanf("%llu", &index);

	// Calculate Fibonacci number
	char* result = fibonacci(index);

	// Display result (example)
	printf("%s ", result);
	free(result); 
	return 0; 
}

