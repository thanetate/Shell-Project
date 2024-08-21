#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "major2.h"

/*
Splits a line of text into individual arguments based on spaces.
The 0th index of the returned array contains the command, 
and subsequent indices contain the arguments for that command.
*/
char** SplitLineIntoArguments(char *line)
{
    int position = 0;   // Current index in the arguments array
    const char delim[2] = " ";   // Delimiter for splitting arguments
    char **arguments = malloc(64 * sizeof(char*));  // Array to store arguments
    char *argument;

    // Check if memory allocation was successful
    if (!arguments)
    {
        fprintf(stderr, "Memory allocation error\n");
        exit(1);
    }

    // Tokenize the line using the space delimiter
    argument = strtok(line, delim);

    // Loop through the line and split arguments
    while (argument != NULL)
    {
        arguments[position] = argument;  // Store the current argument
        position++;
        // Get the next argument
        argument = strtok(NULL, delim);
    }

    // Null-terminate the array of arguments
    arguments[position] = NULL;
    
    return arguments;
}

/*
Splits a line of text into separate segments based on the pipe delimiter '|'.
This function is used to handle pipelined commands.
*/
char** SplitLineWithPipes(char *line)
{
    int position = 0;   // Current index in the arguments array
    const char delim[2] = "|";   // Delimiter for splitting commands
    char **arguments = malloc(64 * sizeof(char*));  // Array to store segments
    char *argument;

    // Check if memory allocation was successful
    if (!arguments)
    {
        fprintf(stderr, "Memory allocation error\n");
        exit(1);
    }

    // Tokenize the line using the pipe delimiter
    argument = strtok(line, delim);

    // Loop through the line and split segments
    while (argument != NULL)
    {
        arguments[position] = argument;  // Store the current segment
        position++;
        // Get the next segment
        argument = strtok(NULL, delim);
    }

    // Null-terminate the array of segments
    arguments[position] = NULL;
    
    return arguments;
}

/*
Splits a line of text into segments for input redirection based on the '<' delimiter.
The function divides the line into the command and the target file for input redirection.
*/
char** SplitLineWithInputRed(char *line)
{
    int position = 0;   // Current index in the arguments array
    const char delim[2] = "<";   // Delimiter for splitting input redirection
    char **arguments = malloc(64 * sizeof(char*));  // Array to store segments
    char *argument;

    // Check if memory allocation was successful
    if (!arguments)
    {
        fprintf(stderr, "Memory allocation error\n");
        exit(1);
    }

    // Tokenize the line using the input redirection delimiter
    argument = strtok(line, delim);

    // Loop through the line and split segments
    while (argument != NULL)
    {
        arguments[position] = argument;  // Store the current segment
        position++;
        // Get the next segment
        argument = strtok(NULL, delim);
    }

    // Null-terminate the array of segments
    arguments[position] = NULL;
    
    return arguments;
}

/*
Splits a line of text into segments for output redirection based on the '>' delimiter.
The function divides the line into the command and the target file for output redirection.
*/
char** SplitLineWithOutputRed(char *line)
{
    int position = 0;   // Current index in the arguments array
    const char delim[2] = ">";   // Delimiter for splitting output redirection
    char **arguments = malloc(64 * sizeof(char*));  // Array to store segments
    char *argument;

    // Check if memory allocation was successful
    if (!arguments)
    {
        fprintf(stderr, "Memory allocation error\n");
        exit(1);
    }

    // Tokenize the line using the output redirection delimiter
    argument = strtok(line, delim);

    // Loop through the line and split segments
    while (argument != NULL)
    {
        arguments[position] = argument;  // Store the current segment
        position++;
        // Get the next segment
        argument = strtok(NULL, delim);
    }

    // Null-terminate the array of segments
    arguments[position] = NULL;
    
    return arguments;
}
