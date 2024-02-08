#include "../include/functions.h"

int main(int argc, char *argv[]) {
	
	if (initVars(argc, argv)) {
		exit(-1); 
	} 
	process(); 
	return 0;
} 
