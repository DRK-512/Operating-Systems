CC = gcc
CFLAGS = -lpthread -g

all: Reader_Writer

Reader_Writer: 
	${CC} main.c functions.c ${CFLAGS} -o Reader_Writer
	./Reader_Writer
	
clean: 
	rm Reader_Writer

