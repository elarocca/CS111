// UCLA CS 111 Lab 1 command execution

#include "command.h"
#include "command-internals.h"

#include <error.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

/* FIXME: You may need to add #include directives, macro definitions,
   static function definitions, etc.  */
void executingSimple(command_t c);
void executingAnd(command_t c);
void executingOr(command_t c);
void executingPipe(command_t c);
void executingSequence(command_t c);
void executingSubshell(command_t c);

void execute_switch(command_t c)
{
	switch(c->type)
	{
	case SIMPLE_COMMAND:
		executingSimple(c);
		break;
	case SUBSHELL_COMMAND:
		executingSubshell(c);
		break;
	case AND_COMMAND:
		executingAnd(c);
		break;
	case OR_COMMAND:
		executingOr(c);
		break;
	case SEQUENCE_COMMAND:
		executingSequence(c);
		break;
	case PIPE_COMMAND:
		executingPipe(c);
		break;
	default:
		error(1, 0, "Not a valid command");
	}
}

void executingSimple(command_t c)
{
	pid_t pid;
	int status; 
	if ((pid = fork()) < 0)
    {
		error(1, errno, "fork was unsuccessful");
    }
    else if (pid == 0)  // run command in child process
    {
    	if(c->input != NULL)
		{
			int input = open(c->input,O_RDWR);
			if ( dup2(input,0) < 0 )
			{
				error(1, errno, "error with dup2");
			}
			execvp(c->u.word[0], c->u.word);
			close(input);
		}
		if(c->output != NULL)
		{
			int output = open(c->output,O_WRONLY);
			if ( dup2(output,1) < 0 )
			{
				error(1, errno, "error with dup2");
			}
			execvp(c->u.word[0], c->u.word);
			close(output);

		}
		if(c->input == NULL && c->output == NULL)
		{
			execvp(c->u.word[0], c->u.word);
		}
    }
    else // parent
    {
    	waitpid(pid,&status,0);
    	c->status = WEXITSTATUS(status);
    }
}
void executingSubshell(command_t c)
{
	if(c->input != NULL)
	{
		int input = open(c->input,O_RDWR);
		dup2(input,0);
		execute_switch(c->u.subshell_command);
		close(input);
	}
	if(c->output != NULL)
	{
		int output = open(c->output,O_WRONLY);
		dup2(output,1);
		execute_switch(c->u.subshell_command);
		close(output);
	}
	if(c->input == NULL && c->output == NULL)
	{
		execute_switch(c->u.subshell_command);
	}
}

void executingAnd(command_t c)
{
	pid_t pid = fork();
	if(pid == 0)
	{
		execute_switch(c->u.command[0]);
	}
	if(pid > 0)
	{
		int status;
		waitpid(pid,&status,0);
		int exitStatus = WEXITSTATUS(status); 
		if(exitStatus < 0)
		{
			c->status = -1;
			return;
		}
		else
			execute_switch(c->u.command[1]);
	}
}
void executingOr(command_t c)
{
	pid_t pid = fork();
	if(pid == 0)
	{
		execute_switch(c->u.command[0]);
	}
	if(pid > 0)
	{
		int status;
		waitpid(pid,&status,0);
		int exitStatus = WEXITSTATUS(status); 
		if(exitStatus >= 0)
		{
			c->status = exitStatus;
			return;
		}
		else
			execute_switch(c->u.command[1]);
	}
}
void executingPipe(command_t c)
{
	pid_t returnedPid;
	pid_t firstPid;
	pid_t secondPid;
	int buffer[2];
	int eStatus;

	if ( pipe(buffer) < 0 )
	{
		error (1, errno, "pipe was not created");
	}

	firstPid = fork();
	if (firstPid < 0)
    {
		error(1, errno, "fork was unsuccessful");
    }
	else if (firstPid == 0) //child executes command on the right of the pipe
	{
		close(buffer[1]); //close unused write end

        //redirect standard input to the read end of the pipe
        //so that input of the command (on the right of the pipe)
        //comes from the pipe
		if ( dup2(buffer[0], 0) < 0 )
		{
			error(1, errno, "error with dup2");
		}
		execute_switch(c->u.command[1]);
		_exit(c->u.command[1]->status);
	}
	else 
	{
		// Parent process
		secondPid = fork(); //fork another child process
                            //have that child process executes command on the left of the pipe
		if (secondPid < 0)
		{
			error(1, 0, "fork was unsuccessful");
		}
        else if (secondPid == 0)
		{
			close(buffer[0]); //close unused read end
			if(dup2(buffer[1], 1) < 0) //redirect standard output to write end of the pipe
            {
				error (1, errno, "error with dup2");
            }
			execute_switch(c->u.command[0]);
			_exit(c->u.command[0]->status);
		}
		else
		{
			// Finishing processes
			returnedPid = waitpid(-1, &eStatus, 0); //this is equivalent to wait(&eStatus);
                        //we now have 2 children. This waitpid will suspend the execution of
                        //the calling process until one of its children terminates
                        //(the other may not terminate yet)

			//Close pipe
			close(buffer[0]);
			close(buffer[1]);

			if (secondPid == returnedPid )
			{
			    //wait for the remaining child process to terminate
				waitpid(firstPid, &eStatus, 0); 
				c->status = WEXITSTATUS(eStatus);
				return;
			}
			
			if (firstPid == returnedPid)
			{
			    //wait for the remaining child process to terminate
   				waitpid(secondPid, &eStatus, 0);
				c->status = WEXITSTATUS(eStatus);
				return;
			}
		}
	}	
}
void executingSequence(command_t c)
{
	pid_t pid = fork();
	if(pid == 0)
	{
		execute_switch(c->u.command[0]);
	}
	if(pid > 0)
	{
		int status;
		waitpid(pid,&status,0);
		int exitStatus = WEXITSTATUS(status);
		execute_switch(c->u.command[1]);
		c->status = exitStatus;
	}
}
int
command_status (command_t c)
{
  return c->status;
}

void
execute_command (command_t c, bool time_travel)
{
  /* FIXME: Replace this with your implementation.  You may need to
     add auxiliary functions and otherwise modify the source code.
     You can also use external functions defined in the GNU C Library.  */
 
  // error (1, 0, "command execution not yet implemented");
  if (time_travel == false)
	{
	    execute_switch(c);
	}
}
