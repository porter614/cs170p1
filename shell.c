#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void launch(char **args);
void print_command(char **args, int num_args);

int main()
{
	char c;
	char* current_arg = malloc(50*sizeof(char*)); //max size of current_arg = 50 chars (arbitrary max)
	char** command_args = malloc(1000*sizeof(char*)); // literally no clue what to set it as
	int index = 0, num_args = 0;

	printf("\nsish:>");
	while( (c=getchar())!= EOF )
	{	
		if( c == '|' || c=='<' || c=='>' || c == ' ' || c == '&' || c == '\n')
		{	
			if( index != 0 ) // Reached the end of a token, time to store
			{
				current_arg[index] = '\0';  // Null terminate the token
				index = 0; // Reset current_arg index

				command_args[ num_args ] = malloc(50*sizeof(char*));
				strcpy(command_args[ num_args ], current_arg); //store current_arg

				memset(current_arg, 0, sizeof current_arg); //clear the current arg buffer
				num_args += 1; //increment number of arguments
			}	

			if (c == '\n') {
				command_args[ num_args ] = '\0';

				print_command(command_args, num_args);
				//fork/execute progam
				launch(command_args);


				//Reset Variables for next command
				memset(command_args, 0, sizeof command_args);
				num_args = 0;

				printf("\nsish:>");
			}
			else if( c == ' ') continue; //If we hit a space, reset arg index

			else { // c == | < > &
				char* charstr = malloc(2*sizeof(char*));
				charstr[0] = c;
				charstr[1] = '\0';

				command_args[ num_args ] =  malloc(2*sizeof(char*));
				strcpy(command_args[ num_args ], charstr); //store the operator as a current_arg
				num_args += 1;
				free(charstr);
			}
		}
		else //Reached normal character
		{	
			current_arg[index] = c;
			index += 1;	
		}
	}
	return 0;
}


void launch(char **args) {
	pid_t process_id = fork();	
	int status;

	if (process_id == 0) {
		//Child Process
		printf("Hello from Child\n");

		if (execvp(args[0], args) == -1) {
		//if (execvp("/bin/sh", NULL) == -1){
		    printf("Unknown command\n");
		    exit(0);
		}
	}
	else if (process_id < 0) {
		//error
	}
	else {
		//Parent Process
		printf("Hello from Parent\n");
		int child_return_status;
		waitpid(process_id, &child_return_status, 0);

		if (child_return_status){
			//error
			printf("Error in child process.");
		}
		else {
			//no error
			printf("Child process finished with return status %d\n", child_return_status);
		}
	}
}


void print_command(char **args, int num_args) {
	for (int i = 0; i < num_args; i++)
		printf("%s\n", args[i]);
	printf("\n");
}