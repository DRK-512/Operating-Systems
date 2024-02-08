CC = gcc

all: Virt_Mem_Manager

Virt_Mem_Manager: 
	${CC} src/main.c src/functions.c -o Virt_Mem_Manager 
	./Virt_Mem_Manager -a ./inputs/addresses.txt -p ./inputs/process.txt
	
clean: 
	rm Virt_Mem_Manager

