# Makefile for building the fibonacci kernel module with -lmpfr

# Compiler and flags
CC := $(shell which gcc)
obj-m := fibonacci.o

# Source files
fibonacci-objs := src/fibonacci.o 

# Kernel build directory
KDIR := /lib/modules/$(shell uname -r)/build

# Module build directory
PWD := $(shell pwd)

# Build rule
all:
	make -C $(KDIR) M=$(PWD) modules
install: 
	sudo insmod fibonacci.ko
upload: 
	sudo mknod /dev/fibonacci c 510 0
	ls /dev/fibonacci
uninstall: 
	sudo rmmod fibonacci
reinstall: 
	make -C $(KDIR) M=$(PWD) clean
	make -C $(KDIR) M=$(PWD) modules
	sudo rmmod fibonacci
	sudo insmod fibonacci.ko
test: 
	gcc src/fibonacciCalc.c src/test-with-gmp.c -o test-with-gmp -lgmp -Iinclude
	gcc src/test-without-gmp.c -o test-without-gmp
clean:
	make -C $(KDIR) M=$(PWD) clean
	rm test-without-gmp test-with-gmp

