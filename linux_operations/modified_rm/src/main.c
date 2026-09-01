#include "functions.h"

int main(int argc, char *argv[]) {
        if (argc != 3) {
                printf("This program is meant to be a more secure way to rm "
                       "files\n");
                printf("This is done by finding out how much memory the file "
                       "takes up,\n");
                printf("Then overwritting it with random data, and only then "
                       "running the "
                       "remove function on it\n");
                printf("Usage: %s <filename> <passes>\n", argv[0]);
                return 1;
        }

        const char *filename = argv[1];
        int         passes   = atoi(argv[2]);

        if (passes <= 0) {
                printf("ERROR: Number of passes must be a positive integer\n");
                return 1;
        }

        // Make the rand seed based on current time so it is always different
        srand((unsigned)time(NULL));

        if (!secureDelete(filename, passes)) {
                printf("File '%s' securely deleted with %d passes.\n", filename, passes);
        } else {
                printf("Failed to securely delete file '%s'\n", filename);
        }

        return 0;
}
