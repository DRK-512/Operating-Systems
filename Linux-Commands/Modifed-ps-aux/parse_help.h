// This is a struct of the 5 options we are looking for
typedef struct commandline_options{
    int pid;
    int state;
    int program_time;
    int vmem;
    int cmdline;
}options;

// These are prototypes for the parse_help.c file
options get_set_options(int argc, char *argv[]);

int checkFile(options opts);

void show_options();
void print_values(options opts);
void print_state(int pid);
void print_programtime(int pid);
void print_vmem(int pid);
void print_cmdline(int pid);


