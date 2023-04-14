#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/device.h>

#define LEN 80  // max length of the input string
#define DEVICE_NAME "klog"

MODULE_LICENSE("GPL");

// Global variables
static int major;  // major number assigned to the device
struct class *class;
struct mutex lock;

/*
 *  Prototypes - this would normally go in a .h file
 */
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release
};

int init_module(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);

    if (major < 0) {  // registration failed
        // print it to the log
        printk(KERN_ALERT "Registration failed with %d\n", major);
        return major;
    }

    // Create class
    class = class_create(THIS_MODULE, "5.2");

    // Create device
    device_create(class, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);

    mutex_init(&lock);

    return 0;
}

void cleanup_module(void) {
    // remove the device
    device_destroy(class, MKDEV(major, 0));

    // unregister the device class
    class_unregister(class);

    // remove the device class
    class_destroy(class);

    // unregister the major number
    unregister_chrdev(major, DEVICE_NAME);
}

/**
 * Called when a process tries to open the device file.
*/
static int device_open(struct inode *inode, struct file *file) {
    int check;
    check = mutex_trylock(&lock);

    if (check == 0) {
        return -EBUSY;
    }

    return 0;
}

/**
 * Called when a process closes the file
*/
static int device_release(struct inode *inode, struct file *file) {
    // decrement the usage count
    mutex_unlock(&lock);
    return 0;
}

/**
 * Called when a process tries to read from the file.
*/
static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t *offset) {
    printk(KERN_ALERT "Sorry, this operation is not supported.\n");
    return -EINVAL;
}

/**
 * Called when a process try to write to the file.
*/
static ssize_t device_write(struct file *filp, const char *buff, size_t length, loff_t *off) {
    printk(KERN_INFO "Input string is: %s\n", buff);
    return length;
}
