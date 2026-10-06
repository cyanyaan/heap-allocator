#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "mem.h"

int main(int argc, char *argv[])
{
	char *array;
	int i;

	/*
	 * must be first call in the program
	 */
	Mem_Init(10);

	array = Mem_Alloc(100);
	// array = (char*)head;
	Mem_Available();
	if(array == NULL)
	{
		fprintf(stderr,"call to MyMalloc() failed\n");
		fflush(stderr);
		exit(1);
	}

	for(i=0; i < 9; i++)
	{
		array[i] = 'a' + i;
	}
	array[9] = 0;

	printf("here is my nifty new string: %s\n",array);

	// Mem_Free(array);

	return(0);
}
