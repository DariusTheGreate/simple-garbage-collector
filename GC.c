#include <stdio.h>
#include <stdlib.h>

#define STACK_MAX 256
#define INIT_OBJ_NUM_MAX 8

typedef enum{
	stack_overflow,
	stack_underflow,
	not_error
} error_code;

//strange - baaad
void assert_errors(int cond, error_code err){
	if(cond == 0){
		if(err == stack_overflow)
			printf("stack overflow\n");
		if(err == stack_underflow)
			printf("stack underflow\n");
		exit(1);
	}
}

void* s_malloc(size_t n){
    void *p = malloc(n);
    if (p == NULL) {
        fprintf(stderr, "Fatal: failed to allocate %zu bytes.\n", n);
		exit(2);
    }
    return p;
}

typedef enum{
	OBJ_INT,
	OBJ_STR
} StandartType;

typedef struct sObject{
	StandartType type;
	unsigned char marked;

	struct sObject* next;
  // copypast - baaad
	union{
		int int_value;
		char* str_value;
  	} value;
} Object;

typedef struct{
	Object* stack[STACK_MAX];
	int size;

	Object* first_object;

	int objects_counter;

	int max_objects;
} VM;

//void pointer input looks dangerous...
Object* new_obj_standart_type(VM* vm, StandartType input_type, void* value_container){
	Object* obj = (Object*)s_malloc(sizeof(StandartType));
	obj -> type = input_type;
	if(input_type == OBJ_STR)
		(*obj).value.str_value = (char*) value_container;
	else if(input_type == OBJ_INT)
		(*obj).value.int_value = *(int*) value_container;
	return obj;
}

VM* new_vm(int start_size = 0){
	VM* vm = (VM*)s_malloc(sizeof(VM));
	vm->size = start_size;
	vm->first_object = NULL;
	vm->objects_counter = 0;
	vm->max_objects = INIT_OBJ_NUM_MAX;
	return vm;
}

void push_vm(VM* vm, Object* input_value){
	assert_errors(vm -> size < STACK_MAX, stack_overflow);
	vm -> stack[vm -> size++] = input_value;
}

Object* pop_vm(VM* vm){
	assert_errors(vm -> size < STACK_MAX, stack_overflow);
	return vm -> stack[--vm -> size];
}

void mark(Object* obj){
	if(obj -> marked)
		return;
	obj -> marked = 1;
}

void mark_all(VM* vm){
	for(int i = 0; i < vm -> size; ++i)
		mark(vm -> stack[i]);
}

int main(){
	VM* virual_machine = new_vm(3);
	printf("size is %d\n", virual_machine -> size);
	const char* const message = "putin molodec politik lider i boec";
	Object* in_val = new_obj_standart_type(virual_machine, OBJ_STR, (void*)message);
	push_vm(virual_machine, in_val);
	printf("%s\n", pop_vm(virual_machine) -> value);
	
}