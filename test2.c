#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "mem.h"

int main(int argc, char *argv[])
{
	char *a1;
	char *a2;
	char *a3;
	char *a4;

	/*
	 * must be first call in the program
	 */
	Mem_Init(4000);
	printf("initial memory: %d\n", Mem_Available());

	a1 = (char *)Mem_Alloc(128);
	if(a1 == NULL)
	{
		fprintf(stderr,"call to MyMalloc(128) failed\n");
		fflush(stderr);
		exit(1);
	}

	printf("FREE LIST after malloc(128)\n");
	Mem_Available();
	Mem_Dump();

	a2 = (char *)Mem_Alloc(32);
	if(a2 == NULL)
	{
		fprintf(stderr,"first call to MyMalloc(32) failed\n");
		fflush(stderr);
		exit(1);
	}

	printf("FREE LIST after malloc(32)\n");
	Mem_Available();

	Mem_Dump();

	Mem_Free(a1);

	printf("FREE LIST after free of first 128 malloc()\n");
	Mem_Available();
	Mem_Dump();

	a3 = (char *)Mem_Alloc(104);
	if(a3 == NULL)
	{
		fprintf(stderr,"call to MyMalloc(104) failed\n");
		fflush(stderr);
		exit(1);
	}

	printf("FREE LIST after malloc(104)\n");
	// printf("current head: %p\n", head);
	Mem_Available();
	Mem_Dump();

	a4 = (char *)Mem_Alloc(8);
	printf("block being used at a4: %p\n", a4);
	if(a4 == NULL)
	{
		fprintf(stderr,"call to MyMalloc(8) failed\n");
		fflush(stderr);
		exit(1);
	}
	printf("FREE LIST after malloc(8)\n");
	Mem_Available();
	Mem_Dump();

	/*
	 * free it all -- notice that a1 is already free
	 */
	Mem_Free(a2);
	Mem_Free(a3);
	Mem_Free(a4);
	printf("FREE LIST after all free\n");
	Mem_Available();
	mergeSort();
	Mem_Dump();

	return(0);
}
