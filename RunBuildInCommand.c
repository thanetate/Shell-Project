#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "major2.h"

//substring function
char* substr(const char *src, int m, int n)
{
    int len = n-m;
    char *dest = (char*)malloc(sizeof(char)*(len + 1));
    for (int i=m; i<n && (*(src+i) != '\0'); i++)
    {
        *dest = *(src+i);
        dest++;
    }
    *dest = '\0';
    return dest-len;
}

/*
This command should be invoked before executing any other commands. 
It checks if the cmd argument corresponds to a built-in command (such as path, cd, or myhistory). 
If it is a built-in command, the appropriate function is executed. If the cmd is not a built-in command,
the function returns 0. In the event of a failure, it returns -1.
*/

//Return -1 if failed
int RunBuildInCommand(char * cmd, char ** arguments)
{
    //Check to see if cmd is: path, exit, or cd
    if(strcmp(cmd, "exit") == 0)
    {
        printf("Running exit command\n");
        // Run the exit command using the arguments
        exit(0); // Exit 0 for successful execution
    }
    else if(strcmp(cmd, "path") == 0)
    {
        //Run the path command
        return Path(arguments);
    }
    else if(strcmp(cmd, "cd") == 0)
    {
        //printf("Running cd command\n");
        changedir(arguments);
        return 1;
        //Run the cd command
    }
    else if(strcmp(cmd, "kill") == 0) {
        // Find length of -XXX command (XXX is the signal integer)
        int length = strlen(arguments[1]);
        //Separate the "-", leaving XXX
        char* signalChar = substr(arguments[1], 1, length);
        int signalInt = atoi(signalChar);
        int pidInt = atoi(arguments[2]);
        return killpg(pidInt, signalInt);
    }
    else
    {
        //Return that the cmd was not a built in command
        return 0;
    }
}
