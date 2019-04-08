#include "blackdos.h"

#define BADOPTION -1
#define BOOT0 0
#define CLRS1 1
#define COPY2 2
#define DDIR3 3
#define ECHO4 4
#define EXEC5 5
#define HELP6 6
#define PRNT7 7
#define REMV8 8
#define SENV9 9
#define SHOW10 10
#define TWET11 11
#define MAXOPTIONS 11

typedef struct 
{
	char *inputString; 
	int val; 
} menuChoices;


static menuChoices options[]=
{	
	{ "boot", BOOT0 },
	{ "clrs", CLRS1 },
	{ "copy", COPY2 },
	{ "ddir", DDIR3 },
	{ "echo", ECHO4 },
	{ "exec", EXEC5 },
	{ "help", HELP6 },
	{ "prnt", PRNT7 },
	{ "remv", REMV8 },
	{ "senv", SENV9 },
	{ "show", SHOW10 },
	{ "twet", TWET11 }
};

       /*
	clrs clears screan \
	copy -arg1 -ar2 \
	      copys file \
	ddir displays disk directory \
	echo displays comment \	
	exec starts program \
	help displays this message \
	prnt -arg1\
	      prints file contents \
	remv -arg1 \
	      deletes file \
	senv  set environment variables; load and execute Stenv at segment 4 \
	show -arg1 \
	      displays file contents \
	twet -arg1 \
	      creates and saves text file, saves to filenamen";
*/
void terminalCommands(char *s);
int getOption(char *key);
int stringCompare(char one[10], char two[10]);

//global buffer 
char buffer[12288];
int size;
void main()
{
	//load configuration files
	interrupt(33,2,buffer,258,0);

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
	char *argument1;
	char *argument2;
	
	PRINTS("\n\r\0");
	switch (getOption(s)) 
	{
		case BOOT0:
		     	BOOT;  			
			break;
		case CLRS1:
			CLRS;	
			break;
		case COPY2: //This I am stuck on
		//	SCANS(argument1);
		//	SCANS(argument2);
		//	COPY(argument1,argument2);
		       	break;
		case DDIR3: //Needs done
			break;
		case ECHO4: 
			SCANS(argument1);
			while(*argument1 == '\0')
			{
				ECHO(argument1);
			}
			break;
		case EXEC5: 
			break;
		case HELP6:
		       HELP;	
			break;
		case PRNT7: 
			break;
		case REMV8: 
			break;
		case SENV9: 
			break;
		case SHOW10: 
			break;
		case TWET11: 
			break;
		case BADOPTION:
			PRINTS("BAD OPTION \n\r\0");
			break;
	}
}
