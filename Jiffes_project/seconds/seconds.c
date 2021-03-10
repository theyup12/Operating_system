/*second.c*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/jiffies.h>

#define BUFFER_SIZE 128
#define P_NAME "seconds"

unsigned long begin_jiffies;
ssize_t proc_read(struct file *file, char *buf, size_t count, loff_t *pos);

static struct file_operations proc_ops = {
    .owner = THIS_MODULE,
    .read = proc_read,
};

/* This function is called when the module is loaded. */
int proc_init(void) {
    // creates the /proc/second entry
    proc_create(P_NAME, 0666, NULL, &proc_ops);
    printk(KERN_INFO "/proc/%s created\n", P_NAME);
    begin_jiffies = jiffies;
	return 0;
}

/* This function is called when the module is removed. */
void proc_exit(void) {
    remove_proc_entry(P_NAME, NULL);
    printk( KERN_INFO "/proc/%s removed\n", P_NAME);
}

ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos) {
    int rv = 0;
    char buffer[BUFFER_SIZE];
    static int completed = 0;
    if (completed) {
            completed = 0;
            return 0;
    }
    completed = 1;
    rv = sprintf(buffer, "current time :%lu seconds\n", ((jiffies - begin_jiffies)/HZ));
    // copies the contents of buffer to userspace usr_buf
    copy_to_user(usr_buf, buffer, rv);

    return rv;
}
/* Macros for registering module entry and exit points. */
module_init( proc_init );
module_exit( proc_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello Module");
MODULE_AUTHOR("SGG");