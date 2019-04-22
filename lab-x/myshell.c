// ACADEMIC INTEGRITY PLEDGE
//
// - I have not used source code obtained from another student nor
//   any other unauthorized source, either modified or unmodified.
//
// - All source code and documentation used in my program is either
//   my original work or was derived by me from the source code
//   published in the textbook for this course or presented in
//   class.
//
// - I have not discussed coding details about this project with
//   anyone other than my instructor. I understand that I may discuss
//   the concepts of this program with other students and that another
//   student may help me debug my program so long as neither of us
//   writes anything during the discussion or modifies any computer
//   file during the discussion.
//
// - I have violated neither the spirit nor letter of these restrictions.
//
//
//
// Signed: Garrett McDonnell, Aleksandar Drobnjak, Kevin O'Neil	Date: 4/16/2019

// 3460:426 Lab 4A - Basic C shell

/* Basic shell */

/*
 * This is a very minimal shell. It finds an executable in the
 * PATH, then loads it and executes it (using execv). Since
 * it uses "." (dot) as a separator, it cannot handle file
 * names like "minishell.h"
 *
 * The focus on this exercise is to use fork, PATH variables,
 * and execv. 
 */

#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

#define MAX_ARGS	64
#define MAX_ARG_LEN	16
#define MAX_LINE_LEN	80
#define WHITESPACE	" ,\t\n"

struct command_t {
   char *name;
   int argc;
   char *argv[MAX_ARGS];
};

/* Function prototypes */
int parseCommand(char *, struct command_t *);
void printPrompt();
void readCommand(char *);
bool checkMyCommands(struct command_t *, int pid, int status);
void listDirectoryContents(int pid, int status);
void displayManual();

int main(int argc, char *argv[]) {
   int pid;
   int status;
   char cmdLine[MAX_LINE_LEN];
   struct command_t command;

   while (1) {
      printPrompt();
      /* Read the command line and parse it */
      readCommand(cmdLine);

      // Checks to see if use pressed only the enter key, and then loops the prompt and takes input again
      if(cmdLine[0] == '\n')
 	    continue;

      parseCommand(cmdLine, &command);

      command.argv[command.argc] = NULL;

      fflush(stdout);

      /* Create a child process to execute the command */
      if ((pid = fork()) == 0) {
         /* Child executing command */
      	if (!checkMyCommands(&command,pid,status)) {
			execvp(command.name, command.argv);
			if (errno)
				printf("%s\n", "Error: command not valid.  Press H for help menu.");
		}
      }
      /* Wait for the child to terminate */
      wait(&status);
   }

   /* Shell termination */
   printf("\n\n shell: Terminating successfully\n");
   return 0;
}

/* End basic shell */

/* Parse Command function */

/* Determine command name and construct the parameter list.
 * This function will build argv[] and set the argc value.
 * argc is the number of "tokens" or words on the command line
 * argv[] is an array of strings (pointers to char *). The last
 * element in argv[] must be NULL. As we scan the command line
 * from the left, the first token goes in argv[0], the second in
 * argv[1], and so on. Each time we add a token to argv[],
 * we increment argc.
 */
int parseCommand(char *cLine, struct command_t *cmd) {
   int argc;
   char **clPtr;
   /* Initialization */
   clPtr = &cLine;	/* cLine is the command line */
   argc = 0;
   cmd->argv[argc] = (char *) malloc(MAX_ARG_LEN);
   /* Fill argv[] */
   while ((cmd->argv[argc] = strsep(clPtr, WHITESPACE)) != NULL) {
      cmd->argv[++argc] = (char *) malloc(MAX_ARG_LEN);
   }

   /* Set the command name and argc */
   cmd->argc = argc-1;
   cmd->name = (char *) malloc(sizeof(cmd->argv[0]));
   strcpy(cmd->name, cmd->argv[0]);
   return 1;
}

/* End parseCommand function */

/* Print prompt and read command functions - pp. 79-80 */

void printPrompt() {
   /* Build the prompt string to have the machine name,
    * current directory, or other desired information
    */
   char* promptString = "linux(3021287)|> ";
   printf("%s ", promptString);
}

void readCommand(char *buffer) {
   /* This code uses any set of I/O functions, such as those in
    * the stdio library to read the entire command line into
    * the buffer. This implementation is greatly simplified,
    * but it does the job.
    */
   fgets(buffer, 80, stdin);
}
/* End printPrompt and readCommand */

bool checkMyCommands(struct command_t *command, int pid, int status) {
	char* args[1];
	args[0]='\0';

	// Checks which command was issued and checks if the correct args were specified
	if (strcmp(command->name,"C") == 0) {
		if (command->argv[1] == NULL || command->argv[2] == NULL)
			printf("%s\n", "Error: command not valid.  Press H for help menu.");
		else
			execvp("cp",command->argv);

		return true;
	}
	else if (strcmp(command->name,"D") == 0) {
		if (command->argv[1] == NULL)
			printf("%s\n", "Error: command not valid.  Press H for help menu.");
		else
			execvp("rm",command->argv);

		return true;
	}
	else if (strcmp(command->name,"E") == 0) {
		execvp("echo",command->argv);
		return true;
	}
	else if (strcmp(command->name,"H") == 0) {
		if (command->argv[1] != NULL)
			printf("%s\n", "Error: command not valid.  Press H for help menu.");
		else
			displayManual();

		return true;
	}
	else if (strcmp(command->name,"L") == 0) {
		if (command->argv[1] != NULL)
			printf("%s\n", "Error: command not valid.  Press H for help menu.");
		else
			listDirectoryContents(pid,status);

		return true;
	}
	else if (strcmp(command->name,"M") == 0) {
		if (command->argv[1] == NULL)
			printf("%s\n", "Error: command not valid.  Press H for help menu.");
		else
			execvp("nano",command->argv);

		return true;
	}
	else if (strcmp(command->name,"P") == 0) {
		if (command->argv[1] == NULL)
			printf("%s\n", "Error: command not valid.  Press H for help menu.");
		else
			execvp("more",command->argv);

		return true;
	}
	else if (strcmp(command->name,"Q") == 0) {
		if (command->argv[1] != NULL)
			printf("%s\n", "Error: command not valid.  Press H for help menu.");
		else
			kill(pid, SIGTERM);

		return true;
	}
	else if (strcmp(command->name,"S") == 0) {
		if (command->argv[1] != NULL)
			printf("%s\n", "Error: command not valid.  Press H for help menu.");
		else
			execvp("firefox",args);

		return true;
	}
	else if (strcmp(command->name,"W") == 0) {
		if (command->argv[1] != NULL)
			printf("%s\n", "Error: command not valid.  Press H for help menu.");
		else
			execvp("clear", (char *const[]){"clear", NULL});

		return true;
	}
	else if (strcmp(command->name,"X") == 0) {
		if (command->argv[1] == NULL)
			printf("%s\n", "Error: command not valid.  Press H for help menu.");
		else
			execvp(command->argv[1], (char* const[]){command->argv[1],NULL});

		return true;
	}

	return false;

}

void listDirectoryContents(int pid, int status) {
	char *argm[] = {"ls", "-l", 0}; 

	printf("\n");
	// necessary to fork again, in order to use execvp twice for "L" command

	pid = fork(); 
	if (pid == 0)
		execvp("pwd",(char *const[]){"pwd", NULL});
	if (pid > 0) {
		pid = wait(&status);
		printf("\n");
		execvp(argm[0],argm);
	}
}

void displayManual() {
	printf("\n");
	printf("%s\n","C file1 file2 - Copy; create file2, copy contents of file1 to file2 without deleting file1.");
	printf("%s\n","D file - Delete the named file.");
	printf("%s\n","E comment - Echo; display comment on screen.");
	printf("%s\n","L - List the contents of the current directory.");
	printf("%s\n","M file - Make; create the named text file by launching a text editor.");
	printf("%s\n","P file - Print; display the contents of the named file on screen.");
	printf("%s\n","Q - Quit the shell.");
	printf("%s\n","S - Surf the web by launching a browser.");
	printf("%s\n","W - Wipe; clear the screen.");
	printf("%s\n","X program - Execute the named program.");
	printf("\n");
}
