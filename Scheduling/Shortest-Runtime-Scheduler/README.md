# Shortest Remaining Time First Scheduler
**Author:Darek Konopka**

## What is a Shortest Runtime Scheduler? 
- Shortest Job First (SJF) Scheduling: A scheduler that allows processes to be executed based on their burst time, with the shortest job (or the one with the shortest burst time) being executed first. It can be either non-preemptive or preemptive.

## How to compile it
To compile the code use following command:
```bash
make
```
This will also automatically run it as well since no input params are needed, it only looks at the processing.txt

## Project Description
Here we will create an SJF scheduler that creates one child process for each process in the processing.txt file, and schedules them according to the shortest remaining time scheduling algorithm. It ensures that, at every instant of time, the process that is currently running has the least amount of remaining run time compared to any other process in the system. A process is taken off the CPU in two ways:
  1.) The current running process finishes its burst or 
  2.) A newly arrived process has lower burst time than the remaining time of the currently running process. 

The input to the scheduler consists of m lines, each line contains a process number(0 to m—1), arrival time of the process, and, the CPU burst of each process. Each parameter is separated by one or more spaces. There will be a header line in the input file which will not be processed. The arrival times and CPU bursts are given in seconds and will be whole numbers. The processes are listed in the increasing order of their arrival times and no processes arrive in a fractional number of seconds (such as 2.5). Multiple processes may not arrive at the same instant. There will be at most 10 entries in the file. The entries in each line may be separated by any number of spaces or tabs. Your program should handle this with no errors. There are no errors expected in the input file. The name of the file containing the input is read from the terminal.

The scheduler keeps time to determine which process needs to be scheduled next on the CPU. When a process is scheduled to run for the first time, it is forked and it is ran. When a new process arrives that needs to be scheduled, the running process is suspended and the new process runs. When it is time for the suspended process to run, it is resumed and it is run. When a process finishes its entire burst, the scheduler terminates that process.

Thus the scheduler switches processes on the CPU by suspending one process, and resuming another process. The suspensions and resumptions are done by sending signals
to the corresponding child processes. The scheduler keeps time by using an interval timer, as described later in this document.

For simplicity, each child process executes the same code. It will start with a random 10 digit number, say 1234567890 and simply find the next prime number higher than the starting number. The primality checking algorithm we use is a trivial one, in which number n is decided to be prime, if it is not divisible by any number between 2 to n. This algorithm is chosen so that CPU is kept busy during the burst time of the process. Keeping the starting number large will keep the CPU busy. 
Note: That you have to use a long	unsiqned int as the variable type that stores the number being checked for primality.

Each child process should print during the following: 
- First time when it is started
- Each time it is suspended
- Each time it is resumed
- When it has completed. 
When a child process first gets the CPU, it should print the first number that it is checking to verify primality. Each time a child process is suspended or resumed, it should print the largest number that it determined to be prime. When the process finally finishes its burst it should print the largest number it found to be prime. No intermediate prime numbers should be printed.

The output from your scheduler should include the folloiwing scheduling activities: 
- A line when it schedules a process for the first time, i.e forks
- Each time the scheduler suspends a process
- Each time the scheduler resumes a process 
- Each time it terminates a process. 
Each line of printout should include the current time in seconds that is kept by the scheduler.

## SAMPLE RUN:
Assume the processes.txt contains the following:

| PID |  AT  |  BURST |
| :-: | :--: |  :---: |
|  0  |   1  |    7   |
|  1  |   2  |    4   |
|  2  |   4  |    6   |
|  3  |   6  |    4   | 
|  4  |   7  |    2   |

```bash
Scheduler: Scheduler runtime: 1 second. 
Scheduling Process 0 (Pid 1314) with a 7 second burst time.
```
**When a process is scheduled for the first time it prints the following:**
```bash
Process 0: My PID is 1314, and I just got started. 
Process 0: I'll be finding the next prime number from 233343434.
Scheduler: Scheduler runtime: 2 seconds. 
```
**When a process is suspended it prints the following:**
```bash
Process 0: I am about to be suspended. 
Process 0: The highest prime number I found is 5754853343.
Suspending Process 0 with remaining time of 6 seconds and resuming process 1 with remaining time of 4 seconds
Process 1: my PID is 1315: I just got started. 
Process 1:  I'll be finding the next prime number from 9848288302. 
Scheduler: Scheduler runtime: 4 seconds
New process 2 with remaining time of 4 seconds has arrived
Scheduler: Time Now: 6 seconds Terminating Process 1
New process 3 with remaining time of 4 seconds has arrived Scheduling process 3
Etc.
```
**When a process ends it prints the following:**
```bash
Process 0: my PID is 1314: I completed my task and I am exiting. Highest prime number I found is 1000000000063.
```

