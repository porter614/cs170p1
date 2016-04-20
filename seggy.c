#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>


struct List // this should resultingly be an [array of {array of (array of characters/ strings)}]
{
	int count;		// the number of strings in the below array
	char** the_list; // the {array of (array of characters/ strings)}	

	// char* the_list[1024]; // tried this at first, didnt work
};

void list_insert( struct List myList, char* str ); // give a string and list, it will insert it

int main()
{

	// struct List res[100]; // declare the arr of arr of strings

	// int i;
	// for( i = 0; i < 100; i++) // for each element in the List struct, allocate space for 50 strings
	// {
	// 	res[i].count = 0;
	// 	res[i].the_list = malloc(50*sizeof(char*));
	// }

	// list_insert(res[0], "hello"); // insert hello into the List res[0]


	char** l[100];
	int i;
	for(i = 0; i < 100; i++)
	{
		l[i] = malloc(50*sizeof(char*));

		int j;
		for(j = 0; j < 100; j++)
		{
			l[i][j] = malloc(50*sizeof(char));
		}
	}

	// l[0] = malloc(50*sizeof(char));
	strcpy(l[0][0], "hello");


	printf("%s\n", l[0][0]);

	return 0;

}

void list_insert( struct List myList, char* str )
{
	myList.the_list = malloc(50*sizeof(char));	// clear space for a 50 char string as an element
	strcpy ( myList.the_list[ myList.count ], str) ; // add
	myList.count += 1;
}
