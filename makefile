newshell: major2.o changedir.o SplitLineIntoArguments.o SplitCommandsInLine.o RunCommand.o RunBuildInCommand.o Path.o InteractiveMode.o
	gcc -o newshell major2.o changedir.o SplitLineIntoArguments.o SplitCommandsInLine.o RunCommand.o RunBuildInCommand.o Path.o InteractiveMode.o
major2.o: major2.c major2.h
	gcc -c major2.c major2.h
changedir.o: changedir.c
	gcc -c changedir.c
SplitLineIntoArguments.o: SplitLineIntoArguments.c
	gcc -c SplitLineIntoArguments.c
SplitCommandsInLine.o: SplitCommandsInLine.c
	gcc -c SplitCommandsInLine.c
RunCommand.o: RunCommand.c
	gcc -c RunCommand.c
RunBuildInCommand.o: RunBuildInCommand.c
	gcc -c RunBuildInCommand.c
Path.o: Path.c
	gcc -c Path.c
InteractiveMode.o: InteractiveMode.c
	gcc -c InteractiveMode.c