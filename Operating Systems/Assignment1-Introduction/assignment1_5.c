#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

// define leds pins
#define LED1 7
#define LED2 0
#define LED3 2
#define LED4 3

void setup();

int main( int argc, char *argv[] )  {
    // Check the number of arguments provided in the command line
    if( argc == 2 ) {
        wiringPiSetup();
        int inp = atoi(argv[1]);  // convert input string to integer
        if (inp > 15 || inp < 0) { // input x must be 0 <= x <= 15
            printf("The input must be between 0 and 15 (both inclusive). Exiting the program...");
            return 1;
        }

        // setup pinModes
        setup();

        // bitwise comparison to check the exact bit that I want.
        // inp & 1 return a value greater than 0 if the bit is 1.
        digitalWrite(LED1, (inp & 1) > 0);
        digitalWrite(LED2, (inp & (1<<1)) > 0);
        digitalWrite(LED3, (inp & 100) > 0);
        digitalWrite(LED4, (inp & 1000) > 0);

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

// Sets up the pinModes
void setup() {
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    pinMode(LED4, OUTPUT);
}