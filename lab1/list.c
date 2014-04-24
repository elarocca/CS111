#include "list.h"
#include <errno.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

List* initList(void* data)
{
	List* init = malloc(sizeof(List));
	init->data = data;
	init->next = NULL;
	return init;
}
void pushBack(void*data, List **head)
{
	List* node = initList(data);
	if(head == NULL || (*head) == NULL)
	{
		(*head) = node;
		return;
	}
	else
	{
		List* tmp = (*head);
		while(tmp->next != NULL)
		{
			tmp = tmp->next;
		}
		tmp->next = node;
		return;
	}
}
void* popBack(List **head)
{
	//if list does not exist
	if(head == NULL || *head ==NULL)
		return NULL;
	//if list contains only one item
	if((*head)->next == NULL)
	{
		void* tmpData = (*head)->data;
		free(*head);
		return tmpData;
	}

	if((*head)->next->next == NULL)
	{
		void* tmpData = (*head)->next->data;
		free((*head)->next);
		(*head)->next == NULL;
		return tmpData;
	}
	else
		popBack(&((*head)->next));
}
void* peakBack(List **head)
{
	
	//if list does not exist
	if(*head == NULL)
		return NULL;

	if((*head)->next == NULL)
	{
		return (*head)->data;
	}
	else
		popBack(&((*head)->next));
}

/*
void pushFront(void*data,List **head);
void* popFront(List **head);
void* peakFront(List **head);
*/