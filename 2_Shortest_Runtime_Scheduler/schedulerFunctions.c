#include "schedulerFunctions.h"

// the current running child
int running_child = -1;
int totalTimeValue;
PIDData data[100];
int dataSize;
pid_t children[10];

/*
    Function Name: createData
    Input to the method: input file
    Output(Return value): none
    Brief description of the task: This function takes the data from the input file, and 
    puts it into the data struct we created
 */
// this will setup our data struct using the inputs from the file
void createData(FILE* file) { 

    // varibles needed for the function 
    char input[100];
    char buffer[60]; 
    int i=0; 
    int j =0; 

    // throw first line away
    fgets(buffer,60,file); 

    // since max of 10 PIDs, and 3 total parameters, then we have a max of 30 
    for(i;i<=30;i++){
        if(fscanf(file, "%s", input)==EOF) {
            break; 
        }
        // the file splits the params into three collumns, so we check for them 
        if(i==0 || i%3==0){
            data[j].PID = atoi(input);
                    dataSize++; 
        } else if((i-1)%3==0){
            data[j].AT = atoi(input);  
        } else {
            data[j].Burst = atoi(input); 
            j++; 
        } 
    } 
}

/*
    Function Name: suspendChild
    Input to the method: PID of the child
    Output(Return value): None
    Brief description of the task: This will suspend the child process
*/
void suspendChild(pid_t child){
    if(child != 0){
        kill(child,SIGTSTP);
    }
}

/*
    Function Name: resumeChild
    Input to the method: PID of the child
    Output(Return value): None
    Brief description of the task: This will resume the child process
*/
void resumeChild(pid_t child){
    if(child != 0){
        kill(child,SIGCONT);
    }
}

/*
    Function Name: terminateChild
    Input to the method: PID of the child
    Output(Return value): None
    Brief description of the task: This will terminate the child process
*/
void terminateChild(pid_t child){

    if(child != 0){
        kill(child,SIGTERM);
    }
}

/*
    Function Name: createChild
    Input to the method: PID of the child
    Output(Return value): None
    Brief description of the task: This will create a child process 
*/
void createChild(int new_process){
    char string_process[10];
    if(running_child != -1){
        suspendChild(children[running_child]);
    }

    children[new_process] = fork();
    running_child = new_process;
    sprintf(string_process,"%d",new_process);
    char* argv[3];
    argv[0] ="./sample_prime";
    argv[1] = string_process;
    argv[2] =NULL;
    if (children[new_process] == 0){
        execlp("./prime", argv[0],argv[1],argv[2],NULL);
    } 
    
}

/*
    Function Name: scheduleHandler
    Input to the method: signal
    Output(Return value): none 
    Brief description of the task: This is the schedule handler  
*/
void scheduleHandler(int signum){ 
    totalTimeValue++;
    if(running_child != -1){
        data[running_child].Burst--;
        
        printf("Scheduler: Time Now: %u seconds. Process Running : %d Burst Left : %d\n", 
            totalTimeValue,running_child,data[running_child].Burst);
        if(data[running_child].Burst <= 0){
		printf("Scheduler: Terminating Process %d with Remaining Time : %d \n",
            running_child, data[running_child].Burst);
        	terminateChild(children[running_child]);
                waitpid(children[running_child],NULL,0);
		children[running_child] = 0;
        	running_child = -1;
        }   
    }     
    PIDData smallestPID = findSmallestPID();
    checkBurst(); 
    if(smallestPID.PID != running_child){
        if(running_child != -1){
        	printf("Scheduler: Suspending Process %d with Remaining Time : %d \n",
                running_child, data[running_child].Burst);
        	suspendChild(children[running_child]);
        }
        running_child = smallestPID.PID;
        if(children[running_child] == 0){
            printf("Scheduler: Starting Process %d with Remaining Time : %d \n",
                running_child, data[running_child].Burst); 
            createChild(running_child);
        }
        else{
            running_child = smallestPID.PID;
            printf("Scheduler: Restarting Process %d with Remaining Time : %d \n",
                running_child, data[running_child].Burst);
            resumeChild(children[running_child]);
        }
    }
    

}

/*
    Function Name: findSmallestPID
    Input to the method: none
    Output(Return value): location of smallest PID
    Brief description of the task: this will return the location of the smallest PID
*/
PIDData findSmallestPID() {
 int location = 0;
 for(int i=0; i < dataSize; i++){
     if(data[i].Burst > 0){
     	if((data[i].Burst < data[location].Burst || data[location].Burst <= 0) 
            && data[i].AT <= totalTimeValue){
         	location = i;
     	}
     }
 }
 return data[location];

}

/*
    Function Name: checkBurst
    Input to the method: none
    Output(Return value): none
    Brief description of the task: This will check the burst of the process 
*/
void checkBurst(){
    for(int i = 0; i < dataSize; i++){
        if (data[i].Burst > 0){
            return;
        }
    }
    exit(EXIT_SUCCESS);
}

