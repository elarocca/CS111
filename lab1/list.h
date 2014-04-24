#include <stdio.h>
#include <stdlib.h>



typedef struct ListNodeTag{
	void* data;
	struct ListNodeTag *next;
}List;

List* initList(void* data);
void pushBack(void*data, List **head);
void* popBack(List **head);
void* peakBack(List **head);


/*
void pushFront(void*data,List **head);
void* popFront(List **head);
void* peakFront(List **head);
*/
