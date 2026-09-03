#ifndef FUNCTIONS
#define FUNCTIONS

#include <stdio.h>

#define COPY_BUFFER_SIZE (64 * 1024)
#define MAX_COPY_COUNT 1000

int parse_args(int argc, char **argv, int *nCopies, int *verbose, char **srcPath, char **destPath);
int copy_files(const char *srcPath, const char *destPath, int nCopies, int verbose);

#endif