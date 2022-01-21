//--------------------------------------------------------------------
// shell.c
// Implementation of Custom Simple Shell
// Author: Chase Howard
// Class: CSS 503B [University of Washington Bothell]
// Date: 2021-04-13
//-------------------------------------------------------------------- 
//	Description: 
//    This program serves as a custom shell interface that accepts user 
//    commands via the console and executes each command in a separate process.
//    This utilizes system calls to manage timing and process management.
//    This program is capable of executing input and output of files to a 
//    command using '>' or '<' as well as piping commands together with '|'

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <string.h>
#include <fcntl.h>
#include <assert.h>

#define MAX_LINE 80 /* The maximum length command */
#define MAX_STRS MAX_LINE/2 + 1 // maximum number of strings
enum {NONE, INPUT, OUTPUT, PIPE};
enum {RD, WR};

//------------------------------------------------------------------------
//                                  MUTATORS
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// tokenize: splits input char array into string array based on ' ' and '\n'
//------------------------------------------------------------------------
// Pre: input has been populated
// Post: argsLF has been populated with strings scanned from 'input' with 
//       seprations based on delimiters [' ', '\n']. The size of the argsLF
//       array is returned upon success, returns -1 on fail. 
int tokenize(char input[MAX_LINE], char* argsLF[])
{
      int size = 0;
      
      // parse input string to argsLF array
      char* sbstr;
      sbstr = strtok(input, " \n");
      while(sbstr != NULL)
      {
         (argsLF)[size] = sbstr;
         size++;
         sbstr = strtok(NULL, " \n");
      }

      return size - 1; // decrement to account for loop increment
}

//------------------------------------------------------------------------
// splitInput: splits char array into two separate arrays based
//       on pre-defined delimiters. New arrays store in arguments passed. 
//       Fxn returns value indicating the index of the delimiter
//------------------------------------------------------------------------
// Pre: input has been populated, inputLF and inputRT have been created
//       and allocated memory appropriately externally
// Post: Two arrays passed as arguments have been populated according to the
//       delimiter location. Delimeters are defined as '<', '>', '|'. If no
//       delimiter is found, input is copied over to inputLF and -1 is returned
//       indicating the delimiter location. 
int splitInput(const char input[MAX_LINE], char inputLF[MAX_LINE], 
      char inputRT[MAX_LINE]){

   int i = 0, isLeft = 1, splitLoc = -1;
   char ch = input[i];

   // scan entire input until reaching EOL or NULL, copy to appropriate arr
   //    depending if a delimiter was seen or not.
   while(ch != '\0' && ch != '\n'){
      if(ch == '<' || ch == '>' || ch == '|'){
         isLeft = 0;
         splitLoc = i; 
      }
      else if(isLeft)
         inputLF[i] = input[i];
      else
         inputRT[i - splitLoc - 1] = input[i];
      i++;
      ch = input[i];
   }

   return splitLoc; // negate increment above
}

//------------------------------------------------------------------------
// updateHistory: checks input to check if user made a call to exe last cmd.
//       if not, fxn will update history with the most recent command.
//------------------------------------------------------------------------
// Pre: Input has been read in properly.
// Post: History has been updated with most recent command, OR user input cmd 
//    to call last cmd and history has overwritten the input.
int updateHistory(char* input, char* history)
{
   // assign to history or recall from history
   if(input[0] == '!' && input[1] == '!' && input[2] == '\n')
   {
      if(history[0] == '\0') 
      {
         printf("No commands in history.\n");
         return -1;
      }     
      else 
      {
         strcpy(input, history);
         printf("osh>%s\n", input);
      }
   }
   else
      strcpy(history, input);
   return 0;
}

//------------------------------------------------------------------------
// setOperatorFlag: checks input at specified split location and returns
//       enumerated flag value based on character.
//------------------------------------------------------------------------
// Pre: call has been made to 'splitInput' and the location of the delimiting
//       character has been determined.
// Post: input has been read to determine the appropriate case to handle based
//       on the operator identified, integer returned, defaults to NONE: 0 
int setOperatorFlag(int splitLoc, char input[])
{
   int operatorFlag = NONE;

   if(splitLoc > 0) // found a split in the input argsLF 
   {
      char ch = input[splitLoc];
      if(ch == '<')
         operatorFlag = INPUT;
      else if(ch == '>')
         operatorFlag = OUTPUT;
      else if(ch == '|')
         operatorFlag = PIPE;
      else
         fprintf(stderr,"Input parse error: invalid split at \"%c\"", ch);
   }

   return operatorFlag;
}

//------------------------------------------------------------------------
// setWait: scans argument array for ampersand at final position, returns a 
//          flag, removes '&' and updates args size
//------------------------------------------------------------------------
// Pre: argsLF has been populated appropriately to user input
// Post: argsLF has been accessed at the end position, reading the final str
//       in the argument array. Returns an int indicating if user has 
//       commanded the shell to wait for the child process or not. If '&' is 
//       found, the size of the argsLF[] is decremented. 
int setWait(char* argsLF[], int* size)
{
   int parentWait = 1; // default to waiting state
   
   // check final character of input for ampersand
   if(!strcmp(argsLF[*size],"&"))
   { 
      parentWait = 0;
      argsLF[*size] = NULL; // remove ampersand from argsLF
      *size -= 1; // adjust end
   }
   return parentWait;
}

int main(void)
{
   char* argsLF[MAX_STRS]; /* command line arguments */
   char input[MAX_LINE], history[MAX_LINE]; // store last cmd
   int should_run = 1; /* flag to determine when to exit program */
   memset(history, '\0', MAX_LINE); 

   while (should_run) 
   {
      printf("osh>");
      fflush(stdout);
      memset(argsLF, '\0', MAX_STRS * MAX_LINE * sizeof(char)); // flush argsLF
      memset(input, '\n', MAX_LINE);

      int status = 0;

      // get input
      fgets(input, MAX_LINE, stdin);
      if(input[0] == '\n') continue; // empty input

      // store command history, update history
      if(updateHistory(input, history) < 0)
         continue;
      
//-------------      SPLIT INPUT TO L/R ARGS      ------------------

      char inputLF[MAX_LINE], inputRT[MAX_LINE];
      char* argsRT[MAX_STRS];

      // Re-initialize / clear variables
      memset(argsRT, '\0', MAX_STRS * MAX_LINE * sizeof(char)); 
      memset(inputLF, '\0', sizeof(inputLF));
      memset(inputRT, '\0', sizeof(inputRT));

      int parentWait = 1; // default to waiting
      int endRT = -1; // index of final space of Right Args
      
      // Tokenize left side of input (default to argsLF)
      int splitLoc = splitInput(input, inputLF, inputRT);
      int endLF = tokenize(inputLF, argsLF);

      // test for exit command
      if(strcmp(argsLF[0], "exit") == 0) {
         should_run = 0;
         continue;
      }
      
      parentWait = setWait(argsLF, &endLF);

      //---------      UPDATE IF OPERATOR SCANNED FROM INPUT      -------------

      if(splitLoc > 0) // found a character to split in input argsLF
      {  
         endRT = tokenize(inputRT, argsRT);
         parentWait = setWait(argsRT, &endRT);
      }

      int operatorFlag = setOperatorFlag(splitLoc, input);

      //---------------------      FORK CHILD      ----------------------
      fflush(stdout);
      pid_t pid_Child = fork();

      if(pid_Child < 0)
      {
         fprintf(stderr, "Failed fork()");
         exit(EXIT_FAILURE);
      } 
      else if(pid_Child == 0) 
      { //--------------------     CHILD PROCESS     --------------------
         char* filename; 
         int fd = -1; // init to invalid state

         switch(operatorFlag)
         {
            case NONE: ; //------------   NO OPERAND [DEFAULT]    -------------
               execvp(argsLF[0], argsLF);
               exit(EXIT_FAILURE);
               break;

            case INPUT:  ; //---------------    INPUT '<'     -----------------
               assert(argsRT[0] != NULL);
               assert(splitLoc > 0);

               // open file / get file descriptor
               filename = argsRT[0];
               fd = open(filename, O_RDONLY);

               if(fd < 0)
               {
                  fprintf(stderr, "Input file not found: %s\n", filename);
                  close(fd);
                  break;
               }

               // redirect and execute, close on exec fail
               dup2(fd, STDIN_FILENO); 
               execvp(argsLF[0], argsLF);
               close(fd);
               exit(EXIT_FAILURE);
               break;

            case OUTPUT:  ; //-----------      OUTPUT '>'     ----------------
               assert(argsRT[0] != NULL);
               assert(splitLoc > 0);

               // check for file - overwrite if found
               filename = argsRT[0];
               fd = open(filename, O_WRONLY | O_TRUNC);
               // file not found - create new
               if(fd < 0)
               {
                  fd = open(filename, O_WRONLY | O_CREAT);
                  fprintf(stderr, "File not found\nCreating new: %s\n", 
                     filename);
                  if(fd < 0)
                  {   
                     fprintf(stderr,"Failed to create file\n");
                     close(fd);
                     exit(EXIT_FAILURE);
                  }
               }

               // redirect and execute, close on exec fail
               dup2(fd, STDOUT_FILENO);
               execvp(argsLF[0], argsLF);
               close(fd);
               exit(EXIT_FAILURE);
               break;
            
            case PIPE:  ;  //-----------        PIPE '|'      ----------------
               assert(argsRT[0] != NULL);
               assert(splitLoc > 0);

               // Create Pipe
               int p[2]; 
               if(pipe(p) < 0){
                  fprintf(stderr,"Error in creating pipe\n");
                  exit(EXIT_FAILURE);
               }

               //------------------       FORK GRANDCHILD      ----------------
               pid_t pid_Gchild = fork();

               if(pid_Gchild < 0)
                  fprintf(stderr, "Failed fork");
               else if(pid_Gchild == 0)
               {  //---------------     GRANDCHILD [WR Pipe]    ------------
                  // close reader, redirect output, execute, close pipe on fail
                  close(p[RD]); 
                  dup2(p[WR], STDOUT_FILENO); 
                  execvp(argsLF[0], argsLF); 
                  close(p[WR]);
               }
               else
               {  //---------------     CHILD [RD Pipe]    ------------
                  // close writer, redirect input, execute, close pipe on fail
                  wait(&pid_Gchild); // not needed?
                  close(p[WR]); // this will wait for buffer to populate 
                  dup2(p[RD], STDIN_FILENO); 
                  execvp(argsRT[0], argsRT);
                  close(p[RD]);
               }

               exit(EXIT_FAILURE);
               break;

            default : ;
               // catch any Undefined behavior
               fprintf(stderr, "Invalid input: \"%s\"\n", input);
               exit(EXIT_FAILURE);
               break;
         }

         exit(EXIT_FAILURE); // not needed
      } 
      else      //---------------     PARENT PROCESS     -------------
      { 
         if(parentWait)
         {
            wait(&pid_Child); // wait for child
            
            if(pid_Child) // non-0 exit, error child
               fprintf(stderr,"Error, child exit status: %d\n", pid_Child);
         }
         else
         {
            // check status w/out wait
            waitpid(pid_Child, &status, WNOHANG);
            if(status)
               fprintf(stderr,"Error, child exit status: %d\n", status);
         }     
      }
   }
   return 0;
}