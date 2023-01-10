CC = gcc
CFLAGS = -g

all: Shortest_Scheduler
	
Shortest_Scheduler: 
	${CC} ${CFLAGS} prime.c primeFunctions.c -o prime
	${CC} ${CFLAGS} main.c schedulerFunctions.c -o Shortest_Scheduler
	./Shortest_Scheduler input.txt

clean: 
	rm prime Shortest_Scheduler

