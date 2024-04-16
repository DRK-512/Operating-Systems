#ifndef functions
#define functions

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h> // strlen | strcpy | strcat
#include <getopt.h> // optarg
#include <unistd.h> // getopt_long

int assignID(char** m_cryptID); 
unsigned int createSeedFromID(char* input_ID); 
void uuid_form(char* output_uuid, char* input_string); 
int initParams(int argc, char* argv[], int* idSize);

#endif
