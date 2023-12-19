# Fibonacci Driver
- Write a Linux kernel module to create the device /dev/fibonacci
- Writing to this device should have no effect, however reading at offset k should return the kth fibonacci number
## What is a Fibonacci Sequence
- The Fibonacci sequence is a sequence in which each number is the sum of the two preceding ones
- The first 15 are: 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377

## How The Code Works
- This is a Linux Driver, so it is a bit different than a simple gcc command followed by execution
- The test.c is a way to test the fibanocci function works
- The main.c is the program used to generate the driver
- You need to load the driver to your linux kernel, so that is why the makefile is much longer than usual