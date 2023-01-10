
#include "functions.h"

/*
 * Description: Returns the filepath to the PID's status file
 */
char* getFilePath(int pid, int vmem){

    char file_path[500] = "/proc/"; 
    char pidStr[100];
    char* return_path; 
    
    sprintf(pidStr,"%d",pid);
    strcat(file_path,pidStr);

    /* Content for vmem is in statm, not status */
    if(vmem){
        strcat(file_path,"/statm");
    } else {
        strcat(file_path,"/status");
    } 

    return_path = file_path; 
    return return_path; 
}

/*
 *  Input to the method: The struct of options
 *  Description: Checks if the file exists, and will return 1 if it does 
 */
int checkFile(int pid){
    
    char* file_path = getFilePath(pid, 0);

    if (access(file_path, F_OK) == 0) {
        return 1; 
    } else {
        printf("A program with PID %d does not exist\n", pid);
        return 0;  
    }
}

/*
 *  Description: This will print the command that ran the program
 */
void print_cmdline(int pid) {

    char* file_path = getFilePath(pid, 0);
    FILE *in_file = fopen(file_path,"r"); 
    
    char cmdline_text [500];
    
    /* We check the file before we run this, so its existance is certain */
    fscanf(in_file, "%*s\t%s", cmdline_text);
    printf("\n[%s]",cmdline_text);
}

/*
 * Description: This will print the virtual memory
 */
void print_vmem(int pid){
    
    char* file_path = getFilePath(pid, 1); 
    FILE *in_file = fopen(file_path,"r"); 

    char vmem [100];

    /* We check the file before we run this, so its existance is certain */
    fscanf(in_file, "%s", vmem);
    printf("\nSize: %s",vmem);
    
    fclose(in_file);
}

/*
 * Description: This will print the state of the program
 */
void print_state(int pid){
    
    char* file_path = getFilePath(pid, 0);
    FILE *in_file = fopen(file_path,"r");

    char state[10];

    /* We check the file before we run this, so its existance is certain */
    fscanf(in_file, "%*s %*s %c", state);
    printf("\nState: %s", state);
    
    fclose(in_file);
}

/*
 * Description: This will print the programs runtime
 */
void print_programtime(int pid){
    
    char* file_path = getFilePath(pid, 0);
    FILE *in_file = fopen(file_path,"r");

    char state[10]; 
    int i, uTime = 0, sTime = 0; 
    long unsigned int real_time, hours, minutes, seconds; 

    /* jump to arguments 14 and 15 for the program time */
    for(i=0;i<13;i++){
        fscanf(in_file, "%*s"); 
    }

    /* now we calculate runtime, then print */ 
    fscanf(in_file,"%d",&uTime);
    fscanf(in_file,"%d",&sTime);
    real_time = (float)(uTime+sTime) / sysconf(_SC_CLK_TCK);
    hours = real_time / 3600;
    minutes = (real_time % 3600) / 60;
    seconds = (real_time % 3600) % 60;
    printf("\nRuntime: %lu:%lu:%lu", hours, minutes,seconds);
    
    fclose(in_file);
}

/*
 * Brief description of the task: This will show the options if the user types in an incorrect argument
 */
void show_options(){
    printf("Invalid Input\n");
    printf("Options for this program: \n");
    printf("-p <pid> : Returned info is about this pid. If option is not used, process 1 is used\n");
    printf("-s       : Returns the state of the given pid\n");
    printf("-t       : Returns amount of time consumed by this process in hours:min:seconds format\n");
    printf("-v       : Returns the amount of virtual memory currently being used by this program\n");
    printf("-c       : Returns the command-line that started this program\n");
}

/*
 * Description: This will print the values of the options
 */
void print_values(options opts){
    /* Check if stat file for the PID exists */
    if(checkFile(opts.pid)) {
        /* Stat files have a most of the information we need for this project */
        printf("PID: %d", opts.pid);
        opts.state ? print_state(opts.pid) : 1; 
        opts.program_time ? print_programtime(opts.pid) : 1;
        opts.vmem ? print_vmem(opts.pid) : 1;
        opts.cmdline ? print_cmdline(opts.pid) : 1;
        printf("\n");
    } 
}
