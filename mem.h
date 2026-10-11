#ifndef MEM_H
#define MEM_H
#define MAGIC 1234567
// #define MAX_MALLOC_SIZE (1024*1024*16)
#define MAX_MALLOC_SIZE 4096
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct {
	int size;
	int magic;
	void *padding;
}header_t;

typedef struct free_list{
	int size;
	struct free_list *next;
} f_list;

extern f_list* head;

int Mem_Init(int sizeOfRegion);
void* Mem_Alloc(int size);
int Mem_Free(void *ptr);
int Mem_Available();
void Mem_Dump();

#endif
