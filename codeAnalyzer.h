#ifndef _CODEANALYZER_
#define _CODEANALYZER_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linkedList.h"
#include "general.h"

#define MAX_VARIABLE_NAME_LEN 1

typedef enum command_type{
	CreateTag,
	FreeTag
} commandType;

typedef struct TypeWrapper{
	void* data;
	int data_type;
} type_wrapper;
type_wrapper* new_type_wrapper(int type, void* data);


List* parser(char* filename);
void* parse_creater(char* create_block);
void* parse_free(char* free_block);

#endif
