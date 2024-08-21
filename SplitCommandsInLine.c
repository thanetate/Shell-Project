#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "major2.h"

/*
Splits a line of text into separate commands based on a semicolon delimiter.
This function allows processing of multiple commands within a single line, 
where commands are separated by semicolons.
*/
char** SplitCommandsInLine(char *line)
{
    int position = 0;   // Current index in the arguments array
    const char delim[2] = ";";   // Delimiter for splitting commands
    char **arguments = malloc(64 * sizeof(char*));  // Array to store individual commands
    char *argument;

    // Check if memory allocation was successful
    if (!arguments)
    {
        fprintf(stderr, "Memory allocation error\n");
        exit(1);
    }

    // Tokenize the line using the delimiter
    argument = strtok(line, delim);

    // Loop through the line and split commands
    while (argument != NULL)
    {
        arguments[position] = argument;  // Store the current command
        position++;
        // Get the next command
        argument = strtok(NULL, delim);
    }

    // Null-terminate the array of commands
    arguments[position] = NULL;
    
    return arguments;
}
