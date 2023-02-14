CC = gcc
CFLAGS = -g

all: FCFS_Scheduler

FCFS_Scheduler: 
	mkdir -p bin
	${CC} ${CFLAGS} src/prime.c src/primeFunctions.c -o bin/prime.o -Iinclude
	${CC} ${CFLAGS} src/schedulerFunctions.c src/main.c -o bin/main.o -Iinclude
	echo "./bin/main.o processes.txt" > FCFS_Scheduler
	chmod +x FCFS_Scheduler

clean: 
	rm -r bin
	rm FCFS_Scheduler
