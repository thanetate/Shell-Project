#ifndef MAJOR2_H
#define MAJOR2_H

char** SplitCommandsInLine(char *line);
char** SplitLineIntoArguments(char *line);
char** SplitLineWithPipes(char *line);
int RunBuildInCommand(char * cmd, char ** arguments);
//Returns -1 if failed
int RunCommand(char* command);
//The commands for the built in commands
int Path(char** arguments);
//Interactive Mode Functions
char* ReadLine();
int CommandLoop();
//change directory functions
int changedir(char **arguments);
//Redirection functions
int executeRedirection(char* command, int isInputRedirection, int isOutputRedirection);
char** SplitLineWithInputRed(char *line);
char** SplitLineWithOutputRed(char *line);
//exit function
void exit_shell();


extern char* localPath;
extern char *const environment[4];
extern char* pathSuffix;
extern int environmentPosition;

#endif 
