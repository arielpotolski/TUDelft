#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_barrier_t barrier;

// define leds pins
#define LED1 7
#define LED2 0
#define LED3 2
#define LED4 3

void setup();
void *lit();
void *unlit();

int main(void) {
    wiringPiSetup();
    setup();
    pthread_t fadeLit, fadeUnlit;

    // Create a barrier that waits for two threads
    pthread_barrier_init(&barrier, NULL, 2);

    // Create child thread
    pthread_create(&fadeLit, NULL, lit, NULL);
    pthread_create(&fadeUnlit, NULL, unlit, NULL);

    // Join child thread so main thread waits for its execution
    pthread_join(fadeLit, NULL);
    pthread_join(fadeUnlit, NULL);

    return 0;
}

/*
* Function to light up the LEDS gradually.
*/
void *lit() {
    int i = 0;

    while (i != 101) {
        usleep(75000);
        softPwmWrite(LED1, i);
        softPwmWrite(LED2, i);
        softPwmWrite(LED3, i);
        softPwmWrite(LED4, i);
        i++;
    }

    // wait for both threads to get here
    pthread_barrier_wait(&barrier);

    return NULL;
}

/*
* Function to gradually turn off the LEDS.
*/
void *unlit() {
    // wait for both threads to get here
    pthread_barrier_wait(&barrier);

    int i = 100;

    while (i != -1) {
        usleep(75000);
        softPwmWrite(LED1, i);
        softPwmWrite(LED2, i);
        softPwmWrite(LED3, i);
        softPwmWrite(LED4, i);
        i--;
    }

    return NULL;
}

// Initialize the leds to zero and declare the range
void setup() {
    softPwmCreate(LED1, 0, 100);
    softPwmCreate(LED2, 0, 100);
    softPwmCreate(LED3, 0, 100);
    softPwmCreate(LED4, 0, 100);
}