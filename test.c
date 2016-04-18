#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main()
{
	char c;
	char** output;
	char* term;
	term = malloc(50*sizeof(char*)); //max size of term = 50 chars (arbitrary max)
	output = malloc(1000*sizeof(char*)); // literally no clue what to set it as
	int index = 0;
	int num_of_terms = 0;

	while( (c=getchar())!= EOF )
	{	
		// // if space: ignore
		// if( c == ' ' ) 
		// {
		// 	index = 0; //reset char count for word
		// 	continue; // on to the next one
		// }
		// // build a string unless it is an operator
		// else
		// {	

			if( c == '|' || c=='<' || c=='>' || c == ' ' || c == '\n')
			{	
				// printf("First glimpse C: %c\n", c);

				if( index != 0)
				{
					term[index] = '\0';
					index = 0;
					// printf("New Term = %s\n", term);
					
					output[ num_of_terms ] =  malloc(50*sizeof(char*));
					strcpy(output[ num_of_terms ], term); //store the term
					num_of_terms += 1;
				}	
				// printf("second glimpse C: %s\n", output[0]);
				if (c == '\n') break;
				else if( c == ' ')
				{
					index = 0;

					continue;
				}
				else {
					char* charstr = malloc(2*sizeof(char*));
					charstr[0] = c;
					charstr[1] = '\0';


					output[ num_of_terms ] =  malloc(1*sizeof(char*));
					strcpy(output[ num_of_terms ], charstr); //store the operator as a term
					num_of_terms += 1;
					free(charstr);
				}
			}
			else
			{	
				 // printf("%c\n", c);
				term[index] = c;
				// printf("%s\n", term);

				index += 1;	
			}

		// }	
		
	}





	return 0;
}
