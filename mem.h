#ifndef MEM_H
#define MEM_H
#include <sys/mman.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <assert.h>

typedef struct __node_t{
	struct __node_t *next;
	int size;
} node_t;

typedef struct{
	int size;
	int magic;
} header_t;

// union header {
// 	struct{
//
// 	}
// }

extern node_t* head;

int Mem_Init(int sizeOfRegion);
void* Mem_Alloc(int size);
void Mem_Free(void* ptr);
int Mem_Available();
void Mem_Dump();

#endif
