#include "mem.h"

static unsigned init_cnt = 0;
f_list* head = NULL;

static f_list* get_free_chunk(int size, f_list** prev_node);
static int _round(int sizeOfRegion);
void mergeSort();
// static f_list* coalesce();

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

        f_list *new_head =  (f_list*)(char*)free_chunk + size;
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
    freed_chunk->size = hptr->size;
    freed_chunk->next = head;
    head = freed_chunk;

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

// static f_list* split(f_list* head){
//     f_list *fast = head;
//     f_list *slow = head;

//     while (fast != NULL && fast->next != NULL)
//     {
//         fast = fast->next->next;
//         if (fast != NULL)
//         {
//             slow = slow->next;
//         }
//     }
//     f_list *temp = slow->next;
//     slow->next = NULL;

//     return temp;
// }

// static f_list* merge(f_list *first, f_list *second){
//     if(first == NULL) return second;
//     if(second == NULL) return first;

//     // if (first->)
//     // {
//     //     /* code */
//     // }
    

// }

// void mergeSort(){
//     f_list **headptr = &head;

//     while (*headptr != NULL)
//     {
//         printf("address of pointer: %p\n", *headptr); 
//         (*headptr) = (*headptr)->next;
//     }
// } 