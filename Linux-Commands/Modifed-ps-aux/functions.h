#ifndef functions
#define functions

#include <stdio.h>
#include <stdlib.h>
#include <string.h>			
#include <zconf.h>		
#include <getopt.h>

typedef struct commandline_inputs{
    int pid;
    int state;
    int program_time;
    int vmem;
    int cmdline;
}options;

void show_options(); 
void print_values(options opts);

#endif