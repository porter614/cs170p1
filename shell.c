#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER 100

void parse_path();
void parse_input(char * buff);
int count_it( char* str, char del );

char** paths;
int c;

int main(int argc, char *argv[])
{
	parse_path();
 
	char buffer [ BUFFER ];
	printf("\nsish:>");
	while(fgets(buffer, BUFFER, stdin) != NULL)
	{	
		parse_input(buffer);
		printf("\nsish:>");

	}

	


	return 0;
}

int count_it( char* str, char del )
{
	int i = 0;
	if( !str )  {i = 0;}
	else
	{
		for (i = 0; str[i]; str[i]==del ? i++ : *str++);
		i++;
	}
	return i;
}

void parse_path()
{
	char* getnum = getenv("PATH");
	char*p = getenv("PATH");
	int i; //include the first and last paths

	i = count_it( getnum, ':' );

	// printf("%d\n", i);
	paths = malloc(i*sizeof(char*));

	char* init;
	init = strtok(p, ":");
	int count = 0;

	while( init != NULL )
	{
		paths[count] = init; 
		count+=1;
		init = strtok(NULL, ":");
	}
}

void parse_input(char * buff)
{
	char** args;
	char buff_copy[100];

	strcpy(buff_copy, buff);


	int num_args = count_it(buff_copy, ' ');
	args = malloc(num_args*sizeof(char*));

	printf("%d\n", num_args);
	int i = 0;
	for (; i < num_args; i++) args[i] = malloc(128*sizeof(char));
	i = 0;

	args[i] = strtok(buff, " ");
	while(i < num_args)
	{
		// printf("%s\n", args[i]);
		i+=1;
		args[i] = strtok(NULL, " ");
		
	}
	// printf("%s\n", buff);
}

//function in: input, out: corrected string

// separate string into array of tokens/operators

// recreate string of array concat. by spaces

