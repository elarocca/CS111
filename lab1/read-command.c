// UCLA CS 111 Lab 1 command reading

#define NULL 0
#define EOF -1

#define and 1
#define sequence 2
#define or 3
#define pipe 4
#define simple 5
#define subshell 6

#include "command.h"
#include "command-internals.h"
#include <error.h>
#include <stdio.h>

//***************************** our own defined stack ************************************
	typedef struct stack
	{
		command_t cmd;
		struct stack *next;
		int size;
	}node;

	//pops the top element and return. Return NULL if empty
	command_t pop(node **head)
	{
		if(head == NULL)
		{
			printf("NULL pointer");
			return;
		}
		command_t cmd;
		node* temp;
		temp = (*head);
		cmd = (*head)->cmd;
		(*head) = (*head)->next;
		//(*head)->size--;
		free(temp);
		return cmd;
	}

	void push(command_t word, node **head)
	{
		node *new = malloc(sizeof(node));
		new->cmd = word;
		new->next = *head;
		new->size = (*head)->size+1;
		*head = new;
	}

	//only returns the top element. Return NULL if empty
	command_t top(node **head)
	{
		if((*head) == NULL)
			return NULL;
		else
		{
			return (*head)->cmd;
		}
	}

/* FIXME: Define the type 'struct command_stream' here.  This should
   complete the incomplete type declaration in command.h.  */

struct command_stream
{	
	command_t* cmds;
}; 

//******************************** tokenizer helper functions ****************************
	int isNormalCharacter(int c)
	{
		return ( (c >= 'a' && c <= 'z' ) || 
				 (c >= 'A' && c <= 'Z' ) || 
				 (c >= '1' && c <= '9' ) ||
				 c == '!' || c == '%' || c == '+' ||
				 c == ',' || c == '-' || c == '.' ||
				 c == '/' || c == ':' || c == '@' ||
				 c == '^' || c == '_'
				  ); 

	}
	int isWhiteSpace(int c)
	{
		return (c == ' ' || c == '\t');
	}
	int isRedirect(int c)
	{
		return (c == '<' || c == '>');
	}
	 int isSpecialCharacter(int c)
	{
		return (c == '<'  ||  c == '>'  
		 			|| c == '|' || c == '&'	|| c == '(' || c == ')' || c == ';') ; 
	}

	command_t createCommand(char ** tokens, int type)
	{
		command_t tmp = malloc(sizeof(struct command));

		if(type == simple){
			int i = 0;
			char ** word_array = malloc(sizeof(char*));
			tmp->type = SIMPLE_COMMAND;
			
			//getting the word
			while(tokens[i]!= '\0')
			{	
				if(tokens[i][0]=='<' || tokens[i][0]=='>' )
					break;
					word_array[i]= tokens[i];
					i++;
					word_array = realloc(word_array,(i+1)*sizeof(char*));		
			}
			word_array[i] = NULL;
			tmp->u.word = word_array;

			//no input or output
			if(tokens[i] == '\0')
			{
				//free(tokens);
				tmp->input = NULL;//input_array;
				tmp->output = NULL;//output_array;
				return tmp;
			}
			if(tokens[i][0] == '<')
			{
				i++;
				tmp->input = tokens[i];
			}
			else if(tokens[i][0] == '>')
			{
				i++;
				tmp->output = tokens[i];
				return tmp;
			}	
			i++;

			//no output
			if(tokens[i] == '\0')
			{	
				//free(tokens);
				tmp->output = NULL;
				return tmp;
			}

			i++;
			tmp->output = tokens[i];
			//free(tokens);
			return tmp;
		}
		
		switch (type)
		{
		case and:
			tmp->type = AND_COMMAND;
			break;
		case pipe:
			tmp->type = PIPE_COMMAND;
			break;
		case sequence:
			tmp->type = SEQUENCE_COMMAND;
			break;
		case or:
			tmp->type = OR_COMMAND;
			break;
		case subshell:
			tmp->type = SUBSHELL_COMMAND;
			break;
		}
		tmp->status = -1;
		tmp->input = NULL;
		tmp->output == NULL;
		return tmp;
	}
//******************************** make command stream ****************************************************************
command_stream_t
make_command_stream (int (*get_next_byte) (void *),
		     void *get_next_byte_argument)
{
  /* FIXME: Replace this with your implementation.  You may need to
     add auxiliary functions and otherwise modify the source code.
     You can also use external functions defined in the GNU C Library.  */

// Tokenizer
		 char ** token_array = malloc(sizeof(char *));
		 char * token = malloc(sizeof(char));
		 char prev = '\0';
		 char c;
		 int token_array_index = -1;
		 int token_index = 0;
		 
		 int count_newlines = 0;
		 int count_ampersands = 0;
		 int count_pipes = 0;
		 int isComment = 0;	

		 int startFile = 1;
		 int line_num=1;


		 while((c = get_next_byte(get_next_byte_argument)) != EOF)
		 {

		 	if(c != '#' && !isComment && startFile)
			{
				if(prev != '\0')
				{
					startFile = 0;
					token_array_index = 0;
				}
			}
		 	//first character in script
		 	if (c == '#')
		 	{
		 		isComment = 1;
		 		continue;
		 	}
			if(isComment && c == '\n')
		 	{
		 		isComment = 0;
		 		line_num++;
		 	}
		 	else if(isComment)
		 	{
		 		continue;
		 	}
		 	else if(prev == '\0')
		 	{
		 		if (c == '\n')
		 		{
		 			continue;
		 		}
		 		else if (c == '&')
		 		{
		 			count_ampersands++;
		 		}
		 		else if (c == '|')
		 		{
		 			count_pipes++;
		 		}
		 		else if (isWhiteSpace(c))
		 		{
		 			token = malloc(0);
		 			continue;
		 		}
		 		token = malloc(sizeof(char)); 
		 		token[token_index] = c; 
		 	}
		 	else if(c == '\n')
		 	{
		 		count_newlines++;
		 		line_num++;

		 		if(prev == ')')
		 		{
		 			token = realloc(token,(token_index+1)*sizeof(char));
			 		token[token_index] = '\0';
			 		token_array[token_array_index] = token;

			 		token_array_index++;
			 		token_array = realloc(token_array,(token_array_index+1)*sizeof(char*));
			 		
			 		token = malloc(sizeof(char));
			 		token_index = 0;
		 			token[token_index] = ';';
		 		}
		 		else if(isSpecialCharacter(prev))
		 		{
		 			continue;
		 		}
		 		else if(count_newlines == 1)
		 		{
			 		token = realloc(token,(token_index+1)*sizeof(char));
			 		token[token_index] = '\0';
			 		token_array[token_array_index] = token;

			 		token_array_index++;
			 		token_array = realloc(token_array,(token_array_index+1)*sizeof(char*));
			 		
			 		token = malloc(sizeof(char));
			 		token_index = 0;
		 			token[token_index] = ';';
		 		}
		 		else if (count_newlines == 2)
		 		{
		 			
		 			token[token_index] = ';';

		 		}	
		 		else {
		 			prev = c;
		 			continue;
		 		}
		 	}
		 	else if(c == '&')
		 	{
		 		count_ampersands++;
		 		if(count_ampersands == 1)
		 		{
			 		token = realloc(token,(token_index+1)*sizeof(char));
			 		token[token_index] = '\0';
			 		token_array[token_array_index] = token;

			 		token_array_index++;
			 		token_array = realloc(token_array,(token_array_index+1)*sizeof(char*));
			 		
			 		token = malloc(sizeof(char));
			 		token_index = 0;
		 			token[token_index] = '&';
		 		}
		 		else if (count_ampersands >= 2)
		 		{
		 			token[token_index] = '&';
		 		}	
		 		else 
		 		{

		 		}
		 	}
		 	else if(c == '|')
		 	{
		 		count_pipes++;
		 		if(count_pipes == 1)
		 		{
			 		token = realloc(token,(token_index+1)*sizeof(char));
			 		token[token_index] = '\0';
			 		token_array[token_array_index] = token;

			 		token_array_index++;
			 		token_array = realloc(token_array,(token_array_index+1)*sizeof(char*));
			 		
			 		token = malloc(sizeof(char));
			 		token_index = 0;
		 			token[token_index] = '|';
		 		}
		 		else if (count_pipes >= 2)
		 		{
		 			token[token_index] = '|';
		 		}	
		 		else
		 		{

		 		}
		 	}
		 	else if(isSpecialCharacter(c))
		 	{
			 	token = realloc(token,(token_index+1)*sizeof(char));
				token[token_index] = '\0';
		 		token_array[token_array_index] = token;

				token_array_index++;
			 	token_array = realloc(token_array,(token_array_index+1)*sizeof(char*));
			 		
				token = malloc(sizeof(char));
			 	token_index = 0;
		 		token[token_index] = c;
		 	}
		 	else if (isWhiteSpace(c))
		 	{
		 		prev = c;
		 		continue;
		 	}
		 	//if not the same
		 	else if( ( (isNormalCharacter(prev) && !isNormalCharacter(c) ) ) || 
		 			 (!isNormalCharacter(prev) && isNormalCharacter(c) )
		 			 )
		 	{
		 		count_newlines = 0; 
		 		count_ampersands = 0; 
		 		count_pipes = 0;

		 		token = realloc(token,(token_index+1)*sizeof(char));
		 		token[token_index] = '\0';
		 		token_array[token_array_index] = token;

		 		token_array_index++;
		 		token_array = realloc(token_array,(token_array_index+1)*sizeof(char*));
		 		
		 		token = malloc(sizeof(char));
		 		token_index = 0;
		 		token[token_index] = c;
		 	}
		 	//if the same
		 	else 
		 	{
		 		token = realloc(token,(token_index+1)*sizeof(char));
		 		token[token_index] = c;
		 	}
		 	token_index++;
		 	prev = c;
		 }
//--------------------------------------------------------------------------------
			 if(token_array_index == -1)
			{
				return NULL;
			}
		
		 if (!isComment)
		 {
		 	 realloc(token,(token_index+1)*sizeof(char));
			 token[token_index] = '\0';
			 token_array[token_array_index] = token;

		 	if (token[token_index-1] != ';' && token[token_index-1] != '\n')
		 	{
		 		token = malloc(sizeof(char));
			 	token_index = 0;
		 		token[token_index] = ';';
		 		token_index++;

		 		token_array_index++;
		 		realloc(token,(token_index+1)*sizeof(char)); 	
			 	token[token_index] = '\0';
			 	token_array[token_array_index] = token;
		 	}
		 }
		 else 
		 {
		 		token = realloc(token,(token_index+1)*sizeof(char));
		 		token[token_index] = c;
		 }
	
		 int i,j;

			/*
					
		 for( i = 0; i < token_array_index+1; i++)
		  {
		  	j=0;
		  	while(token_array[i][j] != NULL && token_array[i][j] != '\0')
		  	{
		  		printf("%c",token_array[i][j]);
		  		j++;

		  	}
		  	printf("\n");
		  	//printf("%i",i);
		  }  
		  */
// Verifying Syntax
	   
	   /* loop through token array verifying its syntax
	   count the number of lines by the \n character */
	 
	   i = 0;
	   int hasError = 0;
	   int saw_left_paren = 0;
	   count_pipes = 0; 
	   count_ampersands = 0;

	   for( i = 0; i <= token_array_index; i++)
	   {
	   	  	j=0;
		  	
		  	while(token_array[i][j] != NULL || token_array[i][j] != '\0')
		  	{
		  		if( token_array[i][j] == '`')
		  		{
		  			hasError = 1; 
		  		}
		  	 // Subshell Command
		  		if(token_array[i][j] == '(')
		  		{
		  			saw_left_paren++;
		  			if(saw_left_paren == 2)
		  			{
		  					
		  			}
		  		}
		  		else if (token_array[i][j] == ')')
		  		{
		  			if(saw_left_paren)
		  			{
		  				saw_left_paren--;
		  			}
		  			else 
		  			 	hasError = 1;
		  		}
		  		else if( token_array[i][j] == ';')
		  		{
		  			if(saw_left_paren > 0)
		  				hasError = 1;
		  		}
	         // OR Command and Pipe Command
					if (token_array[i][j] == '|')
					{
						count_pipes++;
						if ( token_array[0][0] == '|' ||
							 (count_pipes == 2 && token_array_index == i) ||
							  count_pipes == 2 && token_array_index != i && 
							  !isNormalCharacter(token_array[i+1][0]) && token_array[i+1][0] != '(')	
							{
							hasError = 1;
						}
						else if(count_pipes == 2 && token_array[i][j+1] == '|') 
							hasError = 1;
						else if((count_pipes == 2 || count_pipes == 1) && !isNormalCharacter(token_array[i-1][0]) 
						&& token_array[i-1][0] != ')')
							hasError = 1;
					}
					else
					{
						count_pipes = 0;
					}
			 // AND Command 
				
					if(token_array[i][j] == '&')
					{
						count_ampersands++;
						if(  token_array[0][0] == '&'   || 
							(count_ampersands == 1 && token_array[i][j+1] == '\0') ||
						(count_ampersands == 2 && token_array[i][j+1] == '&') )
						hasError = 1;
					}
					else 
					{
						count_ampersands = 0;
					}
			 // Redirections 
				 if ( (i == 0 && token_array[i][j] == '<') ||
				 	  (token_array[i][0] == '<' && !isNormalCharacter(token_array[i-1][0])) )
				 	hasError = 1; 
			     else if (token_array_index != i)
				 {
			  	  if( (isRedirect(token_array[i+1][0]) && token_array_index == i+1) ||
				  	  (isRedirect(token_array[i][0]) && !isNormalCharacter(token_array[i+1][0])) )
				   	{	
				  			hasError = 1;
				  	}
				 }
				 else if (token_array_index == i && isRedirect(token_array[i][0]))
			     {
				  		hasError = 1;
				 }
			    j++;
	   		 }
	   		 
	 // Sequence Command 
	 //	must have the form: a ; b 
		   // ';'
		   if (token_array[0][0] == ';')
			   	hasError = 1; 
		   // '; a'
		   if (token_array_index != i)
		   {
		   	
		   	if(i != 0 && token_array[i][0] == ';' && token_array[i][1] == '\0' &&
		   		!isNormalCharacter(token_array[i-1][0]) && token_array[i-1][0] != ')' )
		   	{
		   		hasError = 1;
		   	}
		   }
		   
		   if (hasError)
	     	{
		  		break;
		  	}
		if(token_array[i][0]== ';')
	   		line_num++;
		 }  
	   if (hasError)
	   {
	   	fprintf(stderr, "%i: Syntax error\n",line_num);
	   	exit(1);	
	   } 
//******************************** stack implementation ****************************************************************
									  	
  char ** sub_token_array = malloc(sizeof(char *)); 
  int sub_token_array_index = 0;
 
  node * cmd_stack = malloc(sizeof(node));
  cmd_stack->size = 0;
  node *op_stack = malloc(sizeof(node));
  op_stack->size = 0;

  command_t cmd;
  command_t tmp_cmd; 
  int type; 

  //precedence 1 being lowest, 3 highest
  int curr_precedence = 0;
  int prev_precedence = 0;

  command_stream_t cmd_stream = NULL;
  int command_stream_index = 0; 

  for( i = 0; i < token_array_index+1; i++)
  {

	  	//case operations
  	if ( token_array[i][0] == '&' || token_array[i][0] == ';'  || (token_array[i][0] == '|' ) )
  	{

		if(sub_token_array_index != 0)
		{

			sub_token_array[sub_token_array_index] = '\0';
			if (sub_token_array[0][0] == '<' || sub_token_array[0][0] == '>')
			{
				
				command_t tmp = pop(&cmd_stack);

				j = 0;
				//char ** word_array = malloc(sizeof(char*));
				//check input
				tmp->input = NULL;
				tmp->output = NULL;
				if(sub_token_array[j][0] == '<')
				{
					j++;
					tmp->input = sub_token_array[j];
				}
				else if(sub_token_array[j][0] == '>')
				{
					j++;
					tmp->output = sub_token_array[j];
				}	
				j++;

				//no output
				if(sub_token_array[j] == '\0')
				{	
					//free(tokens);

				}
				else if(sub_token_array[j][0] =='>')
				{
					j++;
					tmp->output = sub_token_array[j];
				}
				else
				{
					printf("shouldn't go in here!!!!");
				}
				//free(tokens);
				push(tmp,&cmd_stack);
				
				
			}//-------------------------------------------------------------------------
			else
			{
				cmd = createCommand(sub_token_array, simple);
				push(cmd,&cmd_stack);
			}
		}
		
  		if (token_array[i][0] == '&')
  		{
  			type = and;
  			curr_precedence = 2;
  		}
  		else if (token_array[i][0] == ';' && token_array[i][1] == '\0' 
  			&& token_array_index != i)
  		{
  			type = sequence;
  			curr_precedence = 1;
  		}
  		else if (token_array[i][0] == '|' && token_array[i][1] == '|')
  		{
  			type = or; 
  			curr_precedence = 2;
  		}
  		else if (token_array[i][0] == '|' && token_array[i][1] == '\0')
  		{
  			type = pipe;
  			curr_precedence= 3;
  		}
  		else
  		{
  			type = 0; //new command tree
  			curr_precedence = 0;
  		}
  		
  		if(op_stack->size != 0)
  		{
  			if(top(&op_stack)->type == SEQUENCE_COMMAND)
  			{
  				prev_precedence = 1;
  			}
  			else if(top(&op_stack)->type == AND_COMMAND || top(&op_stack)->type == OR_COMMAND)
  			{
  				prev_precedence = 2;
  			}
  		 	else if(top(&op_stack)->type == PIPE_COMMAND)
  		 	{
  		 		prev_precedence = 3;
  		 	}	
  		}

  		while(op_stack->size != 0 && 
  			curr_precedence <= prev_precedence)
  		{
			tmp_cmd = pop(&op_stack);
	  		tmp_cmd->u.command[1] = pop(&cmd_stack);
	  		tmp_cmd->u.command[0] = pop(&cmd_stack);
	  		push(tmp_cmd,&cmd_stack); 

	  		if(op_stack->size != 0)
	  		{
	  			if(top(&op_stack)->type == SEQUENCE_COMMAND)
	  			{
	  				prev_precedence = 1;
	  			}
	  			else if(top(&op_stack)->type == AND_COMMAND || top(&op_stack)->type == OR_COMMAND)
	  			{
	  				prev_precedence = 2;
	  			}
	  		 	else if(top(&op_stack)->type == PIPE_COMMAND)
	  		 	{
	  		 		prev_precedence = 3;
	  		 	}
	  		}
  		}

		if( (token_array[i][0] == ';' && token_array[i][1] == ';'
		 && token_array[i-1][1] != '|' ) || 
			(token_array[i][0] == ';' && token_array[i][1] == '\0' &&
		token_array_index == i))
		 
		{
			if(cmd_stream == NULL)
			{
				cmd_stream = malloc(sizeof(struct command_stream));
				cmd_stream->cmds = malloc(sizeof(command_t));
				if(cmd_stack->size != 0)
				{
				(cmd_stream->cmds)[command_stream_index] = pop(&cmd_stack);
				command_stream_index++;
			
				}
			}		
			else
			{
				if(cmd_stack->size != 0)
				{
				cmd_stream->cmds = realloc(cmd_stream->cmds,(command_stream_index+1)*sizeof(command_t));
				cmd_stream->cmds[command_stream_index] = pop(&cmd_stack);
				command_stream_index++;
				}
			} 
			sub_token_array_index = 0;
  			sub_token_array = malloc(sizeof(char *));
			continue;
		}
  		cmd = createCommand(NULL, type);
  		push(cmd,&op_stack); 

  		sub_token_array_index = 0;
  		sub_token_array = malloc(sizeof(char *));
  	}
    // Subshell command
  	else if(token_array[i][0] == '(' && token_array[i][1] == '\0')
  	{
		if(sub_token_array_index != 0) 
		{
			sub_token_array[sub_token_array_index] = '\0';
			cmd = createCommand(sub_token_array, simple);
			push(cmd,&cmd_stack);
		}
		
  		cmd = createCommand(NULL, subshell);
		push(cmd,&op_stack);
 
  		sub_token_array_index = 0;
  		sub_token_array = malloc(sizeof(char *)); 
  	}
  	else if(token_array[i][0] == ')' && token_array[i][1] == '\0')
  	{
		if(sub_token_array_index != 0)
		{
			sub_token_array[sub_token_array_index] = '\0';
	  		cmd = createCommand(sub_token_array, simple);
	  		push(cmd,&cmd_stack);
			
			sub_token_array_index = 0;
	  		sub_token_array = malloc(sizeof(char *)); 
		}
  		while(op_stack->size != 0 && top(&op_stack)->type != SUBSHELL_COMMAND)
  		{
  			tmp_cmd = pop(&op_stack);
  			if(tmp_cmd->type == OR_COMMAND || tmp_cmd->type == AND_COMMAND || 
  			   tmp_cmd->type == SEQUENCE_COMMAND || tmp_cmd->type == PIPE_COMMAND)
  			{
  				tmp_cmd->input = NULL;
	  			tmp_cmd->output = NULL;
	  			tmp_cmd->u.command[1] = pop(&cmd_stack);
	  			tmp_cmd->u.command[0] = pop(&cmd_stack);
	  			push(tmp_cmd,&cmd_stack); 
  			}	
  			else if (tmp_cmd->type == SUBSHELL_COMMAND)
  			{
  				//cmd = createCommand(NULL,subshell);
  				tmp_cmd->u.subshell_command = pop(&cmd_stack);
				tmp_cmd->input = NULL;
				tmp_cmd->output = NULL;
  				push(tmp_cmd,&cmd_stack);				
				break;
  			}			
  		}
  		tmp_cmd = pop(&op_stack);
  		tmp_cmd->u.subshell_command = pop(&cmd_stack);
		tmp_cmd->input = NULL;
		tmp_cmd->output = NULL;
  		push(tmp_cmd,&cmd_stack);
  	}
  	else
  	{
  		sub_token_array[sub_token_array_index] = token_array[i];
  		sub_token_array_index++;
  		sub_token_array = realloc(sub_token_array, (sub_token_array_index+1)*sizeof(char *));
  	}
	}	
	cmd_stream->cmds[command_stream_index] = NULL;
  	return cmd_stream; 
		
	}

//******************************** read command stream ****************************************************************
command_t
read_command_stream (command_stream_t s)
{
  /* FIXME: Replace this with your implementation too.  */
	 
	if(s == NULL || s->cmds == NULL)
		return 0;
	else{
		command_t tmp = s->cmds[0];
		(s->cmds)++; 
  		return tmp;
  	}
}
