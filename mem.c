#include "mem.h"
#define H_MAGIC 1234567
#include <stdio.h>


//globals
static unsigned init_cnt = 0;
node_t* head = NULL;

static int _round(int sizeOfRegion){
	int page_size = getpagesize();
	return ((sizeOfRegion+(page_size-1))/page_size) * page_size;
}

int Mem_Init(int sizeOfRegion){
	if ((sizeOfRegion <= 0) || (init_cnt != 0)) {
		printf("err\n");
		return -1;
	}
	init_cnt++;

	sizeOfRegion = _round(sizeOfRegion);
	head = mmap(NULL, sizeOfRegion, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	head->size = sizeOfRegion - sizeof(node_t);
	head->next = NULL;
	return 0;
}

// printf("im here\n");

void* Mem_Alloc(int size){
	void* block;
	int total_size = size + sizeof(header_t);
	int free_size = 0;

	node_t* tmp = head;
	header_t* hptr;
	//find a free chunk from the free list
	while (tmp != NULL) {
		if (tmp->size >= total_size) {
			hptr = (header_t*)tmp;
		}
		tmp = tmp->next;
	}
	hptr->magic = H_MAGIC;
	hptr->size = size;

	block = hptr;

	head = block + total_size;
	head->size -= total_size;

	block = (header_t*)block + 1;

	return block;
}



void Mem_Free(void* ptr){
	header_t *hptr = (header_t*) ptr - 1;
	assert(hptr->magic == H_MAGIC);
}
int Mem_Available(){
	int free_size = 0;
	node_t* tmp = head;
	while (tmp != NULL) {
		free_size += tmp->size;
		tmp = tmp->next;
	}
	printf("free size: %d\n", free_size);
	return free_size;
}

void Mem_Dump(){
	node_t* tmp = head;
	while (tmp != NULL) {
		printf("block: %p\n", tmp);
		printf("	size: %d\n", tmp->size);
		printf("	next: %p\n", tmp->next);
		// printf("	is_free?: %s\n", tmp->is_free ? "true" : "false");
		tmp = tmp->next;
	}
}
