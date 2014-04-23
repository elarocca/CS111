#include "parallel.h"

#include <error.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

DependencyGraph * createGraph(command_stream_t c){
	int exitStatus;
	Queue cmd_queue;
	//Create readLists and writeLists for each command tree
	int i;
	while(c->cmds[i] != NULL)
	{
		Queue tmp_readList;
		Queue tmp_writeList;
		listNode tmp_listNode;
		processCommand(c->cmds[i], &tmp_readList, &tmp_writeList);
		i++;
	}
	//Insert listNode for for each command tree into the head of a Queue
	return exitStatus;
}

void processCommand(command_t cmd, Queue *readList, Queue *writeList)
{
	if(cmd->type == SIMPLE_COMMAND)
	{
		//store input (options, etc.) into readList
		int i = 1;
		while(cmd->u.word[i] != NULL && cmd->u.word[i] != '>')
		{
			enqueue(cmd->u.word[i],readList);
			i++;
		}
		//store inputRedirect into readList
		if(cmd->input != NULL)
			enqueue(cmd->input,readList);
		//store outputRedirect into writeList
		if(cmd->output != NULL)
			enqueue(cmd->output,writeList);
	}
	else if(cmd->type == SUBSHELL_COMMAND)
	{
		//store inputRedirect into readList
		if(cmd->input != NULL)
			enqueue(cmd->input,readList);
		//store outputRedirect into writeList	
		if(cmd->output != NULL)
			enqueue(cmd->output,writeList);
		processCommand(cmd->u.subshell_command, readList, writeList);
	}
	else 
	{
		processCommand(cmd->u.command[0] , readList, writeList);
		processCommand(cmd->u.command[1] , readList, writeList);
	}
}
