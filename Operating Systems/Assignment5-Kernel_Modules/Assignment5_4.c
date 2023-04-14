#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/device.h>
#include <linux/hrtimer.h>
#include <linux/gpio.h>

#define LED1 4
#define LED2 17
#define LED3 27
#define LED4 22

#define DEVICE_NAME "mypwm"

MODULE_LICENSE("GPL");

// Global variables
static int major;  // major number assigned to the device
static int deviceOpen = 0;  // Used for thread safety (prevents multiple accesses)
struct class *class;
struct mutex lock;
static struct hrtimer hr_timer;
static ktime_t interval;
static long milli;

enum hrtimer_restart timer_callback(struct hrtimer *timer);


/*
 *  Prototypes - this would normally go in a .h file
 */
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations fops = {
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
    class = class_create(THIS_MODULE, "5.4");

    // Create device
    device_create(class, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);

    // initialize mutex
    mutex_init(&lock);

    //request LEDs
    gpio_request(LED1, "LED1");
    gpio_request(LED2, "LED2");
    gpio_request(LED3, "LED3");
    gpio_request(LED4, "LED4");

    //initialize timer
    hrtimer_init(&hr_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
    hr_timer.function = timer_callback;

    return 0;
}

void cleanup_module(void) {
    // cancel timer
    hrtimer_cancel(&hr_timer);

    // Turn LEDs off
    gpio_direction_output(LED1, 0);
    gpio_direction_output(LED2, 0);
    gpio_direction_output(LED3, 0);
    gpio_direction_output(LED4, 0);

    //free LEDs
    gpio_free(LED1);
    gpio_free(LED2);
    gpio_free(LED3);
    gpio_free(LED4);

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
    mutex_unlock(&lock);
    return 0;
}

/**
 * Called when a process try to write to the file.
*/
static ssize_t device_write(struct file *filp, const char *buff, size_t length, loff_t *off) {
    size_t maxLen = 80, suc;
    char databuf[maxLen];
    long inp;
    int intInp;

    if (length < maxLen) {
        maxLen = length;
    }

    suc = copy_from_user(databuf, buff, maxLen);

    if (suc != 0) {
        printk(KERN_INFO "Could not copy the input string.");
    }

    databuf[maxLen] = 0;

    kstrtol(databuf, 10, &inp);

    intInp = (int) inp;
    printk(KERN_INFO "New duty cycle is: %d\n", intInp);

    milli = 10 * intInp;
    if (intInp == 0) {  //if we want the lights off
        milli = 0;
    }

    interval = ktime_set(0, milli * 10000);  // set interval to duty cycle
    hrtimer_start(&hr_timer, interval, HRTIMER_MODE_REL);

    return length;
}

enum hrtimer_restart timer_callback(struct hrtimer *timer) {
    static int led_state = 0;  // current state
    led_state = !led_state;  // next state

    if (led_state && milli != 0) {  // turn on leds
        gpio_direction_output(LED1, 1);
        gpio_direction_output(LED2, 1);
        gpio_direction_output(LED3, 1);
        gpio_direction_output(LED4, 1);
        interval = ktime_set(0, milli * 10000);  // duty cycle interval until next callback
    } else {  // turn off leds
        gpio_direction_output(LED1, 0);
        gpio_direction_output(LED2, 0);
        gpio_direction_output(LED3, 0);
        gpio_direction_output(LED4, 0);
        interval = ktime_set(0, (1000-milli) * 10000);  
    }

    hrtimer_forward_now(timer, interval);  //forward the expirity so it expires after now
    return HRTIMER_RESTART;
}