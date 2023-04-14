#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "queue.h"

Queue *q1, *q2, *q3, *q4;  // LEDs queues

// define leds pins
#define LED1 7
#define LED2 0
#define LED3 2
#define LED4 3

void setup();
void *getInput();
void *turnOnLeds(void *arg);

int main() {
    wiringPiSetup();
    setup();

    pthread_t th1, th2, th3, th4, read;  // children threads
    int l1, l2, l3, l4;

    l1 = 1;
    l2 = 2;
    l3 = 3;
    l4 = 4;

    // allocate space to initialize the queues
    q1 = malloc(sizeof(Queue));
    q2 = malloc(sizeof(Queue));
    q3 = malloc(sizeof(Queue));
    q4 = malloc(sizeof(Queue));

    // Initialize queues
    initQueue(q1);
    initQueue(q2);
    initQueue(q3);
    initQueue(q4);

    // Create children threads
    pthread_create(&read, NULL, getInput, NULL);
    pthread_join(read, NULL);

    pthread_create(&th1, NULL, turnOnLeds, (void*)&l1);
    pthread_create(&th2, NULL, turnOnLeds, (void*)&l2);
    pthread_create(&th3, NULL, turnOnLeds, (void*)&l3);
    pthread_create(&th4, NULL, turnOnLeds, (void*)&l4);

    // Join children threads so main thread waits for their execution
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    pthread_join(th3, NULL);
    pthread_join(th4, NULL);

    // exit main thread and return
    printf("Exiting from main thread\n");
    free(q1);
    free(q2);
    free(q3);
    free(q4);
    return 0;       
}

/*
* Continuously listens to the input
*/
void *getInput() {
    int LED, brightness, duration;

    while (1) {
        int args = scanf("%d %d %d", &LED, &brightness, &duration); // Read user input

        if (args < 3) break;

        if (LED < 0 || LED > 3) {
            printf("Invalid LED.");
        } else if (brightness < 0 || brightness > 100) {
            printf("Invalid brightness");
        } else {
            if (LED == 0) {
                addToQueue(q1, brightness, duration);
            } else if (LED == 1) {
                addToQueue(q2, brightness, duration);
            } else if (LED == 2) {
                addToQueue(q3, brightness, duration);
            } else if (LED == 3) {
                addToQueue(q4, brightness, duration);
            }
        }
    }

    return NULL;
}

void *turnOnLeds(void *arg) {
    int *LED = (int*) arg; // ptr to the LED nbr
    int *b = malloc(sizeof(int)); // ptr to the brightness value
    int *d = malloc(sizeof(int)); // ptr to the duration value

    while (1) {
        if (*LED == 1) {
            while (queueSize(q1) != 0) {
                removeFromQueue(q1, b, d);
                softPwmWrite(LED1, *b);
                usleep((*d) * 1000);  // turn off if queue is empty
            }
            softPwmWrite(LED1, 0);
        } else if (*LED == 2) {
            while (queueSize(q2) != 0) {
                removeFromQueue(q2, b, d);
                softPwmWrite(LED2, *b);
                usleep((*d) * 1000);
            }
            softPwmWrite(LED2, 0);
        } else if (*LED == 3) {
            while (queueSize(q3) != 0) {
                removeFromQueue(q3, b, d);
                softPwmWrite(LED3, *b);
                usleep((*d) * 1000);
            }
            softPwmWrite(LED3, 0);
        } else if (*LED == 4) {
            while (queueSize(q4) != 0) {
                removeFromQueue(q4, b, d);
                softPwmWrite(LED4, *b);
                usleep((*d) * 1000);
            }
            softPwmWrite(LED4, 0);
        }
    }
    free(b);
    free(d);
}

// Sets up the pinModes and turn off leds
void setup() {
    softPwmCreate(LED1, 0, 100);
    softPwmCreate(LED2, 0, 100);
    softPwmCreate(LED3, 0, 100);
    softPwmCreate(LED4, 0, 100);
}