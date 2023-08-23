# First-Come, First-Served
## What is a FCFS Scheduler
- First-Come, First-Served (FCFS) Scheduling: processes are executed in the order in which they arrive in the system. 
- It is a non-preemptive scheduling algorithm
- Once the CPU has been allocated to a process, that process keeps the CPU until it releases the CPU, either by terminating or by requesting I/O
- This makes the FCFS algorithm troublesome for interactive systems
- It is important that each process gets a share of the CPU at regular intervals
- One issue with this program is that there is no interrupt for processes that take too long, so one process can hog all the processing power
- The goal of this project is to grab a file "processes.txt", look at the processes listed, and implement a FCFS algorithm to schedule each of those threads
- The threads will be running a script to find the largrest prime number after some randomly generated uint 32
## How to Run
```bash
make
./FCFS_Scheduler
```
This will run the FCFS Scheduler with the processes being defined in the processes.txt file<br>
If two processes arrive at the same time, then the one with the smallest PID goes first<br>
Feel free to edit the processes file to see how the scheduler would work with your processes
## Why is the processes.txt so oddly formated? 
The reason the processes.txt file is so oddly formated is because it was tested to work even with X amount of spaces and tabs between each number<br>
This gets rid of needing to ensure the formatting is perfect/required to look a specific way