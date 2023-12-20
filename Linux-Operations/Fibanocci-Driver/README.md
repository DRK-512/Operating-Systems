# Fibonacci Driver
- Write a Linux kernel module to create the device /dev/fibonacci
- Writing to this device should have no effect, however reading at offset k should return the kth fibonacci number
## What is a Fibonacci Sequence
- The Fibonacci sequence is a sequence in which each number is the sum of the two preceding ones
- The first 15 are: 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377

## How The Code Works
- This is a Linux Driver, so it is a bit different than a simple gcc command followed by execution
- The test.c is a way to test the fibanocci function works
- The fibonacci.c is the module we will be using
- You need to load the driver to your linux kernel, so that is why the makefile is much longer than usual
- You will notice there is no main.c file, that is becuase kernel modules dont really use a main funciton

## Expected Errors: 
- If you get this: 
```bash
warning: the compiler differs from the one used to build the kernel
  The kernel was built by: x86_64-linux-gnu-gcc-11 (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0
  You are using:           gcc-11 (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0
```
Do this: 


- If you get this error: 
```bash
 insmod: ERROR: could not insert module fibonacci.ko: Invalid module format
```
- It means that the kernel version is having a miss match, you can run dmesg and see it
- To fix this, run the bugFix script
