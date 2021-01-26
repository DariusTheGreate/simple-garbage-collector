#include "GC.h"

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
    #ifdef _DEBUG_
		printf("malloc\n");
	#endif
    void *p = malloc(n);
    if (p == NULL) {
        fprintf(stderr, "Fatal: failed to allocate %zu bytes.\n", n);
		exit(2);
    }
    return p;
}

void s_free(void* ptr){
	#ifdef _DEBUG_
		printf("free\n");
	#endif
    if (ptr == NULL) {
        fprintf(stderr, "trying to free NULL.\n");
		exit(2);
    }
    free(ptr);
}


//void pointer input looks dangerous...
Object* new_obj_standart_type(VM* vm, StandartType input_type, void* value_container){
	Object* obj = (Object*)s_malloc(sizeof(StandartType));
	obj -> type = input_type;
	if(input_type == OBJ_STR)
		(*obj).value.str_value = (char*) value_container;
	else if(input_type == OBJ_INT)
		(*obj).value.int_value = ((int*) value_container);
	else if(input_type == HARD_TYPE)
		(*obj).value.undefined_struct = value_container;
	
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

void push_entity(VM* vm, StandartType input_type, void* value){
	Object* obj = new_obj_standart_type(vm, input_type, value);
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
			s_free(temp);
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
