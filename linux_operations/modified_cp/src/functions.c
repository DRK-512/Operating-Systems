#include "functions.h"

#include <errno.h>    // errno
#include <getopt.h>   // getopt_long & option
#include <stdlib.h>   // free
#include <string.h>   // strchr & strlen and a bunch more
#include <sys/stat.h> // stat
#include <unistd.h>   // optarg
#include <fcntl.h>    // O_EXCL

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
        // strtol returns an errno so if there is an error this gets set
        errno  = 0;
        long result = strtol(str, &end, 10);

        if (errno != 0 || end == str || *end != '\0') {
                return -1;
        }

        if (result <= 0 || result > MAX_COPY_COUNT) {
                return -1;
        }

        *value = (int)result;
        return 0;
}

// helper function to check if an input file exists
static int file_exists(const char *path) {
        struct stat info;
        return (stat(path, &info) == 0 && S_ISREG(info.st_mode));
}

// helper function to check if an input directory exists
static int dir_exists(const char *path) {
        struct stat info;
        return (stat(path, &info) == 0 && S_ISDIR(info.st_mode));
}

// helper function for the '.' and '/' input one-off issues I was getting
static int has_trailing_slash(const char *path) {
        size_t len = strlen(path);
        return len > 0 && path[len - 1] == '/';
}

// Split the filename at the last '.'.
// For example, file.old.txt becomes:
// pre_ext = file.old
// post_ext = .txt
static void split_file(char *filename, char **pre_ext, char **post_ext) {
        const char *dot = strrchr(filename, '.');
        // One issue is copying .bashrc not turning into 1.bashrc
        // First I check if the file is just called .
        if (dot == filename) {
                *pre_ext = strdup(filename);
                *post_ext = strdup("");

                if (*pre_ext == NULL || *post_ext == NULL) {
                        free(*pre_ext);
                        free(*post_ext);
                        *pre_ext = NULL;
                        *post_ext = NULL;
                }

                return;
        }
        // I now check if there is no . and pre_ext should be .bashrc and post_ext=""
        if (dot == NULL) {
                *pre_ext = strdup(filename);
                *post_ext = strdup("");

                if (*pre_ext == NULL || *post_ext == NULL) {
                        free(*pre_ext);
                        free(*post_ext);
                        *pre_ext = NULL;
                        *post_ext = NULL;
                }

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

// copies a file a number of times with a progress as well
static int copy_single_file(const char *old_file, const char *new_file) {
        int result = 0; // used for error handling at the bottom
        // create buffer to hold content of the file
        // Used to just do entire file size, but I think most
        // copy functions do buffer by buffer so we will just do it that way
        char buffer[COPY_BUFFER_SIZE];

        // total written is size_t only because I want to compare it to bytes_read
        // And bytes_read + bytes_written are size_t since its the return of fread and fwrite
        size_t bytes_read = 0, total_written = 0, bytes_written = 0;

        // Open the file with read only instead of just calling open
        int src_fd = open(old_file, O_RDONLY);

        if (src_fd == -1) {
                fprintf(stderr, "Error opening source '%s': %s\n", old_file, strerror(errno));
                return 1;
        }
        // Check if the file actually opened
        struct stat src_info;

        if (fstat(src_fd, &src_info) == -1) {
                fprintf(stderr, "Error checking source '%s': %s\n", old_file, strerror(errno));
                close(src_fd);
                return 1;
        }

        if (!S_ISREG(src_info.st_mode)) {
                fprintf(stderr, "Error: source '%s' is not a regular file\n", old_file);
                close(src_fd);
                return 1;
        }

        // Here we apply the read only when we create the file we plan on manipulating
        FILE *src_f = fdopen(src_fd, "rb");

        if (src_f == NULL) {
                fprintf(stderr, "Error creating source stream '%s': %s\n", old_file, strerror(errno));
                close(src_fd);
                return 1;
        }

        // I need this to fail if this dest file exists
        int dest_fd = open(new_file, O_WRONLY | O_CREAT | O_EXCL, 0600);

        if (dest_fd == -1) {
                fprintf(stderr, "Error creating destination '%s': %s\n", new_file, strerror(errno));
                fclose(src_f);
                return 1;
        }

        // I am opening the files with the premissions defined earlier
        FILE *dest_f = fdopen(dest_fd, "wb");

        // If the file fails to be opened just delete the new file we created
        if (dest_f == NULL) {
                fprintf(stderr, "Error creating destination stream '%s': %s\n", new_file, strerror(errno));

                close(dest_fd);
                fclose(src_f);
                remove(new_file);

                return 1;
        }

        // I basically just keep reading the file buffer by buffer and write it out
        while ((bytes_read = fread(buffer, 1, sizeof(buffer), src_f)) > 0) {
                total_written = 0;
                while (total_written < bytes_read) {
                        bytes_written = fwrite(buffer + total_written, 1, bytes_read - total_written, dest_f);

                        if (bytes_written == 0) {
                                if (ferror(dest_f)) {
                                        perror("fwrite");
                                } else {
                                        fprintf(stderr, "Error writing to '%s'\n", new_file);
                                }

                                result = 1;
                                break;
                        }

                        total_written += bytes_written;
                }
                if (result != 0) {
                        break;
                }
        }

        if (ferror(src_f)) {
                fprintf(stderr, "Error reading source '%s'\n", old_file);
                result = 1;
        }

        if (fclose(dest_f) != 0) {
                fprintf(stderr, "Error closing destination '%s'\n", new_file);
                result = 1;
        }

        if (fclose(src_f) != 0) {
                fprintf(stderr, "Error closing source '%s'\n", old_file);
                result = 1;
        }

        if (result != 0) {
                // I dont & this with the top because I want to run remove only if result !=0
                if (remove(new_file) != 0) {
                        fprintf(stderr, "Warning: could not remove incomplete file '%s'\n", new_file);
                }
        }

        return result;
}

// Here we init the values, basically I take in the inputs, and the return values I care about
// nCopies = number of copies to make
// verbose = shows the progress of copying these files
// src_path = the name of the file you want to duplicate
// dest_path = where the file will end up, the default is ./
int parse_args(int argc, char **argv, int *nCopies, int *verbose, char **src_path, char **dest_path) {
        int option = 0;
        char *tmp;
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
                                tmp = strdup(optarg);

                                if (tmp == NULL) {
                                        perror("strdup");
                                        return 1;
                                }
                                // Free it before we set it
                                free(*src_path);
                                *src_path = tmp;
                                break;

                        case 'd':
                                tmp = strdup(optarg);

                                if (tmp == NULL) {
                                        perror("strdup");
                                        return 1;
                                }
                                // Free it before we set it
                                free(*dest_path);
                                *dest_path = tmp;

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

        if (optind < argc) {
                fprintf(stderr, "Error: unexpected argument: %s\n", argv[optind]);
                return 1;
        }

        // Obviously if there is no src_path or dest path then we should error this out
        if (*src_path == NULL) {
                fprintf(stderr, "Error: source file is required.\n\n");
                print_help(argv[0]);
                return 1;
        }
        
        // A weird one off I found where if -d . is done then everything is hidden
        if ((*dest_path == NULL) || (strcmp(*dest_path, ".") == 0)) {
                tmp = strdup("./");

                if (tmp == NULL) {
                        perror("strdup");
                        return 1;
                }

                free(*dest_path);
                *dest_path = tmp;
        }

        // Now I check if the two exist (if they don't this should fail)
        if (!file_exists(*src_path)) {
                fprintf(stderr, "Error: source is not a regular file: %s\n", *src_path);
                return 1;
        }

        if (!dir_exists(*dest_path)) {
                fprintf(stderr, "Error: destination is not a directory: %s\n", *dest_path);
                return 1;
        }

        return 0;
}

int copy_files(const char *src_path, const char *dest_path, int nCopies, int verbose) {
        char *pre_ext  = NULL;
        char *post_ext = NULL;
        // if the input is ./filename then filename is the return
        char *filename = strrchr(src_path, '/');
        int required = 0;
        const char* req_format;

        if (filename != NULL) {
                filename++;
        } else {
                filename = (char*)src_path;
        }

        split_file(filename, &pre_ext, &post_ext);

        if (pre_ext == NULL || post_ext == NULL) {
                fprintf(stderr, "Error: failed to split filename\n");
                free(pre_ext);
                free(post_ext);
                return 1;
        }

        // The goal is now to create a numbered dest file by looping the copy_single_file function
        for (int i = 1; i <= nCopies; i++) {
                // Check if it ends with '/' 
                if (has_trailing_slash(dest_path)) {
                        req_format = "%s%s%d%s";
                } else {
                        req_format = "%s/%s%d%s";
                }
                required = snprintf(NULL, 0, req_format, dest_path, pre_ext, i, post_ext);
                // Idk if we would ever reach this, unless one of the above functions fails
                if (required < 0) {
                        fprintf(stderr, "Error constructing destination path\n");
                        free(pre_ext);
                        free(post_ext);
                        return 1;
                }

                // Since we know required has a non-zero value we can set it to the size of the new file
                char *new_file = malloc((size_t)required + 1);

                if (new_file == NULL) {
                        perror("malloc");
                        free(pre_ext);
                        free(post_ext);
                        return 1;
                }

                // Now we set the new_file to the size of the buffer we are writting
                snprintf(new_file, (size_t)required + 1, req_format, dest_path, pre_ext, i, post_ext);

                if (copy_single_file(src_path, new_file) != 0) {
                        free(new_file);
                        free(pre_ext);
                        free(post_ext);
                        return 1;
                }

                if (verbose) {
                        printf("Created %s from %s\n", new_file, src_path);
                }

                free(new_file);
        }

        free(pre_ext);
        free(post_ext);

        return 0;
}
