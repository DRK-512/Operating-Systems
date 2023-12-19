#include "fibonacciCalc.h"

#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>


#define DEVICE_NAME "fibonacci"

// All of my code I make should be under MIT
MODULE_LICENSE("MIT");
MODULE_AUTHOR("Darek Konopka");
MODULE_DESCRIPTION("Fibonacci Device Driver");
MODULE_VERSION("1.0");

static int major_number;

// Here I calculate the n-th Fibonacci number
void fibonacciCalc(unsigned int n) 
{
        // we printf instead of return because we have 2 return types, long long unsinged int, and mapz_t
        if (n <= 1)
                return;
        if (n<=94) {
                int i;
                unsigned long long a = 0, b = 1, temp=0;
                
                for (i = 2; i <= n; i++) {
                        temp = a + b;
                        a = b;
                        b = temp;
                }
                
                printf("%llu", a);
        } else {
                // 94 is 12200160415121876738, and 95 will overflow, so we need this for large numbers
                int i;
                unsigned long long a = 0, b = 1, temp=0;
                
                for (i = 2; i <= 93; i++) {
                        temp = a + b;
                        a = b;
                        b = temp;
                }

                mpz_t bA, bB, bTemp;
                mpz_inits(bA, bB, bTemp, NULL);

                // I know this might look weird, but trust me, its right
                mpz_set_ui(bA, temp);
                mpz_set_ui(bTemp, a);
                mpz_add(bB, bA, bTemp); 

                for (i = 94; i <= n; i++) {
                        //printf("I is: %d", i); 
                        mpz_add(bTemp, bA, bB);
                        mpz_set(bA, bB);
                        mpz_set(bB, bTemp);
                }
                gmp_printf("%Zd", bA);
                mpz_clears(bA, bB, bTemp, NULL);

        }
}

// Function to handle reads from the device
static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t *offset) 
{
    /*
    int bytes_read = 0;

    // Check if offset is valid
    if (*offset >= MAX_FIBONACCI)
        return 0;  // End of file

    // Calculate the Fibonacci number at the specified offset
    unsigned long long fib_value = fibonacci((int)(*offset));

    // Copy the Fibonacci number to the user buffer
    size_of_message = snprintf(message, sizeof(message), "%llu\n", fib_value);
    if (copy_to_user(buffer, message, size_of_message) != 0) {
        pr_alert("Fibonacci: Failed to send Fibonacci number to the user\n");
        return -EFAULT;
    }

    // Update the offset and return the number of bytes read
    *offset += 1;
    return size_of_message;
    */
}

// File operations for the device
static const struct file_operations fops = 
{
    .read = device_read,
};

// Module initialization function
static int __init fibonacci_init(void) 
{
    // Register the character device
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        pr_alert("Fibonacci: Failed to register a major number\n");
        return major_number;
    }

    pr_info("Fibonacci: Module loaded with major number %d\n", major_number);
    return 0;
}

// Module cleanup function
static void __exit fibonacci_exit(void) 
{
    // Unregister the character device
    unregister_chrdev(major_number, DEVICE_NAME);
    pr_info("Fibonacci: Module unloaded\n");
}

module_init(fibonacci_init);
module_exit(fibonacci_exit);
