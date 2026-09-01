#include "functions.h"

int main(int argc, char *argv[]) {

        int   nCopies = 1, verbose = 0;
        char *srcPath = NULL, *destPath = NULL;

        if (parse_args(argc, argv, &nCopies, &verbose, &srcPath, &destPath)) {
                return 1;
        }

        printf("Now copying %s to %s %d time(s)\n", srcPath, destPath, nCopies);

        if(copy_files(srcPath, destPath, nCopies, verbose)){
                return 1;
        }

        printf("Files copied successfully.\n");

        return 0;
}
