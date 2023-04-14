#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/module.h>

#define LED1 4
#define LED2 17
#define LED3 27
#define LED4 22

MODULE_LICENSE("GPL");

int init_module(void) {
    //request LEDs
    gpio_request(LED1, "LED1");
    gpio_request(LED2, "LED2");
    gpio_request(LED3, "LED3");
    gpio_request(LED4, "LED4");

    printk(KERN_INFO "Turning on LEDS...");

    // turn LEDs on
    gpio_direction_output(LED1, 1);
    gpio_direction_output(LED2, 1);
    gpio_direction_output(LED3, 1);
    gpio_direction_output(LED4, 1);

    return 0;
}

void cleanup_module(void) {
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

    printk(KERN_INFO "Leaving module. Goodbye!");
}