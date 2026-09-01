CC = gcc
CFLAGS = -g

all: Shortest_Scheduler
	
Shortest_Scheduler: 
	mkdir -p bin
	${CC} ${CFLAGS} src/prime.c src/primeFunctions.c -o bin/prime.o -Iinclude
	${CC} ${CFLAGS} src/schedulerFunctions.c src/main.c -o bin/main.o -Iinclude
	echo "./bin/main.o processes.txt" > Shortest_Scheduler
	chmod +x Shortest_Scheduler

clean: 
	rm -r bin
	rm Shortest_Scheduler
