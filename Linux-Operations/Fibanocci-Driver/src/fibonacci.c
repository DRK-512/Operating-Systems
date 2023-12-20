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

struct overLong_t {
	unsigned long long overCount;
	unsigned long long remainder;
};

// Here is the arithmetic function I use to add two numbers from bigNum_t
struct overLong_t addBigNums(struct overLong_t a, struct overLong_t b) 
{
	struct overLong_t result = {0, 0};  // Default values

	result.remainder = a.remainder + b.remainder;

	// Check for overflow
	if (result.remainder < a.remainder || result.remainder < b.remainder) {
		result.overCount = 1;
	}

	result.overCount += a.overCount + b.overCount;

	// Adjust remainder if overflow occurred
	if (result.overCount > 0) {
		result.remainder -= (unsigned long long)(-1); // subtract max value to get correct remainder
	}

	return result;
}

// returns the fibonacci number with remainder and overflow counter
struct overLong_t fibonacci(unsigned long long n) 
{
	// initalize
	unsigned long long i; 
	struct overLong_t a = {0, 0};
	struct overLong_t b = {0, 1};
	struct overLong_t temp;

	// no need to calc such small numbers
	if (n == 0) {
		return a;
	} else if(n <= 2) {
		return b; 
	}

	for (i = 2; i <= n; ++i) {
		temp = b;
		b = addBigNums(a, b);
		a = temp;
	}

	return b;
}

static ssize_t device_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos) 
{
	long long unsigned input;
	struct overLong_t result; 
	
	if (kstrtoull_from_user(buf, count, 10, &input) != 0) 
		return -EINVAL;
	
	// Calculate Fibonacci number
	result = fibonacci(input);

	// Display the result
	if (result.overCount == 0) {
		pr_info("Fibonacci number at index %llu: %llu\n", input, result.remainder);
	} else {
		pr_info("Fibonacci number at index %llu: %llu + 18446744073709551615 * %llu\n", input, result.remainder, result.overCount);
		pr_info("18446744073709551615 is the max of unsinged long long, so overflow will occur here"); 
	}
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

