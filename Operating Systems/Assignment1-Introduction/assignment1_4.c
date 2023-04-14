#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void* getFilesShellCommand();

int main(void) {
    pthread_t th1;
    // Create child thread
    pthread_create(&th1, NULL, getFilesShellCommand, "Child Thread");

    // Join child thread so main thread waits for its execution
    pthread_join(th1, NULL);

    // exit main thread and return
    printf("Exiting from main thread\n");
    return 0;
}

/*
* Get the files and directories in the current directory
* using the shell command.
* Returns a pointer for the child thread.
*/
void *getFilesShellCommand() {
    if (system("ls -1") == -1) {
        perror("Error");
        return NULL;
    }

    return;
}