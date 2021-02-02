#include "general.h"
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
