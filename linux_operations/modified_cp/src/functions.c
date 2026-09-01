#include "functions.h"

#include <errno.h>    // errno
#include <getopt.h>   // getopt_long & option
#include <limits.h>   // ‘INT_MAX’
#include <stdlib.h>   // free
#include <string.h>   // strchr & strlen
#include <sys/stat.h> // stat
#include <unistd.h>   // optarg

// I call help a few times in parse args, so I just made a helper function for it
static void print_help(const char *program_name) {
        printf("Usage: %s [OPTIONS]\n"
               "\n"
               "Copy a file one or more times.\n"
               "\n"
               "Required arguments:\n"
               "  -s, --src FILE          Source file to copy\n"
               "\n"
               "Options:\n"
               "  -n, --number NUM        Number of copies to create (default: 1)\n"
               "  -d, --dest DIR          Destination directory (default: .)\n"
               "  -v, --verbose           Show copy progress\n"
               "  -h, --help              Show this help message\n"
               "\n"
               "Examples:\n"
               "  %s -s file.txt\n"
               "  %s -s file.txt -n 5\n"
               "  %s -s file.txt -n 5 -d ./copies\n"
               "  %s --src file.txt --number 10 --dest ./backup --verbose\n",
               program_name, program_name, program_name, program_name, program_name);
}

// This is a helper script I wipped up to check if the nums being inputted are positive or not
// If they are positive and less than the max of an int, then we just set value
static int parse_positive_int(const char *str, int *value) {
        char *end;
        long  result;
        // strtol returns an errno so if there is an error this gets set
        errno  = 0;
        result = strtol(str, &end, 10);

        if (errno != 0 || end == str || *end != '\0') {
                return -1;
        }

        if (result <= 0 || result > INT_MAX) {
                return -1;
        }

        *value = (int)result;
        return 0;
}

// helper function to check if an input file exists
static int file_exists(const char *path) {
        struct stat info;
        return stat(path, &info) == 0 && S_ISREG(info.st_mode);
}

// helper function to check if an input directory exists
static int dir_exists(const char *path) {
        struct stat info;
        return stat(path, &info) == 0 && S_ISDIR(info.st_mode);
}

// helper function that gets num of digits, so 1235 would return 4
// I need it
static int get_num_digits(int num) {
        int count;
        if (num == 0) {
                return 1;
        }
        while (num != 0) {
                num /= 10;
                count++;
        }
        return count;
}

// copies a file a number of times with a progress as well
static int copy_single_file(const char *old_file, const char *new_file) {
        FILE *srcFile = fopen(old_file, "rb");
        if (srcFile == NULL) {
                perror("Error opening source file");
                return 1;
        }

        // figure out the size of the srcFile
        fseek(srcFile, 0, SEEK_END);
        long fileSize = ftell(srcFile);
        fseek(srcFile, 0, SEEK_SET);

        // create buffer to hold content of the file
        char *buffer = malloc(fileSize);
        if (buffer == NULL) {
                perror("Error allocating memory");
                fclose(srcFile);
                return 1;
        }
        // Read the entire file into the buffer
        fread(buffer, 1, fileSize, srcFile);
        fclose(srcFile);

        FILE *destFile = fopen(new_file, "wb");

        if (destFile == NULL) {
                perror("Error opening destination file");
                free(buffer);
                return 1;
        }

        // now write the buffer to the destination file
        fwrite(buffer, 1, fileSize, destFile);

        free(buffer);
        fclose(destFile);
        return 0;
}

// Here we init the values, basically I take in the inputs, and the return values I care about
// nCopies = number of copies to make
// verbose = shows the progress of copying these files
// srcPath = the name of the file you want to duplicate
// destPath = where the file will end up, the default is ./
int parse_args(int argc, char **argv, int *nCopies, int *verbose, char **srcPath, char **destPath) {
        int dispHelp = 0, option = 0;
        // I usually do this manually, but option is not a bad idea
        static struct option long_options[] = {
            {"number", required_argument, 0, 'n'}, {"verbose", no_argument, 0, 'v'}, {"src", required_argument, 0, 's'},
            {"dest", required_argument, 0, 'd'},   {"help", no_argument, 0, 'h'},    {0, 0, 0, 0}};
        // Loop through input options I listed above
        while ((option = getopt_long(argc, argv, "n:vs:d:h", long_options, NULL)) != -1) {
                switch (option) {
                        case 'n':
                                // pass in the parsed arguemnt, and if its good set it to nCopies
                                if (parse_positive_int(optarg, nCopies) != 0) {
                                        fprintf(stderr, "Error: invalid number of copies: '%s'\n", optarg);
                                        return 1;
                                }
                                break;

                        case 'v':
                                *verbose = 1;
                                break;

                        case 's':
                                *srcPath = strdup(optarg);
                                if (*srcPath == NULL) {
                                        perror("strdup");
                                        return 1;
                                }
                                break;

                        case 'd':
                                *destPath = strdup(optarg);
                                if (*destPath == NULL) {
                                        perror("strdup");
                                        return 1;
                                }
                                break;

                        case 'h':
                                print_help(argv[0]);
                                return 1;

                        case '?':
                        default:
                                print_help(argv[0]);
                                return 1;
                }
        }

        // Obviously if there is no srcPath or dest path then we should error this out
        if (*srcPath == NULL) {
                fprintf(stderr, "Error: source file is required.\n\n");
                print_help(argv[0]);
                return 1;
        }
        
        // A weird one off I found where if -d . is done then everything is hidden
        if ((*destPath == NULL) || (strcmp(*destPath, ".") == 0)) {
                *destPath = strdup("./");
                if (*destPath == NULL) {
                        perror("strdup");
                        return 1;
                }
        }

        // Now I check if the two exist (if they don't this should fail)
        if (!file_exists(*srcPath)) {
                fprintf(stderr, "Error: source is not a regular file: %s\n", *srcPath);
                return 1;
        }

        if (!dir_exists(*destPath)) {
                fprintf(stderr, "Error: destination is not a directory: %s\n", *destPath);
                return 1;
        }

        return 0;
}

// this will take srcPath and split from the first '.'
// so if srcPath = file.old.txt then
// pre extension = file
// post extension = .old.txt
static void split_file(char *filename, char **pre_ext, char **post_ext) {
        const char *dot = strrchr(filename, '.');

        if (dot == NULL) {
                *pre_ext = strdup(filename);
                *post_ext = strdup("");
                return;
        }

        size_t pre_len = (size_t)(dot - filename);

        *pre_ext = malloc(pre_len + 1);
        if (*pre_ext == NULL) {
                *post_ext = NULL;
                return;
        }

        memcpy(*pre_ext, filename, pre_len);
        (*pre_ext)[pre_len] = '\0';

        *post_ext = strdup(dot);

        if (*post_ext == NULL) {
                free(*pre_ext);
                *pre_ext = NULL;
        }
}

int copy_files(const char *srcPath, const char *destPath, int nCopies, int verbose) {
        char *pre_ext  = NULL;
        char *post_ext = NULL;
        // if the input is ./filename then filename is the return
        char *filename = strrchr(srcPath, '/');

        if (filename != NULL) {
                filename++;
        } else {
                filename = (char*)srcPath;
        }

        split_file(filename, &pre_ext, &post_ext);

        if (pre_ext == NULL) {
                fprintf(stderr, "Error: failed to split filename\n");
                return 1;
        }

        size_t dest_path_size = strlen(destPath);
        size_t pre_ext_size   = strlen(pre_ext);
        size_t post_ext_size  = strlen(post_ext);

        for (int i = 1; i <= nCopies; i++) {
                int num_i = get_num_digits(i);

                size_t file_size = dest_path_size + pre_ext_size + num_i + post_ext_size + 1;

                char *new_file = malloc(file_size);

                if (new_file == NULL) {
                        perror("malloc");
                        free(pre_ext);
                        return 1;
                }

                snprintf(new_file, file_size, "%s%s%d%s", destPath, pre_ext, i, post_ext);

                if (copy_single_file(srcPath, new_file) != 0) {
                        free(new_file);
                        free(pre_ext);
                        return 1;
                }

                if (verbose) {
                        printf("Created %s from %s\n", new_file, srcPath);
                }

                free(new_file);
        }

        free(pre_ext);

        return 0;
}
