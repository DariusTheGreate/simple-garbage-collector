#include "GC.h"

typedef struct some_type
{
	int a;
	int b;
	struct some_type* next;
	struct some_type* prev;
	void* entity;
} some_type;

int main(){
	VM* virual_machine = new_vm(3);
	printf("size is %d\n", virual_machine -> size);
	const char* const message = "putin molodec politik lider i boec";
	int* some_int = (int*)s_malloc(sizeof(int));
	*some_int = 12;
	push_entity(virual_machine, OBJ_STR, (void*)message);
	push_entity(virual_machine, OBJ_STR, (void*)message);
	push_entity(virual_machine, OBJ_INT, (void*)some_int);

	some_type* node = (some_type*)s_malloc(sizeof(some_type));
	push_entity(virual_machine, HARD_TYPE, (void*)node);
	
	printf("%s\n", pop_vm(virual_machine) -> value);
	free_vm(virual_machine);
	printf("----------------\n");
}