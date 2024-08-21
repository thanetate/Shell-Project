# CSCE 3600 Shell Project

Overview:
The shell project is a simple command-line interpreter implemented in C, designed to handle user commands, manage the PATH environment variable, and execute scripts from batch files. The shell mimics basic shell functionalities with the ability to change directories, execute commands, and modify the PATH environment variable. It also supports reading commands from an interactive prompt or from a batch file.

Features:

Interactive Command Execution:

The shell continuously prompts the user for input, processes commands, and executes them. It handles standard shell commands such as changing directories and running external programs.
Directory Management:

The changedir function allows users to change the current working directory using the cd command. It supports navigating to the user's home directory if no argument is provided, and it prints the new working directory or an error message if the change fails.
Command Loop:

The CommandLoop function reads user input, parses commands, and executes them in a loop until the shell is terminated. It uses the SplitCommandsInLine function to handle multiple commands on a single line and RunCommand to execute them.
PATH Environment Variable Management:

The Path function allows users to view or modify the PATH environment variable. It supports appending new paths or removing existing ones based on user input. The removeSubstring helper function is used to remove specific path segments from the PATH variable.
Batch File Execution:

The shell can execute commands from a batch file specified as a command-line argument. It reads each line from the file, splits it into commands, and executes them sequentially.
Memory Management:

The project handles dynamic memory allocation for various string operations and ensures proper memory management to avoid leaks and errors.
Error Handling:

The shell includes error handling for memory allocation failures, invalid commands, and issues with file operations. It provides informative error messages to help diagnose problems.
Key Functions:

readLine(): Reads a line of input from the user.
changedir(char **arguments): Changes the current directory or returns to the home directory.
CommandLoop(): Continuously prompts the user for input, parses commands, and executes them.
removeSubstring(char *line, const char *substring): Removes a specified substring from a string.
Path(char **arguments): Modifies or displays the PATH environment variable based on provided arguments.
Usage:

Interactive Mode:

Run the shell executable without arguments to enter interactive mode. The shell will prompt for commands, execute them, and display results or error messages.
Batch File Mode:

Run the shell executable with a batch file name as an argument to execute commands from the file. The shell will process each line in the file as a separate command.
Example Commands:

cd /path/to/directory: Change the current working directory to /path/to/directory.

path + /new/path: Append /new/path to the PATH environment variable.

path - /path/to/remove: Remove /path/to/remove from the PATH environment variable.

./shell batchfile.txt: Execute commands from batchfile.txt.

This shell project provides a foundation for understanding basic shell functionality and can be expanded with additional features and improvements as needed
