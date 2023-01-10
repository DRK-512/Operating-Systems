CC = gcc
CFLAGS = -lpthread -g

all: Game_Parlor

Game_Parlor: 
	${CC} main.c gameProcessing.c gameStats.c ${CFLAGS} -o Game_Parlor 
	./Game_Parlor
	
clean: 
	rm Game_Parlor

