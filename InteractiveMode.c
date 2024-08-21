#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include "major2.h"

// Buffer size for reading input
#define BUFFSIZE 512

/*
Reads a line of input from the user.
Allocates a buffer to store the input and dynamically resizes it if necessary.
The function handles input until a newline or EOF is encountered.
Returns the input line as a dynamically allocated string.
*/
char *readLine(void)
{
    int bufsize = BUFFSIZE; // Initial buffer size
    int position = 0; // Current position in the buffer
    char *buffer = malloc(sizeof(char) * bufsize); // Allocate buffer
    int c; // Character read from input

    if (!buffer) {
        perror("allocation error");
        return NULL;
    }

    while (1) {
        // Read a character from input
        c = getchar();

        // If EOF or newline is encountered, terminate the string and return
        if (c == EOF || c == '\n') {
            buffer[position] = '\0';
            return buffer;
        } else {
            buffer[position] = c; // Store character in buffer
        }
        position++;

        // If buffer size is exceeded, print error and return NULL
        if (position >= bufsize) {
            perror("Too many characters in input");
            return NULL;
        }
    }
}

/*
Main command loop that continuously reads, parses, and executes user input.
Displays the current working directory followed by a prompt.
Processes input lines by splitting them into individual commands and executing them.
Handles errors during command execution and cleans up dynamically allocated memory.
*/
int CommandLoop() {
    char **cmds; // Array of commands parsed from input
    char *line; // User input line

    while (1)
    {
        int MAXDIRCHARS = 256; // Maximum number of characters for directory path
        char directoryecho[MAXDIRCHARS]; // Buffer for current working directory
        // Print prompt with current working directory
        printf("%s$ ", getcwd(directoryecho, MAXDIRCHARS));

        // Read a line of input from the user
        line = readLine();
        
        if (!line) {
            perror("Error reading input");
            continue;
        }

        // Split the input line into commands
        cmds = SplitCommandsInLine(line);

        // Execute each command
        int i = 0;
        while (cmds[i] != NULL) {
            if (RunCommand(cmds[i]) == -1) {
                perror("Error running command");
            }
            i++;
        }

        // Free allocated memory
        free(line);
        free(cmds);
    }

    // This point is never reached, but return 0 for completeness
    return 0;
}
