CC = gcc
CFLAGS = -g

all: Memory_Manager

Memory_Manager: 
	${CC} ${CFLAGS} main.c -o Memory_Manager
	./Memory_Manager addresses.txt process.txt

clean: 
	rm Memory_Manager
