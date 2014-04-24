#include "stack.h"


Stack* initStack(void* data){
 	Stack* init = malloc(sizeof(Stack));
	init->data = data;
	init->next = NULL;
	return init;
}

void* popStack(Stack **head){

	if(head == NULL || (*head) == NULL)
	{
		return NULL;
	}
	void* tmpData;
	if((*head)->next == NULL)
	{
		tmpData = (*head)->data;
		//free(*head);
		*head = NULL;
		return tmpData; 
	}
	else
	{
		Stack* tmp = (*head);
		while(tmp->next->next != NULL)
		{
			tmp = tmp->next;
		}
		tmpData = (*head)->next->data;
		free((*head)->next);
		(*head)->next = NULL;
		return tmpData;
	}

}
void pushStack(void* data, Stack **head){

	Stack* node = initStack(data);
	if(head == NULL || (*head) == NULL)
	{
		(*head) = node;
		return;
	}
	else
	{
		Stack* tmp = (*head);
		while(tmp->next != NULL)
		{
			tmp = tmp->next;
		}
		tmp->next = node;

		return;
	}
}

void* topStack(List **head){

	if(head == NULL || (*head) == NULL)
	{
		return NULL;
	}
	Stack* tmp = (*head);
	while(tmp->next != NULL)
	{
		tmp = tmp->next;
	}
	return (*head)->data;
}