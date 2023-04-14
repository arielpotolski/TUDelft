#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>

wiringPiSetup();

/*
* Creates a directory using shell command
*/
int createDirShellCommand() {
    if (system("mkdir shell_command_dir") == -1) { // shell command mkdir
        printf("Error creating dir.");
        return 1;
    }
    return 0;
}

/*
* Create a directory using interaction with files and
* folders from C standard libraries available through
* "#include"
*/
int createDirThroughInclude() {
    if (mkdir("include_command_dir", 0777) == -1) {
        perror("Error");
        return 1;
    }

    return 0;
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

/*
* Get the files and directories in the current directory
* using C standard libraries.
*/
int getFilesThroughInclude() {
    DIR *directory;  // instantiates a DIR pointer
    struct dirent *entry;

    directory = opendir(".");  // opens directory

    if (directory == NULL) {  // if null, error
        printf("Error opening directory.\n");
        return 1;
    }

    // While entry read from the directory not null
    while ((entry = readdir(directory)) != NULL) { 
        // print name of the file/directory
        printf("%s\n", entry->d_name);
    }

    //close directory
    if (closedir(directory) == -1) {
        printf("Error closing directory.\n");
        return 1;
    }

    return 0;
}

int main() {
    int err = 0;
    if (createDirShellCommand() == 1) { // error
        err++;  // completed with an error
    }

    if (createDirThroughInclude() == 1) {
        err++;
    }

    if (getFilesThroughInclude() == 1) {
        err++;
    }

    if (getFilesShellCommand() == 1) {
        err++;
    }

    return err;
}