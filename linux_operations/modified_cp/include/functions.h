#ifndef FUNCTIONS
#define FUNCTIONS

#include <stdio.h>

int  parse_args(int argc, char **argv, int *nCopies, int *verbose, char **srcPath, char **destPath);
int copy_files(const char *srcPath, const char *destPath, int nCopies, int verbose);

#endif