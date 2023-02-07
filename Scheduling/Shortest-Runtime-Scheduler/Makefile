CC = gcc
CFLAGS = -g

all: Shortest_Scheduler
	
Shortest_Scheduler: 
	mkdir -p bin
	${CC} ${CFLAGS} src/prime.c src/primeFunctions.c -o bin/prime -Iinclude
	${CC} ${CFLAGS} src/schedulerFunctions.c src/main.c -o Shortest_Scheduler -Iinclude
	./Shortest_Scheduler processes.txt

clean: 
	rm -r bin
	rm Shortest_Scheduler
