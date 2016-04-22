#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

//*****************************************************************************
//**           struct List: 											   ****
//**                an array of these makes an array of array of strings   ****
//*****************************************************************************
struct List 
{
	int count; // the number of strings in the below array
	char** the_list; // an array of strings
};

int lt_found = 0, gt_found = 0, bg_found = 0;
//*****************************************************************************
//**                      function definitions                             ****
//*****************************************************************************
int sep_by_pipe(struct List* res, char** command_args, int* pipes, int NO_PIPES, int NO_ARGS );
void list_insert( struct List* myList, char* str );
void launch(char **args, int num_args, int in, int out);
void print_command(char **args, int num_args);
void create_file(char *args);

//*****************************************************************************
//**           main: 											           ****
//**             launches shell, grabs commands, runs them                 ****
//*****************************************************************************
int main()
{
	// variables to be used throughout
		char c;
		char* current_arg = malloc(50*sizeof(char)); //max size of current_arg = 50 chars (arbitrary max)
		char** command_args = malloc(50*sizeof(char*)); // literally no clue what to set it as
		int index = 0, num_args = 0;
		int MAX_ARG_POSSIBLE = 1024, NO_PIPES = 0;
	//this array will store all possible locations of pipes, -1 signifies end of pipe list
		int pipe_locations[ MAX_ARG_POSSIBLE ], n = 0;
		for(n = 0; n < MAX_ARG_POSSIBLE ; n++) { pipe_locations[n] = -1; }

	//printing the shell prompt
		printf("\nsish:>");
	//interpret character-by-character 
		while( (c=getchar())!= EOF )
		{	
			//if (c == EOF) 
			//	exit(0);
			if( c == '|' || c=='<' || c=='>' || c == ' ' || c == '&' || c == '\n')
			{	
			// Reached the end of a token, time to store argument that has been built
				if( index != 0 ) 
				{	
				// Null terminate the token
					current_arg[index] = '\0';  

				// Check if command entered is "exit" if so exit shell
					if (num_args == 0 && strcmp(current_arg, "exit") == 0)
						exit(1);

				// Reset current_arg index for next arg rebuild
					index = 0; 

				//allocate space for argument, store it
					command_args[ num_args ] = malloc(50*sizeof(char*));
					strcpy(command_args[ num_args ], current_arg);

				//clear the current arg buffer, increment the total argument count 
					memset(current_arg, 0, 1*sizeof(current_arg));
					num_args += 1;
			}	

			// if a newline is reached, the entry is finished and needs to be run
				if (c == '\n') {

				// add the null-terminating character for exec
					command_args[ num_args ] = '\0';
					//print_command(command_args, num_args);

				// pass in command arugments and pipe-index-array, returns list of list sep by pipes
				// a list array will be returned
					struct List res[100];
				// ... List Init
					int i,j;
					for( i = 0; i < 100; i++)
					{
						res[i].count = 0;
						// char**
						res[i].the_list = malloc(50*sizeof(char*));

						for (j = 0; j < 100; j++)
						{	
							// char*
							res[i].the_list[j] = malloc(50*sizeof(char));
						}
						
					}

				// separate the input arguments by pipes
					int list_number = sep_by_pipe(res, command_args, pipe_locations, NO_PIPES, num_args );

				//fork/execute progam
					int in = 0, fd[2], save = dup(1);
					for(i = 0; i <= list_number; i++)
					{
						// printf("----------------------------\n");
						if (i == list_number) {
							dup2(save, 1);
							launch(res[i].the_list, res[i].count, in, 1);
							break;
						}

						pipe(fd);
						//launch(command1[i], 3, 0, 1);
						launch(res[i].the_list, res[i].count, in, fd[1]);
						close(fd[1]);
						in = fd[0];
					}	

				//Reset Variables for next command
					memset(command_args, 0, 1*(sizeof command_args));
					num_args = 0; gt_found = 0; lt_found = 0, bg_found = 0;
				//reset the pipecount and pipe-index-array
					for(n = 0; n < MAX_ARG_POSSIBLE ; n++) { pipe_locations[n] = -1; }
					NO_PIPES = 0;
					printf("\nsish:>");
			}
			// if space is reached, do nothing			
				else if( c == ' ') continue; 
			// if c is an operator
				else { 
				// if pipe, add the index of command_args to the pipe_locations array
					if( c == '|') {	
						pipe_locations[ NO_PIPES++  ] = num_args-1;
					}	
					if(c == '>') {
						gt_found = 1;					
					}
					else if (c == '<') {
						lt_found = 1;
					}
					else if (c == '&') {
						bg_found = 1;
					}
				}
			}
		//Reached normal character
			else 
			{	
				current_arg[index] = c;
				index += 1;	
			}
		}
		return 0;
}


//*****************************************************************************
//**           Launch function:	     									   ****
//**                                                                       ****
//*****************************************************************************
void launch(char **args, int num_args, int in, int out) {
	// printf("iNSIDE launch\n");
	// printf("entered in: %d \n", in);
	// printf("entered out: %d \n", out);
	args[num_args] = '\0';

	int fp = 1, stdio_save;

	if (gt_found) {
		//printf("Last arg = %s", args[num_args - 1]);
		create_file(args[num_args -1]);

		fp = open(args[num_args - 1], O_WRONLY); //Open file/get file descriptor
		args[num_args - 1] = '\0'; 				 //Delete file from command so you can run

		stdio_save = dup(1);  //duplicate stdout fd
	    dup2(fp, 1);		   //make stdout point to file pointer
    }
    else if (lt_found) {
    	//printf("Last arg = %s\n", args[1]);
    	fp = open(args[1], O_RDONLY); //Open file/get file descriptor
		args[1] = '\0'; 				 //Delete file from command so you can run

		stdio_save = dup(0);  //duplicate stdin fd
	    dup2(fp, 0);		   //make stdout point to file pointer
    }

    //Fork a process
    pid_t process_id = fork();
	if (process_id == 0) {
		//Child Process
		if (in != 0) {
			dup2 (in, 0);
			close (in);
		}
		if (out != 1) {
			dup2 (out, 1);
			close (out);
		}

		if (execvp(args[0], args) == -1) {
		//if (execvp("/bin/sh", NULL) == -1){
		    printf("ERROR: Exec failed, unknown command\n");
		    exit(0);
		} 
	}
	else if (process_id < 0) {
		//error
		printf("ERROR: While forking process...\n");
		exit(0);
	}
	else {
		//Parent Process
		int child_return_status = 0;

		if (bg_found == 0)
			waitpid(process_id, &child_return_status, 0);

		if (child_return_status){
			//error
			printf("ERROR: in child process...\n");
		}
		else {
			//no error
			//printf("Child process finished with return status %d\n", child_return_status);
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
		}
	}
}


//*****************************************************************************
//**           Creates the file to pipe output to:						   ****
//**                                                                       ****
//*****************************************************************************
void create_file(char *args) {
	FILE *fp = fopen(args, "w+");
	fclose(fp);
}

//*****************************************************************************
//**           Prints command to be run:								   ****
//**                                                                       ****
//*****************************************************************************
void print_command(char **args, int num_args) {
	for (int i = 0; i < num_args; i++)
		printf("%s\n", args[i]);
	printf("\n");
}

//*****************************************************************************
//**           Adds string to a List object:     						   ****
//**                                                                       ****
//*****************************************************************************
void list_insert( struct List* myList, char* str )
{
	strcpy ( myList->the_list[ myList->count ], str) ;
	myList->count += 1;
}

//*****************************************************************************
//**           Takes input commands & separates it by pipes 			   ****
//**                                                                       ****
//*****************************************************************************
int sep_by_pipe(struct List* res, char** command_args, int* pipes, int NO_PIPES, int NO_ARGS )
{	
	//forloop iterators
	int i = 0;
	int j = 0;
	// int list_len[1024];  // keeps track of how many terms are in each array of the array of arrays
	int list_number = -1;     //keeps track of which list it is in the tbr array
							  // note, list_number is -1 from the actual # of lists (to start filling at result[0])
	int start_index = -1;
	int stop_index = pipes[0];
	for( i = 0 ; i < NO_PIPES; i++ )
	{
		list_number += 1;

		for(j = start_index+1; j <= stop_index; j++)
		{	
			list_insert( &res[list_number], command_args[j] );

			//printf("%s, ", command_args[j]);
		}
		//printf("\n");
		start_index = stop_index;
		stop_index = pipes[i+1];
	}
	if( start_index !=  NO_ARGS )
	{
		list_number += 1;
		for(i = start_index+1; i < NO_ARGS; i++)
		{
			list_insert( &res[list_number], command_args[i] );

			//printf("%s, ", command_args[i]);
		}
	}

	return list_number;
}

