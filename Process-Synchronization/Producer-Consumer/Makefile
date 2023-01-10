CC = gcc
CFLAGS = -lpthread -g

all: Producer_Consumer

Producer_Consumer: 
	${CC} main.c functions.c ${CFLAGS} -o Producer_Consumer
	./Producer_Consumer
	
clean: 
	rm Producer_Consumer

