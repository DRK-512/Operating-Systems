#ifndef functions
#define functions

#include <stdio.h>
#include <stdint.h> // For unit8

// RAM is simulated by an array of 2^7 (128) bytes, or (16 * 8) bytes
uint16_t ram = [8];

// Page size & frame size are 2^4 (16) bytes
// NOTE: page size is always equal to frame size
uint8_t page_size = 16;

// Number of pages in the process is 32 (512/16)
int page_table = [32];

typedef struct {
        const char* address_path;
        const char* process_path;
        int help;
} arguments;

// Used to hold info needed for each page
typedef struct pages {
        int  pageNumber;
        char values[page_size];
        int  notUsedCycles;
} pages;

arguments parse_args(int argc, char*argv[]);
uint8_t check_args(arguments args);

void main_loop(arguments address_file, arguments process_file);

#endif
