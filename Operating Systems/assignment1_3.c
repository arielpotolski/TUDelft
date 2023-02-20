#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

wiringPiSetup();

int getFilesShellCommand();

int main() {
	int pid = 0;
	pid = fork();  // create child process
    
	if (pid == 0) {
		printf("I am the child process\n");
        getFilesShellCommand();  // call the function to get the files and dirs in the current dir

	} else {
		printf("I am the parent process (pid %d)\n", pid);
		wait(NULL)  // wait for child process to finish
		exit(0);  // exit with no error
	}
	return 1;
}

/*
* Get the files and directories in the current directory
* using the shell command.
*/
int getFilesShellCommand() {
    if (system("ls -1") == -1) {
        perror("Error");
        return 1;
    }

    return 0;
}