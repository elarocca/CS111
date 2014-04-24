#include "queue.h"
#include <errno.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



Queue* initQueue(void* data){
 	Queue* init = malloc(sizeof(Queue));
	init->data = data;
	init->next = NULL;
	return init;
 }
 
void* dequeue(Queue **head)
{
	if(*head == NULL || head == NULL)
		return NULL;
	void** tmpData = (*head)->data;
	Queue* tmpHead = (*head);
	(*head) = (*head)->next;
	free(tmpHead);
	return tmpData;
}

void enqueue(void* data, Queue **head)
{
	Queue* node = initQueue(data);
	if(head == NULL || (*head) == NULL)
	{
		(*head) = node;
		return;
	}
	else
	{
		Queue* tmp = (*head);
		while(tmp->next != NULL)
		{
			tmp = tmp->next;
		}
		tmp->next = node;

		return;
	}
}


void* peak(Queue **head)
{
	if(*head == NULL || head == NULL)
		return NULL;
	return (*head)->data;
}




