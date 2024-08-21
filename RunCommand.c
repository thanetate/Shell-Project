#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "major2.h"

/*
Executes a command in a child process and sets the child's process group ID to its own PID.
This setup allows proper signal handling and the functionality of the kill command.
*/
int runChildCommand(char **command)
{
    // Execute the command using execvp
    setpgid(0, 0);
    if (execvp(command[0], command) == -1)
    {
        fprintf(stderr, "Error executing the %s command\n", command[0]);
        return -1;
    }
}

// Executes a command with input or output redirection based on the provided flags
int executeRedirection(char* command, int isInputRedirection, int isOutputRedirection)
{
    int ifp, ofp;
    char** redir_args;  // Array to store split command line arguments for redirection
    char** cmd_arg_split;   // Array to store split command and arguments before redirection

    // Handle input redirection if flagged
    if (isInputRedirection == 1)
    {
        // Split command line for input redirection
        redir_args = SplitLineWithInputRed(command);
        ifp = open(redir_args[1], O_RDONLY);

        dup2(ifp, STDIN_FILENO);
        close(ifp);

        // Split the command and arguments
        cmd_arg_split = SplitLineIntoArguments(redir_args[0]);

        // Run the command with input redirection
        runChildCommand(cmd_arg_split);
    }

    // Handle output redirection if flagged
    if (isOutputRedirection == 1)
    {
        // Split command line for output redirection
        redir_args = SplitLineWithOutputRed(command);
        ofp = open(redir_args[1], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);

        dup2(ofp, STDOUT_FILENO);
        close(ofp);

        // Split the command and arguments
        cmd_arg_split = SplitLineIntoArguments(redir_args[0]);

        // Run the command with output redirection
        runChildCommand(cmd_arg_split);
    }
}

/*
Parses a command line containing pipes and executes commands with pipelining.
Returns -1 on failure.
*/
int executePipeline(char* command)
{
    // Split the command line based on pipes
    char** pipes = SplitLineWithPipes(command);
    int iterator = 0;
    int numCommands = 0;
    int status;

    // Count the number of pipe-separated commands
    while (pipes[iterator] != NULL)
    {
        iterator++;
        numCommands++;
    }
    printf("numCommands %d\n", numCommands);

    // Initialize variables for command arguments and pipe file descriptors
    char **commandOneArgs, **commandTwoArgs, **commandThreeArgs;
    int child, childTwo, childThree;
    int pipeOne, pipeTwo;

    switch (numCommands)
    {
        case 2:
            // Handle a two-command pipeline
            commandOneArgs = SplitLineIntoArguments(pipes[0]);
            commandTwoArgs = SplitLineIntoArguments(pipes[1]);

            // Create and set up the first pipe
            int pipeFD[2];
            pipeOne = pipe(pipeFD);
            if (pipeOne == -1)
            {
                perror("error making pipe");
                return -1;
            }

            // Fork to execute the first command
            child = fork();
            if (child == -1)
            {
                perror("error forking process");
                return -1;
            }
            else if (child == 0)
            {
                if (close(pipeFD[0]) == -1 || dup2(pipeFD[1], 1) == -1 || close(pipeFD[1]) == -1)
                {
                    perror("error setting up pipe for command one");
                    return -1;
                }

                if (execvp(commandOneArgs[0], commandOneArgs) == -1)
                {
                    fprintf(stderr, "error running cmd %s\n", commandOneArgs[0]);
                }
            }
            else
            {
                // Fork to execute the second command
                childTwo = fork();
                if (childTwo == -1)
                {
                    perror("error forking process");
                    return -1;
                }
                else if (childTwo == 0)
                {
                    if (close(pipeFD[1]) == -1 || dup2(pipeFD[0], 0) == -1 || close(pipeFD[0]) == -1)
                    {
                        perror("error setting up pipe for command two");
                        return -1;
                    }

                    if (execvp(commandTwoArgs[0], commandTwoArgs) == -1)
                    {
                        fprintf(stderr, "error running cmd %s\n", commandTwoArgs[0]);
                    }
                }
            }

            // Close pipe file descriptors and wait for child processes
            close(pipeFD[0]);
            close(pipeFD[1]);
            for (iterator = 0; iterator < 2; iterator++)
            {
                wait(&status);
            }
            break;

        case 3:
            // Handle a three-command pipeline
            commandOneArgs = SplitLineIntoArguments(pipes[0]);
            commandTwoArgs = SplitLineIntoArguments(pipes[1]);
            commandThreeArgs = SplitLineIntoArguments(pipes[2]);

            // Create and set up two pipes
            int fd1[2], fd2[2];
            pipeOne = pipe(fd1);
            pipeTwo = pipe(fd2);
            if (pipeOne == -1 || pipeTwo == -1)
            {
                perror("error making pipes");
                return -1;
            }

            // Fork to execute the first command
            child = fork();
            if (child == -1)
            {
                perror("error forking process");
                return -1;
            }
            else if (child == 0)
            {
                dup2(fd1[1], 1);
                close(fd1[0]);
                close(fd1[1]);
                close(fd2[0]);
                close(fd2[1]);

                if (execvp(*commandOneArgs, commandOneArgs) == -1)
                {
                    fprintf(stderr, "error running cmd %s\n", commandOneArgs[0]);
                }
            }
            else
            {
                // Fork to execute the second command
                childTwo = fork();
                if (childTwo == -1)
                {
                    perror("error forking process");
                    return -1;
                }
                else if (childTwo == 0)
                {
                    dup2(fd1[0], 0);
                    dup2(fd2[1], 1);
                    close(fd1[0]);
                    close(fd1[1]);
                    close(fd2[0]);
                    close(fd2[1]);

                    if (execvp(*commandTwoArgs, commandTwoArgs) == -1)
                    {
                        fprintf(stderr, "error running cmd %s\n", commandTwoArgs[0]);
                    }
                }
                else
                {
                    // Fork to execute the third command
                    childThree = fork();
                    if (childThree == -1)
                    {
                        perror("error forking process");
                        return -1;
                    }
                    else if (childThree == 0)
                    {
                        dup2(fd2[0], 0);
                        close(fd1[0]);
                        close(fd1[1]);
                        close(fd2[0]);
                        close(fd2[1]);

                        if (execvp(*commandThreeArgs, commandThreeArgs) == -1)
                        {
                            fprintf(stderr, "error running cmd %s\n", commandThreeArgs[0]);
                        }
                    }
                }
            }

            // Close all pipe file descriptors and wait for child processes
            close(fd1[0]);
            close(fd1[1]);
            close(fd2[0]);
            close(fd2[1]);
            for (iterator = 0; iterator < 3; iterator++)
            {
                wait(&status);
            }
            break;

        default:
            perror("Too many pipes to handle. Use only 1 or 2 pipes.");
            return -1;
    }
}

/*
Interprets and executes a user-provided command.
The first argument is the command to run, and the second argument is an array of strings containing the command's arguments.
Returns -1 if the command fails to execute.
*/
int RunCommand(char* command)
{
    int isPipe = -1;
    int isInputRedirection = -1;
    int isOutputRedirection = -1;
    int i = 0;

    // Determine the type of command based on the presence of special characters
    if (strstr(command, "|") != NULL)
    {
        isPipe = 1;
    }
    if (strstr(command, "<") != NULL)
    {
        isInputRedirection = 1;
    }
    if (strstr(command, ">") != NULL)
    {
        isOutputRedirection = 1;
    }

    // Handle pipe commands
    if (isPipe == 1)
    {
        if (executePipeline(command) == -1)
        {
            perror("Error executing pipeline");
            return -1;
        }
    }
    // Handle redirection commands
    else if (isInputRedirection == 1 || isOutputRedirection == 1)
    {
        if (executeRedirection(command, isInputRedirection, isOutputRedirection) == -1)
        {
            perror("Error executing redirection");
            return -1;
        }
    }
    else
    {
        // No redirection or pipe, execute the command normally
        char** cmdArgs = SplitLineIntoArguments(command);
        if (cmdArgs == NULL)
        {
            fprintf(stderr, "Error splitting command into arguments\n");
            return -1;
        }

        // Fork and execute the command
        int child = fork();
        if (child == -1)
        {
            perror("error forking process");
            return -1;
        }
        else if (child == 0)
        {
            if (execvp(cmdArgs[0], cmdArgs) == -1)
            {
                fprintf(stderr, "error running cmd %s\n", cmdArgs[0]);
            }
        }
        else
        {
            int status;
            wait(&status);
        }
    }

    return 0;
}
