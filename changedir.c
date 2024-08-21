#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "major2.h"

/*
Changes the current working directory based on the provided arguments for the 'cd' command.
If no argument is provided, the function changes the directory to the user's home directory.
If an argument is provided, the function attempts to change to that directory.
After each successful directory change, the current working directory is printed.
If the directory change fails, an error message is displayed.
*/
int changedir(char **arguments)
{
    int MAXDIRCHARS = 256;  // Maximum number of characters for the directory path
    char directoryecho[MAXDIRCHARS];  // Buffer to store the current working directory

    // If no argument is provided with the 'cd' command
    if (arguments[1] == NULL) {
        // Change directory to the user's home directory
        chdir(getenv("HOME"));
        printf("cd: returning to user's home directory\n"); // Inform the user
        // Print the new current working directory
        printf("%s\n", getcwd(directoryecho, MAXDIRCHARS));
    } 
    // If an argument is provided with the 'cd' command
    else {
        // Attempt to change to the specified directory
        if (chdir(arguments[1]) == 0) {
            // Print the new current working directory
            printf("%s\n", getcwd(directoryecho, MAXDIRCHARS));
        } 
        else {
            // Print an error message if the directory change fails
            perror("directory change failed: ");
        }
    }

    return 1; // Return 1 to indicate successful execution
}
