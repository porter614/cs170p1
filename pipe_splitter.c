#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>


char** sep_by_pipe(  );


int main()
{

	// char** command_args = malloc(50*sizeof(char*));
	// command_args[ num_args ] = malloc(50*sizeof(char*));
	// strcpy(command_args[ num_args ], current_arg);

	int pipes[2] = {1,3};

	char* command_args[ 5 ] = { "firstArg", "secondArg", "ThirdArg", "FourthArg", "FifthArg" };

}

