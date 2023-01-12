CC = gcc
CFLAGS = -lpthread -g

all: Elevator

Elevator: 
	${CC} main.c functions.c ${CFLAGS} -o Elevator 
	./Elevator
	
clean: 
	rm Elevator

