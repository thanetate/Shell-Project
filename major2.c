#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include "major2.h"

// Global variables for managing the PATH environment variable and batch file execution
char* localPath;
char* originalPath;
char* pathSuffix;
char *const environment[4];
int environmentPosition;

int main(int argc, char *argv[])
{
    // Retrieve and copy the PATH environment variable
    char* path = getenv("PATH");
    localPath = (char*) malloc(strlen(path) + 1);
    originalPath = (char*) malloc(strlen(path) + 1);
    pathSuffix = (char*) malloc(strlen(path) + 8);

    // Check if memory allocation was successful
    if(!localPath || !originalPath || !pathSuffix)
    {
        perror("Error allocating memory");
        exit(1);
    }

    // Initialize local path variables and append "PATH=" to pathSuffix
    strcpy(pathSuffix, "PATH=");
    environmentPosition = 0;
    strcpy(localPath, path);
    strcpy(originalPath, path);

    // Check if a batch file argument was provided
    if(argc == 2)
    {
        FILE* batchFile;
        // Open the batch file specified by the argument
        batchFile = fopen(argv[1], "r");
        // Check if the file opened successfully
        if(batchFile == NULL)
        {
            perror("File doesn't exist");
            exit(1);
        }
        char line[512];
        // Variables for reading and executing commands from the batch file
        char* command;
        char** commands;
        char** arguments;

        // Read and process each line from the batch file
        while(fgets(line, sizeof(line), batchFile))
        {
            // Remove the newline character from the end of the line
            line[strlen(line) - 1] = 0;
            printf("Line: %s\n", line);
            // Split the line into individual commands based on semicolons
            commands = SplitCommandsInLine(line);
            // Execute each command in the line
            int position = 0;
            while(commands[position] != NULL)
            {
                RunCommand(commands[position]);
                position++;
            }
        }

        // Close the batch file after processing
        fclose(batchFile);
    }
    else if (argc == 1) {
        // No batch file provided, start the interactive command loop
        if (CommandLoop() == -1) {
            perror("Command loop failed");
        }
    }
    else {
        // Print an error message for invalid arguments
        printf("Too many arguments, 1 optional argument: batchfilename\n");
        return -1;
    }
    
    // Restore the original PATH environment variable and free allocated memory
    putenv(strcat(pathSuffix, originalPath));
    free(localPath);
    free(originalPath);
    free(pathSuffix);
}
