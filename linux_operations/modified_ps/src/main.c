
#include "functions.h"

int main(int argc, char *argv[]) {

        options valid_options = {0, 0, 0, 0, 0};
        int     opt_return    = getopt(argc, argv, "p:stvc");

        /* sets argv to coresponding options */
        while (opt_return != -1) {
                switch (opt_return) {
                        case 'c':
                                valid_options.cmdline = 1;
                                break;
                        case 'p':
                                valid_options.pid = atoi(optarg);
                                break;
                        case 's':
                                valid_options.state = 1;
                                break;
                        case 't':
                                valid_options.program_time = 1;
                                break;
                        case 'v':
                                valid_options.vmem = 1;
                                break;
                        default:
                                show_options();
                                exit(EXIT_FAILURE);
                }
                opt_return = getopt(argc, argv, "p:stvc");
        }

        /* See if we have any options */
        if (!valid_options.pid && !valid_options.vmem && !valid_options.state && !valid_options.cmdline
            && !valid_options.program_time) {
                show_options();
                exit(EXIT_FAILURE);
        } else if (!valid_options.pid) {
                /* PID should never be 0 since that is reserved */
                valid_options.pid = 1;
        }

        print_values(valid_options);
        return 0;
}
