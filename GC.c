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
Object* new_obj_standart_type(VM* vm, StandartType input_type){
	Object* obj = (Object*)s_malloc(sizeof(StandartType));
	
	obj -> type = input_type;
	/*if(input_type == OBJ_STR)
		(*obj).value.str_value = (char*) value_container;
	else if(input_type == OBJ_INT)
		(*obj).value.int_value = *(int*) value_container;
	*/
	obj -> marked = 0;
	obj -> next = vm -> first_object;
	vm -> first_object = obj;
	vm -> objects_counter++;
	
	return obj;
}

VM* new_vm(int start_size){
	VM* vm = (VM*)s_malloc(sizeof(VM));
	vm->size = 0;
	vm->first_object = NULL;
	vm->objects_counter = 0;
	vm->max_objects = INIT_OBJ_NUM_MAX;
	return vm;
}

void push_vm(VM* vm, Object* input_value){
	assert_errors(vm -> size < STACK_MAX, stack_overflow);
	vm -> stack[vm -> size++] = input_value;
}

void pushChar(VM* vm, char* str){
	Object* obj = new_obj_standart_type(vm, OBJ_STR);
	obj -> value.str_value = str;
	push_vm(vm, obj);
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
	for(int i = 0; i < vm -> size; i++){
		mark(vm -> stack[i]);
	}
}

void vm_walkthrough(VM* vm){
	Object* obj = vm -> first_object;
	printf("just start\n");
	while(obj != NULL){
		printf("%s\n", obj -> value);
		if(obj -> marked == 0){
			Object* temp = obj;
			//if(obj -> next == NULL)
			//	return;
			obj = obj -> next;
			free(temp);
			vm -> objects_counter--;
		}	
		else{
			obj -> marked = 0;
			if(obj -> next == NULL)
				return;
			obj = obj -> next;
		}
	}
}

void GC(VM* vm){
	int count = vm -> objects_counter;
	mark_all(vm);
	vm_walkthrough(vm);
	vm -> max_objects = vm->objects_counter == 0 ? INIT_OBJ_NUM_MAX : vm -> objects_counter * 2;

  	printf("Collected %d objects, %d remaining.\n", count - vm->objects_counter, vm->objects_counter);
}

void free_vm(VM* vm){
	vm -> size = 0;
	GC(vm);
	free(vm);
}

int main(){
	VM* virual_machine = new_vm(3);
	printf("size is %d\n", virual_machine -> size);
	const char* const message = "putin molodec politik lider i boec";
	pushChar(virual_machine, (char*)message);
	pushChar(virual_machine, (char*)message);
	pushChar(virual_machine, (char*)message);
	
	printf("%s\n", pop_vm(virual_machine) -> value);
	//mark_all(virual_machine);
	//vm_walkthrough(virual_machine);
	free_vm(virual_machine);
	printf("----------------\n");
}
