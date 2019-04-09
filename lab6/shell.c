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

void terminalCommands(char *s);
int getOption(char *key);
int stringCompare(char one[10], char two[10]);
void forCopy(char* s);
void getCommand(char* s, char* command);

//global buffer 
char buffer[12288];

void main()
{
	//load configuration files
	interrupt(33,2,buffer,258,0);

	while(1)
	{
		char* n;
		PRINTS("^(~(oo)~)^: \0");
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

       	char command[5];
       	command[4] = '\0';
       	getCommand(s,command);

        for(i =0; i < MAXOPTIONS; ++i)
        {
                if(stringCompare(options[i].inputString, command) == 0)
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
		case BOOT0:
		     	BOOT;  			
			break;
		case CLRS1:
			CLRS;	
			break;
		case COPY2:
			forCopy(s);
			break;
		case DDIR3: 
			break;
		case ECHO4: 
			break;
		case EXEC5: 
			break;
		case HELP6: 
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

void forCopy(char* s)
{
	char file1[15];
	char file2[15];
	int size =0;
	int i=5;   // ignores copy 
	int a = 0;

	while(s[i] != ' ')  // set file1 name
	{
		if (s[i] == ' ')
		{
			break;
		}
		if(s[i] != '\0')
		{
			file1[a] = s[i];
		}
		++i;
		++a;
	}

	file1[a]='\0';

	a=0;
	while(s[i] != '\0')  // set file2 name
	{
		if (s[i]=='\0')
		{
			break;
		}
		
		file2[a]=s[i];
		PRINTN(i);
		++i;
		++a;
	}
	file2[a]='\0';

//	interrupt(33,3,file1,buffer,size);  // Won't compile, need size
//	interrupt(33,8,file2,buffer,size);
}

void getCommand(char* s, char* command)
{
	int i = 0;
	for (i=0; i<4; ++i)
	{
		command[i]=s[i];
	}
}
