#include "queue.c"
#include "command.h"
#include "command-internals.h"
//#include "read-command.c"


typedef struct {
  command_t command;
  struct GraphNode ** before; 
  pid_t pid;
} GraphNode;

typedef struct {
  Queue * no_dependencies;
  Queue * dependencies;
} DependencyGraph;  

typedef struct {
	GraphNode* node;	
	Queue * readQueue;
	Queue * writeQueue;
} listNode;    

void processCommand(command_t cmd, Queue *readList, Queue *writeList);

DependencyGraph* createGraph(command_stream_t c);

