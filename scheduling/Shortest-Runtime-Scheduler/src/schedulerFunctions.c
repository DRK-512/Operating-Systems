#include "schedulerFunctions.h"

// the current running child
int8_t running_child = -1;
uint8_t totalTimeValue;
uint8_t dataSize;

PIDData data[100];
pid_t children[10];

// this will setup our data struct using the inputs from the file
void createData(FILE* file) { 

    uint8_t input[100], buffer[60]; 
    uint8_t i, j=0; 

    // throw first line away (since this is the processes.txt header)
    fgets(buffer,60,file); 

    // since max of 10 PIDs, and 3 total parameters, then we have a max of 30 
    for(i=0; i<=30; i++){
        if(fscanf(file, "%s", input)==EOF) {
            break; 
        }
        // the file splits the params into three collumns, so we check for them 
        if(i==0 || (i % 3)==0) {
            data[j].PID = atoi(input);
            dataSize++; 
        } else if(((i-1) % 3)==0) {
            data[j].AT = atoi(input);  
        } else {
            data[j].Burst = atoi(input); 
            j++; 
        } 
    } 
}

void scheduleHandler(){ 

    totalTimeValue++;
    
    if(running_child != -1) {
        data[running_child].Burst--;
        
        printf("Scheduler: Runtime: %u seconds.\nProcess %d is running with %d second(s) left.\n", 
        totalTimeValue,running_child,data[running_child].Burst);

        if(data[running_child].Burst <= 0){
		    printf("Scheduler: Terminating Process %d with Remaining Time: %d \n",
            running_child, data[running_child].Burst);
        	
            terminateChild(children[running_child]);
            waitpid(children[running_child],NULL,0);
		    children[running_child] = 0;
        	running_child = -1;
        }   
    }   

    PIDData shortestPID = findShortestPID();
    checkBurst(); 

    if(shortestPID.PID != running_child) {
        if(running_child != -1) {
        	printf("Scheduler: Suspending Process %d with Remaining Time : %d \n",
            running_child, data[running_child].Burst);

        	suspendChild(children[running_child]);
        }

        running_child = shortestPID.PID;

        if(children[running_child] == 0) {
            printf("Scheduler: Starting Process %d with Remaining Time : %d \n",
            running_child, data[running_child].Burst); 

            createChild(running_child);
        } else {
            running_child = shortestPID.PID;
            printf("Scheduler: Resuming Process %d with Remaining Time : %d \n",
            running_child, data[running_child].Burst);

            resumeChild(children[running_child]);
        }
    }
}

// returns shortestPID just in case two burst times are the same 
PIDData findShortestPID() {

    uint8_t i, location = 0;

    for(i=0; i < dataSize; i++) {
        if(data[i].Burst > 0) {
            if((data[i].Burst < data[location].Burst || 
                data[location].Burst <= 0) && 
                data[i].AT <= totalTimeValue) {
                location = i;
            }
        }
    }   
    
    if(data[location].AT > totalTimeValue){
        totalTimeValue++; 

        printf("Scheduler: Runtime: %u seconds.\nProcess %d has not arrived just yet.\n", 
        totalTimeValue,location);

        findShortestPID(); 
    }
    
    return data[location];
}

// This will check if all processes have finished
void checkBurst() {
    for(int i = 0; i < dataSize; i++)
        if (data[i].Burst > 0)
            return;

    exit(EXIT_SUCCESS);
}

void resumeChild(pid_t child) {
    if(child != 0)
        kill(child,SIGCONT);
}

void suspendChild(pid_t child) {
    if(child != 0)
        kill(child,SIGTSTP);
}

void terminateChild(pid_t child) {
    if(child != 0)
        kill(child,SIGTERM);
}

void createChild(uint16_t new_process) {
    char string_process[10];
    if(running_child != -1)
        suspendChild(children[running_child]);

    children[new_process] = fork();
    running_child = new_process;
    sprintf(string_process,"%d",new_process);

    char* argv[3];
    argv[0] = "./bin/prime";
    argv[1] = string_process;

    if (children[new_process] == 0)
        execlp("./bin/prime", argv[0],argv[1],NULL,NULL);
}
