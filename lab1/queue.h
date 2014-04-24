#include <stdio.h>
#include <stdlib.h>
#include "command.h"

typedef struct queueNodeTag {
  void* data;
  struct queueNodeTag *next;
} Queue;

Queue* initQueue(void* data);
void* dequeue(Queue **head);
void enqueue(void* data, Queue **head);
void* peak(Queue **head);




