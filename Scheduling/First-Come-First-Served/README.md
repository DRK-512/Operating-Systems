# First-Come, First-Served
- First-Come, First-Served (FCFS) Scheduling: processes are executed in the order in which they arrive in the system. 
- It is a non-preemptive scheduling algorithm
- Once the CPU has been allocated to a process, that process keeps the CPU until it releases the CPU, either by terminating or by requesting I/O
- This makes the FCFS algorithm troublesome for interactive systems
- It is important that each process gets a share of the CPU at regular intervals
- One issue with this program is that there is no interrupt for processes that take too long, so one process can hog all the processing power

- The goal of this project is to grab a file "processes.txt", look at the processes listed, and implement a FCFS algorithm to schedule each of those threads
- The threads will be running a script to find the largrest prime number after some randomly generated uint 32
