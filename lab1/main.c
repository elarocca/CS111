// UCLA CS 111 Lab 1 main program

#include <errno.h>
#include <error.h>
#include <getopt.h>
#include <stdio.h>
#include "parallel.c"
#include "list.c"
#include "stack.c"

static char const *program_name;
static char const *script_name;

static void
usage (void)
{
  error (1, 0, "usage: %s [-pt] SCRIPT-FILE", program_name);
}

static int
get_next_byte (void *stream)
{
  return getc (stream);
}

int
main (int argc, char **argv)
{ 
  int command_number = 1;
  bool print_tree = false;
  bool time_travel = false;
  program_name = argv[0];

  for (;;)
    switch (getopt (argc, argv, "pt"))
      {
      case 'p': print_tree = true; break;
      case 't': time_travel = true; break;
      default: usage (); break;
      case -1: goto options_exhausted;
      }
 options_exhausted:;

  // There must be exactly one file argument.
  if (optind != argc - 1)
    usage ();

  script_name = argv[optind];
  FILE *script_stream = fopen (script_name, "r");
  if (! script_stream)
    error (1, errno, "%s: cannot open", script_name);
  command_stream_t command_stream =
    make_command_stream (get_next_byte, script_stream);

  command_t last_command = NULL;
  command_t command;



  while ((command = read_command_stream (command_stream)))
      {
        if (print_tree)
        {
          printf ("# %d\n", command_number++);
          print_command (command);

          Queue* readQueue = NULL;
          Queue* writeQueue = NULL;
          processCommand(command,&readQueue,&writeQueue);
          char** tmp;
 
          

          listNode tmpNode;
          tmpNode.readQueue = &readQueue;
          tmpNode.writeQueue = &writeQueue;


         Queue* tmpReadQueue;
         Queue* tmpWriteQueue;
         listNode * printNode;

          tmpReadQueue = tmpNode.readQueue;
          tmpWriteQueue = tmpNode.writeQueue;  
               
/*
          while(tmp = dequeue(tmpReadQueue)){
            printf("%s%c",tmp,' ');
          }
          printf("\n");

          
          while(tmp = dequeue(tmpWriteQueue)){
            printf("%s%c",tmp,' ');
          } 
          */
          /*
          char** readWord;
          char** writeWord;
          Queue* readItr = tmpReadQueue;
          Queue* writeItr = tmpWriteQueue;

          printf("%s",readItr->data);
          /*
          while(readItr != NULL){
            writeItr = tmpWriteQueue;
            while(writeItr != NULL){
              writeItr = writeItr->next;
            }
            readItr = readItr->next;
          }
          */


          
          
          

/*
          Stack tmpList;
          pushStack('c',&tmpList);

         Queue* tmpReadQueue; 
         Queue* tmpWriteQueue;
         listNode * printNode;
      
         //printNode = popStack(&tmpList);
         //tmpReadQueue = printNode->readQueue;
        //tmpWriteQueue = printNode->writeQueue;
        //printNode = popStack(&tmpList);
     
     
        if(printNode == NULL){
          //printf("asdf");
        }
        char tmpChar;

        while(tmpChar = popStack(&tmpList))
        {
          printf("1 ");
          printf("%c",tmpChar);
          //tmpReadQueue = printNode->readQueue;
          /*
           tmpReadQueue = printNode->readQueue;
           tmpWriteQueue = printNode->writeQueue;
        
            /* 
          while(tmp = dequeue(tmpReadQueue)){
            printf("%s%c",tmp,' ');
            }
            printf("\n");
          while(tmp = dequeue(tmpWriteQueue)){
            printf("%s%c",tmp,' ');
            }
          */
     //   }

/*
          listNode * printNode;
         printNode = popStack(&tmpList);
         Queue* tmpQueue = printNode->readQueue;
         tmp = dequeue(tmpQueue);
           printf("%s%c",tmp,' ');
         //  popStack(&tmpList);


         while(printNode = popStack(&tmpList))
          {

             tmp = dequeue(printNode->readQueue);
             printf("%s%c",tmp,' ');
             printf("\n");
             //tmp = dequeue(&(printNode->writeQueue));
             //printf("%s%c",tmp,' ');
         }
*/
          /*
          while(tmp = dequeue(&readQueue)){
            printf("%s%c",tmp,' ');
          }
          printf("\n");
          while(tmp = dequeue(&writeQueue)){
            printf("%s%c",tmp,' ');
          } 
          */ 

        }
        else
        {
          last_command = command;
          execute_command (command, time_travel);

        }
      }


 
	



	
//--------------------------------------------	
	/* //good stuff don't mess it up
  int command_number = 1;
  bool print_tree = false;
  bool time_travel = false;
  program_name = argv[0];

  for (;;)
    switch (getopt (argc, argv, "pt"))
      {
      case 'p': print_tree = true; break;
      case 't': time_travel = true; break;
      default: usage (); break;
      case -1: goto options_exhausted;
      }
 options_exhausted:;

  // There must be exactly one file argument.
  if (optind != argc - 1)
    usage ();

  script_name = argv[optind];
  FILE *script_stream = fopen (script_name, "r");
  if (! script_stream)
    error (1, errno, "%s: cannot open", script_name);
  command_stream_t command_stream =
    make_command_stream (get_next_byte, script_stream);

  command_t last_command = NULL;
  command_t command;
  while ((command = read_command_stream (command_stream)))
    {
      if (print_tree)
    	{
    	  printf ("# %d\n", command_number++);
    	  print_command (command);
    	}
      else
    	{
    	  last_command = command;
    	  execute_command (command, time_travel);
    	}
    }

  return print_tree || !last_command ? 0 : command_status (last_command);
  */
}

