#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "fibonacci"

// I prefer my code under MIT, but linus knows best ...
MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("Darek Konopka");
MODULE_DESCRIPTION("Fibonacci Device Driver");
MODULE_VERSION("1.0");

static int major_number;
//static char *message = NULL; 
//static short size_of_message;

void fibonacciCalcLow(unsigned long long* result, unsigned int n) 
{
	int i;
	unsigned long long a = 0, b = 1, temp=0;
	// 94 -> 19740274219868223167 > 18446744073709551615 unsigned long long max
	if (n <= 1 || n > 93) {
		printk("Input must be greater than 0, or less than 94"); 
		printk("Our input was: %d", n); 
		return; 
	}

	for (i = 1; i <= n; i++) {
		temp = a + b;
		a = b;
		b = temp;
	}

	*result=a; 
	return; 
}

int countDigitsLow(unsigned long long int num) 
{
	unsigned int count=0; 
	do {
		num /= 10;
		++count;
	} while (num != 0);
	printk("Count is: %d", count); 
	return count; 
}
/*
// Function to allocate memory for the message buffer
static int allocate_message_buffer(int offset) {
    message = kmalloc(offset, GFP_KERNEL);
    if (!message) {
        printk(KERN_ERR "Failed to allocate memory for the message buffer\n");
        return -ENOMEM;
    }
    return 0;
}
*/
static ssize_t device_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos) {
    long long unsigned input, result;

    if (kstrtoull_from_user(buf, count, 10, &input) != 0) {
        return -EINVAL;
    }

    fibonacciCalcLow(&result, input);
    
    pr_info("Fibonacci result for input %llu: %llu\n", input, result);

    return count;
}

// The only thing this module can do is read
static const struct file_operations fops = 
{
	.write = device_write,
};

// mounts device to /dev/fibonacciX where X = major number
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

static void __exit fibonacci_exit(void) 
{
	// Unregister the module at major number
	unregister_chrdev(major_number, DEVICE_NAME);
	pr_info("Fibonacci: Module unloaded\n");
}

// On Linux side we either init the module, or exit, no fancy stuff
module_init(fibonacci_init);
module_exit(fibonacci_exit);

