#include "linkedList.h"
#include "general.h"
#include <iostream>

List* new_list(void* val){
	Node* temp = (Node*)s_malloc(sizeof(Node));
	temp -> next = NULL;
	temp -> value = val;
	temp -> tag = -1;
	List* res_list = (List*)s_malloc(sizeof(List));
	res_list -> start = temp;
	return res_list;
}

void push_list(Node* lst, void* val, int type){
	if(lst -> next == NULL)
	{
		lst -> next = (Node*)s_malloc(sizeof(Node));
		lst -> next -> value = val;
		lst -> next -> next = NULL;
		lst -> next -> tag = type;
	}
	else
		push_list(lst -> next, val, type);
}

void print_list(Node* lst){
	if(lst == NULL)
		return;
	if(lst -> value != NULL)
		std::cout << *(int*)(lst -> value);	//printf("%d\n", *(int*)(lst -> value));
	//printf("%d\n", lst -> value);
	print_list(lst -> next);
}