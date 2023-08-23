#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void copyFile(const char *srcPath, const char *destPath, int copyNumber, int showProgress) {
    FILE *srcFile = fopen(srcPath, "rb");
    if (srcFile == NULL) {
        perror("Error opening source file");
        exit(EXIT_FAILURE);
    }

    fseek(srcFile, 0, SEEK_END);
    long fileSize = ftell(srcFile);
    fseek(srcFile, 0, SEEK_SET);

    FILE *destFile = fopen(destPath, "wb");
    if (destFile == NULL) {
        perror("Error opening destination file");
        fclose(srcFile);
        exit(EXIT_FAILURE);
    }

    int ch;
    long bytesCopied = 0;
    while ((ch = fgetc(srcFile)) != EOF) {
        fputc(ch, destFile);
        bytesCopied++;

        if (showProgress) {
            float progress = (float)bytesCopied / fileSize * 100;
            printf("\rProgress: %.2f%%", progress);
            fflush(stdout);
        }
    }

    fclose(srcFile);
    fclose(destFile);

    if (showProgress) {
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    int numCopies = 1;
    int showProgress = 0;
    int srcIndex = -1;
    int destIndex = -1;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0) {
            if (i + 1 < argc) {
                numCopies = atoi(argv[i + 1]);
                i++;
            }
        } else if (strcmp(argv[i], "-s") == 0) {
            showProgress = 1;
        } else if (srcIndex == -1) {
            srcIndex = i;
        } else if (destIndex == -1) {
            destIndex = i;
        }
    }

    if (srcIndex == -1 || destIndex == -1) {
        fprintf(stderr, "Usage: %s [-n num_copies] [-s] <source_file> <destination_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *srcPath = argv[srcIndex];
    const char *destPath = argv[destIndex];

    char extension[10] = "";
    const char *dot = strrchr(destPath, '.');
    if (dot != NULL) {
        strcpy(extension, dot);
    }

    for (int i = 1; i <= numCopies; i++) {
        char copyNumberSuffix[10] = "";
        if (i > 1) {
            snprintf(copyNumberSuffix, sizeof(copyNumberSuffix), "(%d)", i - 1);
        }

        char newDestPath[1000];
        snprintf(newDestPath, sizeof(newDestPath), "%.*s%s%s", (int)(dot - destPath), destPath, copyNumberSuffix, extension);

        copyFile(srcPath, newDestPath, i, showProgress);
        printf("Copy %d of %d done.\n", i, numCopies);
    }

    printf("File copied successfully.\n");

    return EXIT_SUCCESS;
}

