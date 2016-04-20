#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>


struct List
{
	int count;
	char** the_list;
	// char* the_list[1024];
};

void sep_by_pipe(struct List* res, char** command_args, int* pipes, int NO_PIPES, int NO_ARGS );
void list_insert( struct List* myList, char* str );

int main()
{
	int NO_PIPES = 2;
	int pipes[ 2 ] = {1,3};
	
	int NO_ARGS = 5;
	char* command_args[ 5 ] = { "arg1", "arg2", "arg3", "arg4", "arg5" };

	struct List res[100];	
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

	sep_by_pipe(res, command_args, pipes, NO_PIPES, NO_ARGS );

	return 0;

}

void list_insert( struct List* myList, char* str )
{
	strcpy ( myList->the_list[ myList->count ], str) ;

	printf("Just added %s to the List@ count %d\n", str, myList->count );

	myList->count += 1;

	printf("New Count Value: %d\n", myList->count);

}


void sep_by_pipe(struct List* res, char** command_args, int* pipes, int NO_PIPES, int NO_ARGS )
{	

	//forloop iterators

	int i = 0;
	int j = 0;
	int k = 0;

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

			printf("%s, ", command_args[j]);
		}

		printf("\n");

		start_index = stop_index;
		stop_index = pipes[i+1];
	}



	if( start_index !=  NO_ARGS )
	{
		list_number += 1;
		for(i = start_index+1; i < NO_ARGS; i++)
		{
			list_insert( &res[list_number], command_args[i] );

			printf("%s, ", command_args[i]);
		}
	}
	printf("\n");

	for(i = 0; i <= list_number; i++)
	{
		printf("%d\n", res[i].count);

		for(j = 0; j < res[i].count; j++)
		{
			printf("%s\n", res[i].the_list[j]);
		}

		printf("----------------------------\n");
	}	




	
}

