#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "major2.h"

/*
This helper function removes a specified substring from a given string and returns the updated string.
It can be used to remove a particular path segment from the PATH environment variable.
*/
char *removeSubstring(char *line, const char *substring)
{
    // Allocate memory for the updated substring
    char *updatedSubstring = malloc(strlen(substring) + 2);
    if (!updatedSubstring) {
        perror("Memory allocation error");
        return NULL;
    }
    
    // Clear the memory
    memset(updatedSubstring, 0, strlen(substring) + 2);

    // Create a copy of the line for processing
    char *tempLine = (char*)malloc(strlen(line) + 1);
    if (!tempLine) {
        perror("Memory allocation error");
        free(updatedSubstring);
        return NULL;
    }
    strcpy(tempLine, line);

    char *preTemp = tempLine;
    preTemp = strstr(preTemp, substring);

    // Handle the case where the substring is at the beginning or end of the line
    if ((preTemp - tempLine) == 0) {
        // Substring is at the beginning
        strcpy(updatedSubstring, substring);
        strcat(updatedSubstring, ":");
    } else {
        // Substring is at the end or somewhere in the middle
        strcpy(updatedSubstring, ":");
        strcat(updatedSubstring, substring);
    }

    char *p, *q, *r;

    // Remove the substring from the line
    if (*updatedSubstring && (q = r = strstr(line, updatedSubstring)) != NULL) {
        size_t length = strlen(updatedSubstring);
        // Remove all occurrences of the substring
        while ((r = strstr(p = r + length, updatedSubstring)) != NULL) {
            while (p < r)
                *q++ = *p++;
        }
        while ((*q++ = *p++) != '\0')
            continue;
    } else {
        perror("Error removing substring from path. Either incorrect input or memory allocation issue");
    }

    free(tempLine);
    free(updatedSubstring);
    return line;
}

/*
This function modifies the PATH environment variable based on the provided arguments:
0 - Display the current PATH
1 - Print an error message for improper usage
2 - Append or remove a path segment based on the provided flag (+ or -) and path

Returns:
1 - Success
-1 - Failure (e.g., incorrect arguments, memory allocation error)
*/
int Path(char **arguments)
{
    int i = 0;
    int isAdd;

    // Count the number of arguments and determine if a valid flag is provided
    while (arguments[i]) {
        if (i == 1) {
            isAdd = (strcmp(arguments[i], "+") == 0);
            if (!isAdd && strcmp(arguments[i], "-") != 0) {
                fprintf(stderr, "Usage: %s [- or +] path\n", arguments[0]);
                return -1;
            }
        }
        i++;
    }

    char *modPath;

    // Switch based on the number of arguments
    switch (i) {
        case 1:
            // Just display the current PATH
            printf("%s\n", getenv("PATH"));
            break;
        case 3:
            modPath = arguments[2];
            if (isAdd) {
                // Append the specified path to the PATH environment variable
                char* addToPath = (char*)malloc(strlen(modPath) + 3);
                if (!addToPath) {
                    perror("Memory allocation error");
                    return -1;
                }
                memset(addToPath, 0, strlen(modPath) + 3);
                strcat(addToPath, ":");
                strcat(addToPath, modPath);

                // Reallocate and append to localPath
                char* temp = (char*)realloc(localPath, strlen(localPath) + strlen(addToPath) + 1);
                if (!temp) {
                    perror("Memory allocation error");
                    free(addToPath);
                    return -1;
                }
                localPath = temp;
                strcat(localPath, addToPath);
                free(addToPath);
            } else {
                // Remove the specified path from localPath
                localPath = removeSubstring(localPath, modPath);
            }

            // Update the PATH environment variable
            char* newPath = (char*)malloc(strlen(localPath) + 8);
            if (!newPath) {
                perror("Memory allocation error");
                return -1;
            }
            strcpy(newPath, "PATH=");
            strcat(newPath, localPath);
            putenv(newPath);
            break;
        default:
            // Print usage message for invalid arguments
            fprintf(stderr, "Usage: %s [+ or -] path\n", arguments[0]);
            return -1;
    }
    return 1;
}
