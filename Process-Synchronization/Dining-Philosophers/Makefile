CC = gcc
CFLAGS = -g -lpthread

all: Dining_Philosophers

Dining_Philosophers: 
	${CC} src/main.c src/philosopherFunc.c ${CFLAGS} -o Dining_Philosophers 
	./Dining_Philosophers
	
clean: 
	rm Dining_Philosophers

