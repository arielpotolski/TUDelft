#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

// define leds pins
#define LED1 7
#define LED2 0
#define LED3 2
#define LED4 3

void setup();
void *getInput(void *arg);
void *ledCount(void *arg);
void *readFile(void *arg);

int main(int argc, char *argv[]) {
    // Check the number of arguments provided in the command line
    if( argc == 2 ) {
        wiringPiSetup();
        setup();

        pthread_t listen, count, read;  // children threads
        int inp = 0;

        // Create children threads
        // last argument is the parameter of the function
        pthread_create(&listen, NULL, getInput, (void*)&inp);
        pthread_create(&count, NULL, ledCount, (void*)&inp);
        pthread_create(&read, NULL, readFile, (void*)&argv[1]);

        // Join children threads so main thread waits for their execution
        pthread_join(listen, NULL);
        pthread_join(count, NULL);
        pthread_join(read, NULL);

        // exit main thread and return
        printf("Exiting from main thread\n");
        return 0;       
    }
    else if( argc > 2 ) {
        printf("Too many arguments supplied.\n");
        return 1;
    }
    else {
        printf("One argument expected.\n");
        return 1;
    }
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
        fflush(stdout);
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

void *readFile(void *arg) {
    FILE *inp;
    char *file = *(char**)arg;  // arg is a pointer to a pointer

    //open file in read mode
    inp = fopen(file,"r");

    // check for failure in openning file
    if (inp == NULL) {
        printf("Cannot open file.\n");
        return NULL;
    }
    clock_t start, end;
    double n, sum = 0.0;
    start = clock();

    // while there are numbers to read from the file
    while (fscanf(inp, "%lf", &n) != EOF) {
        n = atan(tan(n));
        sum += n;
    }

    end = clock();
    double duration = ((double)end - start) / CLOCKS_PER_SEC;
    
    printf("The sum is: %lf\n", sum);
    printf("Time taken to read the numbers from the file ");
    printf("and make the calculations (seconds): %f\n", duration);
    return 0;
}

// Sets up the pinModes and turn off leds
void setup() {
    softPwmCreate(LED1, 0, 100);
    softPwmCreate(LED2, 0, 100);
    softPwmCreate(LED3, 0, 100);
    softPwmCreate(LED4, 0, 100);
}