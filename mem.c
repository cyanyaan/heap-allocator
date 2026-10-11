#include "mem.h"

static unsigned init_cnt = 0;
f_list* head = NULL;

static f_list* get_free_chunk(int size, f_list** prev_node);
static int _round(int sizeOfRegion);
static inline f_list* list_comp(f_list *ptr, f_list **previous_node);

int Mem_Init(int sizeOfRegion){
	if ((sizeOfRegion <= 0) || (init_cnt != 0)) {
		printf("err\n");
		return -1;
	}
	init_cnt++;
	sizeOfRegion = _round(sizeOfRegion);
	head = mmap(NULL, sizeOfRegion, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	if (head == MAP_FAILED) {
		head = NULL;
		return -1;
	}
	head->next = NULL;
	head->size = sizeOfRegion;

	return 0;
}

void *Mem_Alloc(int size){

    if (size <= 0)
    {
        return NULL;
    }
    size += sizeof(f_list);

    f_list* previous_node;
    f_list* free_chunk = get_free_chunk(size, &previous_node);
    if (free_chunk == NULL)
    {
        return NULL;
    }
    // if(free_chunk->next == NULL) mainchunk = true;  //set true if chosen chunk is the last chunk, only place size -= sizeof(f_list)
    int freeChunk_size = free_chunk->size;  //size of the free chunk selected
    f_list *next_chunk = free_chunk->next;

    bool split = ((freeChunk_size - size) >= sizeof(f_list));
    
    header_t *header = (header_t*) free_chunk;
    
    header->magic = MAGIC;

    if (!split) // if the remaining space cant hold an additional header
    {
        header->size = freeChunk_size;
        if (previous_node == NULL)
        { 
            head = next_chunk;
        } else{
            previous_node->next = next_chunk;
        }
    } else {
        header->size = size;

        f_list *new_head =  (f_list*)((char*)free_chunk + size);
        new_head->size = freeChunk_size-size;
        new_head->next = next_chunk;

        if (previous_node == NULL)
        {
            head = new_head;
        } else {
            previous_node->next = new_head;
        }
    }
    return (void*)(header+1);
}

void coalesce(f_list* freed, f_list *prev){
    f_list* tmp = freed;

    if ((((char*)tmp + tmp->size) == (char*)tmp->next)){
        int total_size = (tmp->next == NULL) ? tmp->size : tmp->size + tmp->next->size; 

        tmp->size = total_size;
        tmp->next = tmp->next->next; 

        if (prev == NULL) //tmp is head
        {
            // printf("adjacent new size: %d, new pos: %p\n", total_size, (char*)tmp+total_size);
            head = tmp;
        } else{
            // printf("adjacent new size: %d, new pos: %p\n", total_size, (char*)tmp+total_size);
            prev->next = tmp;
        }

        // printf("tmp: %p, prev: %p, next: %p\n", freed, prev, tmp->next);
    }

    if (prev != NULL && (((char*)prev+prev->size) == (char*)tmp))
    {

        // printf("adjacent prev: %p and tmp: %p\n", prev, tmp);
        prev->next = tmp->next;
        prev->size = tmp->size + prev->size;
    }
    

}

int Mem_Free(void *ptr){
    if (ptr == NULL)
    {
        return -1;
    }
    header_t *hptr = (header_t*) ptr - 1;
    if (hptr->magic != MAGIC)
    {
        return -1;
    }
    hptr->magic = 0;

    f_list* freed_chunk = (f_list*) hptr; 
    // printf("freed choooooooooooooooonk: %p\n", freed_chunk);
    freed_chunk->size = hptr->size;

    f_list *previousNode = NULL;
    f_list *insertAt = list_comp(freed_chunk, &previousNode);
    // printf("insert at: %p, previous node: %p, freed chunk: %p\n", insertAt, previousNode, freed_chunk);

    freed_chunk->next = insertAt;
    if (previousNode == NULL) //insert at head
    {
        head = freed_chunk;
    } else{  //has a non empty previous node
        previousNode->next = freed_chunk;
    }
    coalesce(freed_chunk, previousNode);
        
    return 0;
}

static int _round(int sizeOfRegion){
	int page_size = getpagesize();

	return ((sizeOfRegion+(page_size-1))/page_size) * page_size;
}

int Mem_Available(){
	int free_size = 0;
	f_list *tmp = head;
	while (tmp != NULL) {
		free_size = free_size + tmp->size;
		tmp = tmp->next;
	}
	printf("available memory: %d\n", free_size);
	return free_size;
}

void Mem_Dump(){
	f_list* tmp = head;
	printf("\n");
	while (tmp != NULL) {
		printf("block: %p\n", tmp);
		printf("	size: %d\n", tmp->size);
		printf("	next: %p\n", tmp->next);
		printf("\n");

		tmp = tmp->next;
	}
}

static f_list* get_free_chunk(int size, f_list** prev_node){
	f_list *hptr = NULL;
	f_list *prev = NULL;
	f_list *tmp = head;

	while (tmp != NULL) {
		if (tmp->size >= size) {
			hptr = tmp;
			*prev_node = prev;
			return hptr;
		}
		prev = tmp;
		tmp = tmp->next;
	}
	return hptr;
}

static inline f_list* list_comp(f_list *ptr, f_list **previous_node){
    f_list *tmp = head;
    f_list *prev = NULL;
    while (tmp != NULL)
    {
        if (tmp > ptr)
        {
            *previous_node = prev;
            return tmp;
        }
        prev = tmp;
        tmp = tmp->next;
    }
    
    return NULL;
}