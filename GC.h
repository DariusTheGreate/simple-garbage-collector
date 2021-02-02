#ifndef _GC_
#define _GC_

#include <stdio.h>
#include <stdlib.h>

#define STACK_MAX 256
#define INIT_OBJ_NUM_MAX 8

typedef enum{
	OBJ_INT,
	OBJ_STR,
	HARD_TYPE
} StandartType;


typedef struct sObject{
	StandartType type;
	unsigned char marked;

	struct sObject* next;
	union{
		int* int_value;
		char* str_value;
		void* undefined_struct;
  	} value;
} Object;

typedef struct{
	Object* stack[STACK_MAX];
	int size;

	Object* first_object;

	int objects_counter;

	int max_objects;
} VM;

Object* new_obj_standart_type(VM* vm, StandartType input_type, void* value_container);
VM* new_vm(int start_size);
void push_vm(VM* vm, Object* input_value);
void push_entity(VM* vm, StandartType input_type, void* value);
Object* pop_vm(VM* vm);
void mark(Object* obj);
void mark_all(VM* vm);
void vm_walkthrough(VM* vm);
void GC(VM* vm);
void free_vm(VM* vm);

#endif
