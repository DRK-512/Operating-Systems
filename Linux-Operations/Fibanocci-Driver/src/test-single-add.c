#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void addStrings(char num1[], char num2[]) {
	unsigned long long len1 = strlen(num1), len2 = strlen(num2);
	unsigned long long rS = len2+1; 
	char* result = (char *)calloc(rS, sizeof(char));

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
		
	for(i=0; i<rS; i++) 
		printf("%c", result[i]);

	free(result); 
}

int main() {
	// Declare and initialize two strings representing numbers
	char num1[] =  "10284720757613717413913";
	char num2[] = "166410277506205636620961";

	// Print the result
	printf("Sum: "); 
	addStrings(num1, num2);

	return 0;
}

