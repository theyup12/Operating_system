/**
 * hello.c
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/hash.h>
#include <linux/gcd.h>
#include <linux/jiffies.h>

#define BUFFER_SIZE 128
#define P_NAME "hello"

int simple_init(void) {
        printk(KERN_INFO "/proc/%s created\n", P_NAME);
	printk(KERN_INFO "GOLDEN_RATIO_PRIME is: %lu\n", GOLDEN_RATIO_PRIME);
	return 0;
}
/* This function is called when the module is removed. */
void simple_exit(void) {
        printk( KERN_INFO "The gcd of 3,300 and 24 is: %lu\n", gcd(3300, 24));
        printk( KERN_INFO "/proc/%s removed\n", P_NAME);
}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");