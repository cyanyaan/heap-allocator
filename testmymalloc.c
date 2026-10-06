
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "mem.h"

int main(int argc, char *argv[])
{
	int size;
	void *ptr[10];
	int i;

	Mem_Init(1000);
	/*
	 * try mallocing four pieces, each 1/4 of total size
	 */
	size = MAX_MALLOC_SIZE / 4;

	ptr[0] = Mem_Alloc(size);
	if(ptr[0] == NULL)
	{
		printf("malloc of ptr[0] failed for size %d\n",
				size);
		exit(1);
	}

	Mem_Available();
	printf("\n");

	ptr[1] = Mem_Alloc(size);
	if(ptr[1] == NULL)
	{
		printf("malloc of ptr[1] failed for size %d\n",
				size);
		exit(1);
	}

	Mem_Available();
	printf("\n");

	ptr[2] = Mem_Alloc(size);
	if(ptr[2] == NULL)
	{
		printf("malloc of ptr[2] failed for size %d\n",
				size);
		exit(1);
	}

	Mem_Available();
	printf("\n");

	/*
	 * this one should fail due to rounding
	 */
	ptr[3] = Mem_Alloc(size);
	if(ptr[3] == NULL)
	{
		printf("malloc of ptr[3] fails correctly for size %d\n",
				size);
	}

	Mem_Available();
	printf("\n");

	/*
	 * free the first block
	 */
	Mem_Free(ptr[0]);

	Mem_Available();
	printf("\n");

	/*
	 * free the third block
	 */
	Mem_Free(ptr[2]);

	Mem_Available();
	printf("\n");

	/*
	 * now free second block
	 */
	Mem_Free(ptr[1]);

	Mem_Available();
	printf("\n");

	/*
	 * re-malloc first pointer
	 */
	ptr[0] = Mem_Alloc(size);
	if(ptr[0] == NULL)
	{
		printf("re-malloc of ptr[0] failed for size %d\n",
				size);
		exit(1);
	}
	Mem_Available();
	printf("\n");

	/*
	 * try splitting the second block
	 */
	ptr[1] = Mem_Alloc(size/2);
	if(ptr[1] == NULL)
	{
		printf("split second block ptr[1] failed for size %d\n",
				size/2);
		exit(1);
	}
	Mem_Available();
	printf("\n");

	/*
	 * free first block and split of second
	 */
	Mem_Free(ptr[0]);
	Mem_Free(ptr[1]);

	Mem_Available();
	printf("\n");

	/*
	 * try mallocing a little less to make sure no split occurs
	 * first block from previous print should not be split
	 */
	ptr[0] = Mem_Alloc(size-1);
	if(ptr[0] == NULL)
	{
		printf("slightly smaller malloc of ptr[0] failed for size %d\n",
				size);
		exit(1);
	}

	/*
	 * free it and make sure it comes back as the correct size
	 */
	Mem_Free(ptr[0]);

	Mem_Available();
	printf("\n");

	/*
	 * okay, now see if multiples work
	 */
	for(i=0; i < 6; i++)
	{
		ptr[i] = Mem_Alloc(100);
	}

	/*
	 * free first block, third block, fifth block
	 */
	Mem_Free(ptr[0]);
	Mem_Free(ptr[2]);
	Mem_Free(ptr[4]);
	Mem_Available();
	printf("\n");

	/*
	 * now, free second block -- first, second, third blocks
	 * should coalesce
	 */
	Mem_Free(ptr[1]);
	Mem_Available();
	printf("\n");

	/*
	 * free the sixth block and it should merge with the last
	 * block leaving two
	 */
	Mem_Free(ptr[5]);
	Mem_Available();
	printf("\n");

	/*
	 * now free fourth block and they should all be together
	 */
	Mem_Free(ptr[3]);
	Mem_Available();
	printf("\n");

	printf("made it -- passed test\n");

	exit(0);
}
