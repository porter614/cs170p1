#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

void launch(char **args, int num_args, int gt_found, int lt_found);
void print_command(char **args, int num_args);

int main()
{
	char c;
	char* current_arg = malloc(50*sizeof(char)); //max size of current_arg = 50 chars (arbitrary max)
	char* last_arg = malloc(50*sizeof(char));
	char** command_args = malloc(50*sizeof(char*)); // literally no clue what to set it as
	char** launch_command = malloc(50*sizeof(char*));
	int index = 0, num_args = 0, pipe_indeces[50], pipe_count = 0, gt_found = 0, lt_found = 0;

	printf("\nsish:>");
	while( (c=getchar())!= '~' )
	{	
		if ( c == EOF ) continue;

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
				launch(command_args, num_args, gt_found, lt_found);


				//Reset Variables for next command
				memset(command_args, 0, sizeof command_args);
				num_args = 0; gt_found = 0; lt_found = 0;

				printf("\nsish:>");
			}
			else if( c == ' ') continue; //If we hit a space, reset arg index
			else { // c == | < > &
				if(c == '>') {
					gt_found = 1;					
				}
				else if (c == '<') {
					lt_found = 1;
				}
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


void launch(char **args, int num_args, int gt_found, int lt_found) {	
	int fp = 1, stdio_save;

	if (gt_found) {
		printf("Last arg = %s", args[num_args - 1]);
		fp = open(args[num_args - 1], O_WRONLY); //Open file/get file descriptor
		args[num_args - 1] = '\0'; 				 //Delete file from command so you can run

		stdio_save = dup(1);  //duplicate stdout fd
	    dup2(fp, 1);		   //make stdout point to file pointer
    }
    else if (lt_found) {
    	printf("Last arg = %s\n", args[1]);
    	fp = open(args[1], O_RDONLY); //Open file/get file descriptor
		args[1] = '\0'; 				 //Delete file from command so you can run

		stdio_save = dup(0);  //duplicate stdin fd
	    dup2(fp, 0);		   //make stdout point to file pointer
    }


    pid_t process_id = fork();
	if (process_id == 0) {
		//Child Process
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
		int child_return_status;
		waitpid(process_id, &child_return_status, 0);

		if (child_return_status){
			//error
			printf("Error in child process.");
		}
		else {
			//no error
			if ( gt_found ) {
				dup2(stdio_save, 1);
				close(stdio_save);
				close(fp);
			}
			else if ( lt_found ) {
				dup2(stdio_save, 0);
				close(stdio_save);
				close(fp);
			}
			printf("Child process finished with return status %d\n", child_return_status);

		}
	}
}

void launch_pipe(char **args1, char **args2) {	
	// int pipe_fd[2];

	// if (pipe(pipefd) == -1) {
 //        perror("pipe");
 //        exit(EXIT_FAILURE);
 //    }
 //    pid_t p_id = fork();

 //    if (process_id == 0) {
	// 	//Child Process
	// 	if (execvp(args[0], args) == -1) {
	// 	//if (execvp("/bin/sh", NULL) == -1){
	// 	    printf("Unknown command\n");
	// 	    exit(0);
	// 	}
	// }
	// else if (process_id < 0) {
	// 	//error
	// }
	// else {
	// 	//Parent Process
	// 	printf("Hello from Parent\n");
	// 	int child_return_status;
	// 	waitpid(process_id, &child_return_status, 0);

	// 	if (child_return_status){
	// 		//error
	// 		printf("Error in child process.");
	// 	}
	// 	else {
	// 		//no error
	// 		printf("Child process finished with return status %d\n", child_return_status);
	// 	}
	// }



}



void print_command(char **args, int num_args) {
	for (int i = 0; i < num_args; i++)
		printf("%s\n", args[i]);
	printf("\n");
}