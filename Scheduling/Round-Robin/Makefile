CC = gcc
CFLAGS = -g

all: RR_Scheduler

RR_Scheduler: 
	mkdir -p bin
	${CC} ${CFLAGS} src/prime.c src/primeFunctions.c -o bin/prime.o -Iinclude
	${CC} ${CFLAGS} src/schedulerFunctions.c src/main.c -o bin/main.o -Iinclude
	echo "./bin/main.o processes.txt" > RR_Scheduler
	chmod +x RR_Scheduler

clean: 
	rm -r bin
	rm RR_Scheduler
