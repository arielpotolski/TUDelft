#include <stdio.h>
#include <unistd.h>

//setup WiringPi
wiringPiSetup();

int main(void) {
    char name[21] = "Ariel Potolski Eilat"; // name
    int stdNumber = 5482526; // student number

    // print name and student number
    printf("Name: %s\n", name);
    printf("Student Number: %d\n", stdNumber);

    //get the process ID
    pid_t id = getpid();
    printf("ID: %d\n", id);

    return 0;
}