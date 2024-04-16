# Fibonacci Driver
- Write a Linux kernel module to create the device /dev/fibonacci
- Writing to this device should have no effect; however, reading at offset k should return the kth fibonacci number
## What is a Fibonacci Sequence
- The Fibonacci sequence is a sequence in which each number is the sum of the two preceding ones.
- The first 15 are: 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377

## What do we have? 
- In the src dir we have four .c files.
1. fibonacci.c is the kernel module made for this and uses the test-without-gmp.c approach since gmp is not part of the kernel 
2. test-with-gmp.c is a fibonacci solver that uses a GNU library that is not available in the linux kernel
3. test-without-gmp.c is a fibonacci solver that uses a workaround for not being able to use gmp
4. fibonacciCalc.c is a file consisting of the fibonacci functions used for test-with-gmp.c
5. test-single-add.c is a program that adds two large numbers without it being a fibonacci sequence

## How The Code Works
- This is a Linux Driver, so it is a bit different than a simple gcc command followed by execution.
- The test-with-gmp and test-without-gmp is a way to test that fibanocci functions works.
- If someone wants to test the faster way using GNU libraries, they can look at test-with-gmp.
```bash
# Example Output
$ ./test-with-gmp
Enter Fibonacci number you want: 130
Calculating Fibonacci Number for 130 ... 
Fibonacci Number of 130 is: 659034621587630041982498215
The # digits is: 27
$ ./test-without-gmp 
Enter Fibonacci number you want: 130
Calculating Fibonacci Number for 130 ... 
Fibonacci Number of 130 is: 659034621587630041982498215
```
- The fibonacci.c is the module we will be using, and uses the test-without-gmp approach, so expect that output in the dmesg.
- You need to load the driver to your linux kernel, so that is why the makefile is much longer than usual.
- You will notice there is no main.c file, that is because kernel modules don't really use a main funciton.

## How To Build The Kernel Module
- I have created a few make functions that can help you achieve this.
### 1. Compile The Module
```bash
make
```
### 2. Insert The Kernel Module
```bash
make install
```
### 3. Load the Kernel Module
- Before you can run another command, you must run dmesg to find a line of code that looks like this: 
```bash
Fibonacci: Module loaded with major number 510
```
- Now, you will have to edit the Makefile's upload function to use your major function; in my case, it looks like this: 
```bash
sudo mknod /dev/fibonacci c 510 0
```
### 4. Writing to The Kernel Module
- Now the kernel module should be uploaded to /dev/fibonacci
- To test this, open a terminal and run the following: 
```bash
sudo su
echo "90" > /dev/fibonacci 
echo "300" > /dev/fibonacci 
dmesg
```
- What you have done is ask fibonacci to solve for 90 and 130, and the dmesg should output the results
- The results from dmesg should look like this: 

```bash
[11847.563133] Fibonacci: Module loaded with major number 510
[11886.993597] Fibonacci number at index 90: 2880067194370816120
[11934.632741] Fibonacci number at index 300: 222232244629420445529739893461909967206666939096499764990979600
```
