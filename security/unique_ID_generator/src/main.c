#include "../include/functions.h"

int main(int argc, char* argv[]) {
	char* cryptID; 
	int idSize=32;

	initParams(argc, argv, &idSize);
	
	if(assignID(&cryptID)) {
		exit(EXIT_FAILURE); 
	}

	printf("String used to encrypt: %s\n", cryptID);

	srand(createSeedFromID(cryptID)); 
	printf("Randomly Generated Clear-Text ID: ");
	// Now I use the random seed to generate random numbers and capital/lower-case letters
	int i=0, randomNum=0; 
	while (i<idSize) {
		randomNum = rand();
		// I want numbers and letters from ASCII table
		randomNum = 33 + randomNum % (122 - 48 + 1);
		if ((randomNum>47 && randomNum<58) || 
		(randomNum>64 && randomNum<91) ||
		(randomNum>96 && randomNum<123)) {

			if (i%4==0 && i>1) {
				printf("-");
			}
			printf("%c", randomNum); 
			i++;
		}
	}
	free(cryptID); 
	return 0;
}

