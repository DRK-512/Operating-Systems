# Operating-Systems
This repo consists of random operating systems project I have done whilst reading "Operating Systems Concepts by Abraham Silberschatz, Peter Baer Galvin, and Greg Gagne" or random projects I made up because I was bored <br> 
Another great place for Operating System information is (GeekForGeeks)[https://www.geeksforgeeks.org/operating-systems/?ref=shm]<br>
Essentially it will act as a database of operating systems based repos I have done for easier navigation. 

Each project is categorized based off section, and each project should have its own README<br>
From the toplevel I plan on having a CMakeLists.txt so that users only need to do 
```bash
cmake -S . -B build
cmake --build build
```
As I complete sections, I will add it to the top-level cmake, but most stuff will be developmental until I finish<br>
As for style, I will manage that with the format script and format files I define in `./ci/format-files`<br>

# Sections Explained
For each of the sections, I have a set of lessons learned I want to achieve

## Linux-Operations
The goal of each project is to learn the following:
- Processes and programs
- Shell commands
- Files/directories
- Permissions
- Environment variables
- System calls
- Process creation/termination
- Basic Linux tooling
- Kernel Modules/Drivers

## Scheduling
The goal of each project is to learn the following:
- The purpose of each scheduler, and their use-cases
- Context switches
- FCFS
- Round Robin
- Priority scheduling
- Preemptive vs. non-preemptive scheduling
- Scheduling metrics
- Possibly synchronization implications

## Process-Synchronization
The goal of each project is to learn the following:
- Race conditions (Bank Account)
- Critical sections (Bank Account)
- Mutexes (Bank Account)
- Atomic operations (Bank Account)
- Semaphores (Producer-Consumer)
- Condition variables (Producer-Consumer)
- Producer-consumer (Producer-Consumer)
- Starvation (Reader-Writer)
- Reader-writer synchronization (Reader-Writer)
- Monitors (Elevator)
- Deadlocks (Dining Philosophers)
- Livelock (Game Parlor)

## Memory Management
The goal of each project is to learn the following:
- Virtual memory
- Address spaces
- Stack vs. heap
- Paging
- Page tables
- TLB
- Page faults
- Demand paging
- Page replacement
- Memory allocation
- Fragmentation
- malloc/free
- Memory protection
- Copy-on-write

## Networking
The goal of each project is to learn the following:
- Sockets
- TCP/UDP
- IP
- Ports
- Client/server architecture
- Connection establishment
- Blocking vs. non-blocking I/O
- Network byte order
- Basic protocols
- Possibly select/poll/epoll

## Security
The goal of each project is to learn the following:
- User/kernel privilege
- Permissions
- Access control
- Authentication
- Process isolation
- Memory protection
- Buffer overflows
- Privilege escalation
- Secure system calls
- Sandboxing
- Common OS-level vulnerabilities

## Misc
These are project I just find cool and fun, there is no learning exercise, these are for fun
