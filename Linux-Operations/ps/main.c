
#include "functions.h"

int main (int argc, char *argv[]) {
    
    options valid_options = {0,0,0,0,0};
    int opt_return = getopt(argc, argv, "p:stvc");
    
    /* sets argv to coresponding options */
    while(opt_return != -1) {
        switch(opt_return) {           
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
    /* See if we have any options to do  */
    if(valid_options.pid == 0 && valid_options.state == 0 &&
       valid_options.program_time == 0 && valid_options.vmem == 0 && 
       valid_options.cmdline == 0 ) {
           show_options();
           exit(EXIT_FAILURE);
       }
    
    /* PID should never be 0 since that is reserved */ 
    if(valid_options.pid == 0){
        valid_options.pid = 1;
    } 

    print_values(valid_options);
    return 0;
}
