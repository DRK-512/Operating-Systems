# Round Robin
- Round Robin (RR) Scheduling is an improved version of the FCFS algorithm because it implements preemption to enable the system to switch between processes
- A small unit of time, called a time quantum or time slice, is defined (in our example it is 3), and each process will run for a time slide, the go back into the ready queue
- This way processes are executed in a cyclic manner, with each process being given a fixed time slice (or quantum) in which to execute. 
- It is a preemptive scheduling algorithm, just like FCFS

- The goal of this project is to grab a file "processes.txt", look at the processes listed, and implement a RR algorithm to schedule each of those threads
- The threads will be running a script to find the largrest prime number after some randomly generated uint 32
