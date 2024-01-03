#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>
#include <linux/string.h>

#define DEVICE_NAME "fibonacci"

// I prefer my code under MIT, but linus knows best ...
MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("Darek Konopka");
MODULE_DESCRIPTION("Fibonacci Device Driver");
MODULE_VERSION("1.0");

static int major_number;

// Here is the arithmetic function I use to add two numbers 
char* addStrings(char* num1, char* num2, char* result) 
{
	unsigned long long len1 = strlen(num1), len2 = strlen(num2);
	unsigned long long rS = len2+1; 
	unsigned int diff=0, i, sum=0, carry = 0, digit1=0, digit2=0;

	// in case len1<len2, this will cuase issues later
	if(len1!=len2)
		diff=1; 

	// Traverse both strings from right to left
	for (i = rS-1; i > 0; i--) {
		
		if((i-diff)>0){
			digit1 = num1[i-diff-1] - '0';
			digit2 = num2[i-1] - '0';
		} else {
			digit1 = 0;
			digit2 = num2[i-1] - '0';
		}

		sum = digit1 + digit2 + carry;
		
		// sum can only be a single digit, so that is why we have the carry
		if(sum>9) {
			sum-=10; 
			carry=1; 
		} else {
			carry=0; 
		}

		if(i!=1){
			result[i] = sum + '0';
		} else {
			result[1] = sum + '0';
			if(carry==0){
				memmove(result, result + 1, rS);
				rS--; 
			} else {
				result[0] = carry + '0';
			}
		}
	}
	return result; 
}

char* my_strdup(const char* str) 
{
    size_t len = strlen(str) + 5;
    char* new_str = (char*)kmalloc(len, GFP_KERNEL);
    if (new_str != NULL) 
        strcpy(new_str, str);
    
    return new_str;
}

// returns the fibonacci number with remainder and overflow counter
char* fibonacci(unsigned long long n) 
{
	// initalize
	unsigned long long i; 
	char *a, *b, *temp;  

	// no need to calc such small numbers
	if (n == 0)
		return "0"; 
	else if(n <= 2) 
		return "1"; 

	a = my_strdup("0");
	b = my_strdup("1");
	temp = (char*)kmalloc(2, GFP_KERNEL);
	
	for (i = 2; i <= n; i++) {
		// always reallocate before assignment
		kfree(temp); 
		temp = addStrings(a, b, (char *)kcalloc(strlen(b)+2, sizeof(char), GFP_KERNEL));

		kfree(a);
		a = my_strdup(b);

		kfree(b);
		b = my_strdup(temp);

	}
	kfree(a); 
	kfree(temp); 
	return b;
}

static ssize_t device_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos) 
{
	long long unsigned input;
	char* result; 
	
	if (kstrtoull_from_user(buf, count, 10, &input) != 0) 
		return -EINVAL;
	
	result = fibonacci(input);
	pr_info("Fibonacci number at index %llu: %s", input, result);
	kfree(result); 

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

