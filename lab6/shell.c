#include "blackdos.h"

#define BADOPTION -1
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
#define MAXOPTIONS 11

typedef struct 
{
	char *inputString; 
	int val; 
} menuChoices;


static menuChoices options[]=
{	
	{ "boot", BOOT },
	{ "clrs", CLRS },
	{ "copy", COPY },
	{ "ddir", DDIR },
	{ "echo", ECHO },
	{ "exec", EXEC },
	{ "help", HELP },
	{ "prnt", PRNT },
	{ "remv", REMV },
	{ "senv", SENV },
	{ "show", SHOW },
	{ "twet", TWET }
};

void terminalCommands(char *s);
int getOption(char *key);
int stringCompare(char one[10], char two[10]);

void main()
{	
	while(1)
	{
		char* n;
		PRINTS("~_~: \0");
		SCANS(n);
		terminalCommands(n);
	}	
}

int stringCompare(char one[10], char two[10])
{
	int c = 0;
        
        while (one[c] == two[c])
        {
                if (one[c] == '\0' || two[c] == '\0')
                {
                         break;
                }
                c++;
        }

        if (one[c] == '\0' && two[c] == '\0')
        {
                return 0;
        }

        else
        {
                return -1;
        }
}	

//goes through possible options
int getOption(char *s)
{
	int i = 0;
        int trueOrFalse = -1;

        for(i =0; i < MAXOPTIONS; ++i)
        {
                if(stringCompare(options[i].inputString, s) == 0)
                {
                        return options[i].val;
                }
        }

        return BADOPTION;
}

void terminalCommands(char *s)
{
	PRINTS("\n\r\0");

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
		case BADOPTION:
			PRINTS("BAD OPTION \n\r\0");
			break;
	}
}
