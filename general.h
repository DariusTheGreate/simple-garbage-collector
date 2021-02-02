#ifndef _GENERAL_
#define _GENERAL_

#include <stdio.h>
#include <stdlib.h>

typedef enum{
	stack_overflow,
	stack_underflow,
	not_error
} error_code;

void assert_errors(int cond, error_code err);
void* s_malloc(size_t n);
void s_free(void* ptr);

#endif
