#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER 100

void parse_path();
void parse_input(char * buff);
int count_it( char* str, char del );
void correct_string(char* str);

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

void correct_string(char* str)
{
	//make local copy
	char** corrected_str;
	int corrected_args = 0;

	char** args;
	char copy[100];
	strcpy(copy, str);

	int num_args = count_it(copy, ' ');
	args = malloc(num_args*sizeof(char*)); //2* bcs thats the max num of operators, then search for operators
	corrected_str = malloc(2*num_args*sizeof(char*)); //2* bcs thats the max num of operators, then search for operators

	int i = 0;
	args[i] = strtok(copy, " ");
	while( i < num_args )
	{
		i += 1;
		args[i] = strtok(NULL, " ");
	}

	const char* operators = "<>|";
	int boolean = 0;
	for(i = 0; i < num_args; i++)
	{
		char* c = args[i];
		while( *c )
		{		
			// add the term, operator, term
			if( strchr( operators , *c) )
			{
				// printf("%c is in \"%s\" @index=%d\n", *c, args[i], (int)(c-args[i]) );

				//get command1
				strncpy(corrected_str[corrected_args], args[i], (int)(c-args[i])  );
				corrected_str[corrected_args][(int)(c-args[i])] = '\0';
				corrected_args += 1;
				printf("%s", args[i]);

				//get operator
				int index = (int)(c-args[i]);	
				printf("Index: %d\n", index);
				char* temp = args[i];
				temp = temp+5;
				printf("FromCharacterOn: %s", temp);
				// char subbuff [ strlen(args[i]) - index  ];
				// memcpy( subbuff, args[] )
				// printf("FromCharacterOn: %s", args[i][index]);
				strncpy(corrected_str[corrected_args], temp, 1  );
				corrected_str[corrected_args][1] = '\0';
				corrected_args += 1;	

				//get 2ndcommand if any
				// strncpy(corrected_str[corrected_args], args[i]+(int)(c-args[i])+1, 5 );
				int len_last_term = strlen( args[i] )-(int)( c-args[i] )  ;
				char* mod_str = c+1;
				printf("\nMod: %s\n", mod_str);
				// strncpy(corrected_str[corrected_args], (char*)(c-args[i]) , len_last_term );
				corrected_str[corrected_args] = mod_str;
				corrected_args += 1;				

				boolean = 1;
			}
			c++; //hehe
		}

		if(boolean)
		{
			boolean = 0;
			
		}
		else
		{
			printf("No Arg: %s", args[i]);
			corrected_str[ corrected_args ] = args[i];
			corrected_args+=1;
			
		}

	}//end for

	printf(":: PRINTING CORRECTED ARRAY OF ARGS ::\n\n");
	int j = 0;
	for(; j < corrected_args; j++)
	{
		printf("%s\n", corrected_str[j]);
	}


}//endfunc

