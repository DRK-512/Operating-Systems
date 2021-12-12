/*
    Author: Darek Konopka & Tejas Bolar
    Assignment Number: 1
    Date of Submission: 10/20/2021
    Name of this file:5ps.c
    Description of the program: This program is a simple ps program wil 5 arguments
 */
 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>		// for strcat
#include <zconf.h>		// for sysconf
#include "parse_help.h"

/*
   Function Name: main
   Input to the method: There are none
   Output(Return value): There are none
   Brief description of the task: This will show the options if the user types in an incorrect argument
*/
int main (int argc, char *argv[])
{
    options opts = get_set_options (argc, argv);
    print_values(opts);
    return 0;
}
