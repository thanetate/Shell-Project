# CSCE 3600 Shell Project

In our CSCE 3600 Shell Project, we developed a basic command-line interpreter in C, designed to mimic fundamental shell functionalities. This shell handles user commands interactively and from batch files, allowing for directory changes, command execution, and PATH environment variable management.

## Features
* Interactive Command Execution: The shell prompts users for input, processes commands, and executes them, supporting standard operations like directory changes and external program execution.
* Directory Management: The changedir function facilitates navigation through directories, including returning to the home directory, and provides feedback on directory changes or errors.
* Command Loop: The CommandLoop function processes and executes user commands continuously until termination, managing multiple commands on a single line and executing them sequentially.
* PATH Environment Variable Management: The Path function allows users to view or modify the PATH environment variable, including appending or removing paths, with the help of removeSubstring for path modifications.
* Batch File Execution: Commands can be executed from batch files, reading and processing each line as a separate command.

