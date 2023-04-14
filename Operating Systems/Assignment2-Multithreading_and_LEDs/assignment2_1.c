#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// define leds pins
#define LED1 7
#define LED2 0
#define LED3 2
#define LED4 3

void setup();
void *getInput(void *arg);
void *ledCount(void *arg);

int main(void) {
    wiringPiSetup();
    setup();

    pthread_t listen, count;  // children threads
    int inp = 0;

    // Create children threads
    // last argument is the parameter of the function
    pthread_create(&listen, NULL, getInput, (void*)&inp);
    pthread_create(&count, NULL, ledCount, (void*)&inp);

    // Join children threads so main thread waits for their execution
    pthread_join(listen, NULL);
    pthread_join(count, NULL);

    // exit main thread and return
    printf("Exiting from main thread\n");
    return 0;
}

/*
* Continuously listens to the input
*/
void *getInput(void *arg) {
    char input[10];

    while (1) {
        fgets(input, sizeof(input), stdin); // Read user input from command line
        int num = atoi(input);  // convert input string to integer
        int *inp = (int*) arg; // cast void ptr to integer ptr

        if (num <= 15 && num >= 0) { // input x must be 0 <= x <= 15
            *inp = num; // inp is updated
        } else {  // invalid input
            printf("The input must be between 0 and 15 (both inclusive).");
        }

    }

    return NULL;
}

/*
* Counts to the target number
*/
void *ledCount(void *arg) {
    int num = 0; // counter
    int *inp = (int*)arg;  // cast from void ptr to int ptr

    while (1) {
        usleep(200000);  // sleep for 200 miliseconds

        // bitwise comparison to check the exact bit that I want.
        // inp & 1 return a value greater than 0 if the bit is 1.
        if ((num & 1) > 0) {
            softPwmWrite(LED1, 100);
        } else {
            softPwmWrite(LED1, 0);
        }

        if ((num & (1<<1)) > 0) {
            softPwmWrite(LED2, 100);
        } else {
            softPwmWrite(LED2, 0);
        }

        if ((num & 100) > 0) {
            softPwmWrite(LED3, 100);
        } else {
            softPwmWrite(LED3, 0);
        }

        if ((num & 1000) > 0) {
            softPwmWrite(LED4, 100);
        } else {
            softPwmWrite(LED4, 0);
        }

        if (num < *inp) {
            ++num;
        } if (num > *inp) {
            --num;
        }
    }
}

// Sets up the pinModes and turn off leds
void setup() {
    softPwmCreate(LED1, 0, 100);
    softPwmCreate(LED2, 0, 100);
    softPwmCreate(LED3, 0, 100);
    softPwmCreate(LED4, 0, 100);
}