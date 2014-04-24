#include <stdio.h>
#include <stdlib.h>


typedef struct stackNode
{
	void *data;
	struct stackNode *next;
	
}Stack;

Stack* initStack(void* data);
//pops the top element and return. Return NULL if empty
void* popStack(Stack **head);

void pushStack(void* data, Stack **head);

//only returns the top element. Return NULL if empty
void* topStack(List **head);