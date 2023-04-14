#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/module.h>
#include <linux/hrtimer.h>
enum hrtimer_restart timer_callback(struct hrtimer *timer);

#define LED1 4
#define LED2 17
#define LED3 27
#define LED4 22

MODULE_LICENSE("GPL");

static struct hrtimer hr_timer;
static ktime_t interval;


int init_module(void) {
    //request LEDs
    gpio_request(LED1, "LED1");
    gpio_request(LED2, "LED2");
    gpio_request(LED3, "LED3");
    gpio_request(LED4, "LED4");

    interval = ktime_set(1, 0);  //initial interval to 1

    hrtimer_init(&hr_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
    hr_timer.function = timer_callback;
    hrtimer_start(&hr_timer, interval,HRTIMER_MODE_REL);

    return 0;
}

enum hrtimer_restart timer_callback(struct hrtimer *timer) {
    static int led_state = 0;  // current state
    led_state = !led_state;  // next state

    if (led_state) {  // turn on leds
        gpio_direction_output(LED1, 1);
        gpio_direction_output(LED2, 1);
        gpio_direction_output(LED3, 1);
        gpio_direction_output(LED4, 1);
        interval = ktime_set(1, 0);  // 1 second interval until next callback
    } else {  // turn off leds
        gpio_direction_output(LED1, 0);
        gpio_direction_output(LED2, 0);
        gpio_direction_output(LED3, 0);
        gpio_direction_output(LED4, 0);
        interval = ktime_set(2, 0);  // 2 seconds interval until next callback
    }

    hrtimer_forward_now(timer, interval);  //forward the expirity so it expires after now
    return HRTIMER_RESTART;
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
}