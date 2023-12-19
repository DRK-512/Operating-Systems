CC = gcc
CFLAGS = -lgmp -Iinclude

all: fibonacci test

fibonacci: 
	${CC} ./src/fibonacciCalc.c ./src/main.c -o fibonacci ${CFLAGS}
test: 
	${CC} ./src/fibonacciCalc.c ./src/test.c -o test ${CFLAGS}
clean: 
	@if [ -e "test" ]; then \
		echo "Deleting test"; \
		rm test; \
	fi
	@if [ -e "fibonacci" ]; then \
		echo "Deleting fibonacci"; \
		rm fibonacci; \
	fi
