# Own PS Program
**Author: Darek Konopka**

### What is the goal? 
- To write a simple versinon of the ps command. 
- It will be called 5ps, and it will be executed from the command line
- It will read a variety of information about one / more processes on the pc

### How to compile it
To compile the code use following command:
```bash
$gcc 5ps.c parse_help.c -o 5ps
```

### How it is used:
```bash 
./5ps [options]
```

Options:
```
-p <pid>  Display information about the given process id.
-s			  Display the processes current state.
-t			  Display the time the process has consumed.
-v			  Display the amount of virtual memory used by the process.
-c			  Display the program that spawned the process.
```

### Demo
```
$./5ps -p 1134 -c -v -s -t
1134 S time=0:0:0 sz=5652 [bash]

$./5ps -p 1187 -c -v -s -t
1187 S time=0:0:0 sz=3219 [sftp-server]

$./5ps -s -t
1 S time=0:0:2

$./5ps -p 954 -t
954 time=0:0:0

$./5ps -p 285023 -c -v -s -t
A program with PID 285023 does not exist
```

