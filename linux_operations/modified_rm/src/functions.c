
#include "functions.h"

long getFileSize(FILE *file) {
        fseek(file, 0L, SEEK_END);
        long size = ftell(file);
        fseek(file, 0L, SEEK_SET);
        return size;
}

// this overrites the file with random data
int overwriteFile(FILE *file, long size, int passes) {
        unsigned char *randomData = malloc(size);
        if (randomData == NULL) {
                perror("ERROR: Failed to allocate memory for random data\n");
                return 1;
        }
        for (int pass = 0; pass < passes; pass++) {
                // Generate random data
                for (long i = 0; i < size; i++) {
                        randomData[i] = rand() % 256;
                }
                // Write random data to the file
                fwrite(randomData, 1, size, file);
                fflush(file);
                fseek(file, 0L,
                      SEEK_SET); // Reset the file position to the beginning for
                                 // the next pass
        }

        free(randomData);
        return 0;
}

// here we open the file, and overwrite it
int secureDelete(const char *filename, int passes) {
        // Open the file in binary mode to ensure we have the exact data without
        // modifications
        FILE *file = fopen(filename, "rb+");
        if (file == NULL) {
                perror("ERROR: Failed to open file");
                return 1;
        }

        long size = getFileSize(file);

        // Overwrite the file with random data multiple times
        if (overwriteFile(file, size, passes)) {
                return 1;
        }

        // close then delete the file
        fclose(file);
        if (remove(filename) != 0) {
                perror("ERROR: Failed to delete file");
                return 1;
        }

        return 0;
}
