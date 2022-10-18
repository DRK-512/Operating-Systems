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

## Program Features

Your program will implement the features triggered by the following options. If no options are
present, only “5ps” is run, exit the program with no output.

-p <pid>
- Display process information only for the process whose number is pid. If this option is not present then use the pid number a | (init process). The user is expected to provide pid number when using —p option. If the user runs “Sps -p” with no value please give an error message and exit the program.

-s
- Display the single-character state information about the process. This information is found in the stat file in process's directory, looking at the third ("state") field. Note that the information that you read from the stat file is a character string. If this option is not present, do not display this information.

-t
- Display the amount of time consumed by this process in hours:min:seconds format. This information is found in the stat file in process's directory, looking at the "utime” field. Add to this to the system time, “stime” field . This is the total time consumed by the process. This time is represented in clock ticks. You must divide my number of clock ticks per second (system constant) to obtain the number of seconds. It should then be displayed in hours:min:seconds format. If this option is not present, do not display this information.

-V
- Display the amount of virtual memory currently being used (in pages) by this program. This information is found in the statm file in process's directory, looking at first ("size") field. If this option is not present, do not display this information.

-c
- Display the command-line that started this program. This information is found in the cmdline file in process's directory. Be careful on this one, because this file contains a list of null (zero byte) terminated strings. If this option is not present, do not display this information.

All of your output must match with the output of “ps --ely” for the same pid.

## Program Structure
- The key to any good program, is having a clean and modular design. With such a design, your code will be simpler, smaller, easier to debug, and (most important, perhaps) you will get full points.\
- Even though this is a simple program with few components, you will need to develop clean interfaces for each component. And, of course, each component will be in a separately compiled file, linked together as the final step.

**NOTE:** At least two .c files and an .h file is required
### Some suggestions for modules in your design: 
- **Options processing**: This module processes the command line options, setting state variables to record what the options specify.
- **stat and statmparser**: This module will extract strings from the space-separated lists that are read from the stat and statn files.

