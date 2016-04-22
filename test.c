#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

void launch(char **args, int num_args, int gt_found, int lt_found, int in, int out);
void print_command(char **args, int num_args);

int main()
{
	int in = 0, fd[2], save, num_commands = 3;



	// const char *ls[] = { "ls", "-l", 0 };
	// const char *awk[] = { "awk", "{print $1}", 0 };
	// const char *sort[] = { "sort", 0 };
	// const char *uniq[] = { "uniq", 0 };

	// struct command cmd [] = { {ls}, {awk}, {sort}, {uniq} };

	save = dup(1);

	char *command1[5][5] =  {{ "ls", "-l", 0 },{ "awk","{print}", 0 },{ "sort", 0 }};
	//char *command1[5][5] =  {{"./input",'\0'}, {"./input",'\0'}, {"./input",'\0'}};
	printf("TEST: %d\n", sizeof(command1)/sizeof(char *));

	for (int i = 0; i < num_commands; i++) {
		if (i == num_commands - 1) {
			dup2(save, 1);
			launch(command1[num_commands - 1], 2, 0, 0, in, 1);
			break;
		}

		pipe(fd);
		//launch(command1[i], 3, 0, 1);
		launch(command1[i], 2, 0, 0, in, fd[1]);
		close(fd[1]);
		in = fd[0];
	}

	return 0;
}




void launch(char **args, int num_args, int gt_found, int lt_found, int in, int out) {
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
