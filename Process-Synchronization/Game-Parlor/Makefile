CC = gcc
CFLAGS = -lpthread 

all: Game_Parlor

Game_Parlor: 
	${CC} src/main.c src/gameProcessing.c src/gameStats.c ${CFLAGS} -o Game_Parlor 
	./Game_Parlor
	
clean: 
	rm Game_Parlor

