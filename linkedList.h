#ifndef _LINKEDLIST_
#define _LINKEDLIST_

typedef struct node{
	struct node* next;	
	void* value;
	int tag;
} Node;

typedef struct list{
	Node* start;
} List;

List* new_list(void* val);
void push_list(Node* lst, void* val, int type);
void print_list(Node* lst);

#endif
