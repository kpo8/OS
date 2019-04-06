#include "blackdos.h"

#define BOOT 0
#define CLRS 1
#define COPY 2
#define DDIR 3
#define ECHO 4
#define EXEC 5
#define HELP 6
#define PRNT 7
#define REMV 8
#define SENV 9
#define SHOW 10
#define TWET 11

struct menuChoices
{
	char *key; 
	int val; 
};

static menuChoices options[] =
{	
	{ "BOOT", A1 },
	{ "CLRS", A1 },
	{ "COPY", A1 },
	{ "DDIR", A1 },
	{ "ECHO", A1 },
	{ "EXEC", A1 },
	{ "HELP", A1 },
	{ "PRNT", A1 },
	{ "REMV", A1 },
	{ "SENV", A1 },
	{ "SHOW", A1 },
	{ "TWET", A1 }
};

void terminalCommands(char *s);
int getOption(char *key);
void stringCompare(char one[], char two[]);

void main()
{	
	/*char* n;
	PRINTS("~_~: \0");
	terminalCommands(SCANS(n));
	END;
	*/
}

int stringCompare(char one[], char two[])
{
	//https://code.woboq.org/userspace/glibc/string/strcmp.c.html
}	

int getOption(char *s)
{
	//need to make
	// a string compare function
}

void terminalCommands(char *s)
{
	switch (getOption(s)) 
	{
		case BOOT:
		       // put the interrupt stuff here. below is just a test if you get this message above it works	
			PRINTS("THIS WORKS \0");
			break;
		case CLRS: 
			break;
		case COPY:
		       	break;
		case DDIR: 
			break;
		case ECHO: 
			break;
		case EXEC: 
			break;
		case HELP: 
			break;
		case PRNT: 
			break;
		case REMV: 
			break;
		case SENV: 
			break;
		case SHOW: 
			break;
		case TWET: 
			break;
		default:
			PRINTS("BAD OPTION" \0\n);
	}
}
