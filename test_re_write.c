#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main()
{	

	FILE * create_it;
	create_it = fopen("sample.txt", "w");
	close(create_it);

	int out_file = open( "sample.txt", O_WRONLY);
	int saved_stdout; 

	//save current stdout for later
	saved_stdout = dup(1);
	dup2( out_file, 1 );

	printf( "hello world\n" );


	//restore the stdout
	// close(out_file);
	dup2(saved_stdout, 1);
	close(saved_stdout);
	



	printf( "hello world\n" );

	return 0;
}