#include "mem.h"



int main(){
	Mem_Init(4000);
	Mem_Available();
	Mem_Alloc(10);

	Mem_Available();
}
