#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse_help.h"
#include <zconf.h>

/*
   Function Name: get_set_options
   Input to the method: these are the input arguments of the C file (so argc and argv) 
   Output(Return value): Returns a sctruct of all the options, and the default values of the options
   Brief description of the task: This will get all the options from the input arguments
*/
options get_set_options(int argc, char *argv[]) {
    int getopt_return;
    options cmdoptions = {0,0,0,0,0};
    
    /* This will loop through the argv arguments 
    and set it with the coresponding options */
    while((getopt_return = getopt(argc, argv, "p:stvc")) != -1) {
        switch(getopt_return) {
            
            case 'p':
                cmdoptions.pid = atoi(optarg);
                break;
            
            case 's':
                cmdoptions.state = 1;
                break;
            
            case 't':
                cmdoptions.program_time = 1;
                break;
            
            case 'v':
                cmdoptions.vmem = 1;
                break;
            
            case 'c':
                cmdoptions.cmdline = 1;
                break;
            
            default:
                show_options();
                exit(EXIT_FAILURE);
        }
    }
    // Check all values stored in opts, if all are 0, exit 
    if(cmdoptions.pid == 0 && cmdoptions.state == 0 &&
       cmdoptions.program_time == 0 && cmdoptions.vmem == 0 && 
       cmdoptions.cmdline == 0 ) {
           show_options();
           exit(EXIT_FAILURE);
       }
    
    // if PID = 0, set it to 1 
    if(cmdoptions.pid == 0){
        cmdoptions.pid = 1;
    }   
    return cmdoptions;
}

/*
   Function Name: print_values
   Input to the method: The struct of options
   Output(Return value): There are none
   Brief description of the task: This will print the values of the options
 */
void print_values(options opts){
    int exists;
    // check if stat file for the PID exists
    exists = checkFile(opts); 
    if(exists){
     // If the stat file exists, then we find the other options
        printf("%d ", opts.pid);
        if(opts.state){print_state(opts.pid);}
        if(opts.program_time){print_programtime(opts.pid);}
        if(opts.vmem){print_vmem(opts.pid);}
        if(opts.cmdline){print_cmdline(opts.pid);}
        printf("\n");
    } 
}

/*
   Function Name: checkFile
   Input to the method: The struct of options
   Output(Return value): An int being a 1 or 0
   Brief description of the task: This will check if the file exists, and if it does, then we will return 1
 */
int checkFile(options opts){
    
    // find the file
    FILE *in_file;
    
    // get file path
    char file_path[100] = "/proc/"; 
    char pidStr[20];
    
    sprintf(pidStr,"%d",opts.pid);
    char cmdline_text [500];

    strcat(file_path,pidStr);
    strcat(file_path,"/status");
    in_file = fopen(file_path,"r"); 
    
    // If the stat file does not exist, that means that the program does not exist
    if(in_file==NULL) {
        printf("A program with PID %d does not exist\n", opts.pid);
        return 0; 
    } else {
        return 1; 
    }
}

/*
   Function Name: print_cmdline
   Input to the method: The struct of options
   Output(Return value): There are none
   Brief description of the task: This will print the command line that ran the program
*/

void print_cmdline(int pid){
    // open the file
    FILE *in_file;
    char file_path[100] = "/proc/"; 
    char pidStr[20];
    sprintf(pidStr,"%d",pid); 
    char cmdline_text [500];

    strcat(file_path,pidStr);
    strcat(file_path,"/status");
    in_file = fopen(file_path,"r"); 
    
    // since we know the file exists, we should always be able to go into here
    if(in_file!=NULL) {
        fscanf(in_file, "%*s\t%s", cmdline_text);
        printf("[%s]",cmdline_text);
    }
}

/*
   Function Name: print_vmem
   Input to the method: The PID of the program
   Output(Return value): There are none
   Brief description of the task: This will print the virtual memory
*/
 
void print_vmem(int pid){
    // Open the file 
    FILE *in_file;
    char file_path[100] = "/proc/"; 
    char pidStr[20];
    sprintf(pidStr,"%d",pid);
    char vmem [100];

    strcat(file_path,pidStr);
    strcat(file_path,"/statm");
    in_file = fopen(file_path,"r"); 

    // since we know the file exists, we should always be able to go into here
    if(in_file!=NULL) {
        fscanf(in_file, "%s", vmem);
        printf("sz=%s ",vmem);
    }
    fclose(in_file);
}

/*
   Function Name: print_state
   Input to the method: The PID of the program
   Output(Return value): There are none
   Brief description of the task: This will print the state of the program
*/
 
void print_state(int pid){
    // Open the file
    FILE *in_file;
    char file_path[100] = "/proc/"; 
    char pidStr[20];
    sprintf(pidStr,"%d",pid);
    char state[10];

    strcat(file_path,pidStr);
    strcat(file_path,"/stat");
    in_file = fopen(file_path,"r"); 

    // since we know the file exists, we should always be able to go into here
    if(in_file!=NULL) {
        fscanf(in_file, "%*s %*s %c", state);
        printf("%s ",state);
    }
    fclose(in_file);
}

/*
   Function Name: print_programtime
   Input to the method: The PID of the program
   Output(Return value): There are none
   Brief description of the task: This will print the programs runtime
*/

void print_programtime(int pid){
    // open the file
    FILE *in_file;
    char file_path[100] = "/proc/"; 
    char pidStr[20];
    sprintf(pidStr,"%d",pid);
    char state[10];

    strcat(file_path,pidStr);
    strcat(file_path,"/stat");
    in_file = fopen(file_path,"r"); 

    // since we know the file exists, we should always be able to go into here
    if(in_file!=NULL) {
        int uTime =0; 
        int sTime =0; 
        int i=0; 

	// here we jump to arguments 14 and 15 so we can calculate the program time
        for(i;i<13;i++){
            fscanf(in_file, "%*s"); 
        }
            
        // now we will calculate the runtime, then print it hours:minutes:seconds    
        fscanf(in_file,"%d",&uTime);
        fscanf(in_file,"%d",&sTime);
        long unsigned int real_time = (float)(uTime+sTime) / sysconf(_SC_CLK_TCK);
        long unsigned int hours = real_time / 3600;
        long unsigned int minutes = (real_time % 3600) / 60;
        long unsigned int seconds = (real_time % 3600) % 60;
        printf("time=%lu:%lu:%lu ", hours, minutes,seconds);
    }
    fclose(in_file);
}

/*
   Function Name: show_options
   Input to the method: There are none
   Output(Return value): There are none
   Brief description of the task: This will show the options if the user types in an incorrect argument
*/
 
void show_options(){
    // if the user inputs an incorrect argument, the code will echo back all the avalible options
    printf("Incorrect Command\n");
    printf("Options for this program: \n");
    printf("-p <pid> : Returned info is about this pid. If option is not used, process 1 is used\n");
    printf("-s       : Returns the state of the given pid\n");
    printf("-t       : Returns amount of time consumed by this process in hours:min:seconds format\n");
    printf("-v       : Returns the amount of virtual memory currently being used by this program\n");
    printf("-c       : Returns the command-line that started this program\n");
}
