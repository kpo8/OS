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
#define MAXOPTIONS 12

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
void arg1(char* s);
void listFiles();

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
//DDIR funciton
void listFiles()
{

	// 257 is where the directory is
    int i =0;
    int k = 0;
    int getNameLength=0;
    int q = 0;
	int fileSize=0;
	int totalSize=0;
	char dummy[16];
    char bufferDirectory[512];
    interrupt(33,2,bufferDirectory,257,0);

   	// Goes through all 16 files in directory
    while(i < 512)
    {
        // lists files 
       	while(bufferDirectory[i+k] != '\0')
        {
			dummy[k] = bufferDirectory[i+k];
			++k;
        }

		dummy[k] = '\0';		

 		//lists sizes        
 		while(q < 24)
		{
			int currentSector = bufferDirectory[i + 8 + q];

			// Read each sector into buffer
			if(currentSector != 0)
			{
				// Updates the amounts of sectors requried by file
				++fileSize;
			}
			else
			{
				break;
			}
	
			++q;                            
		}

		totalSize += fileSize;	
	
		if(dummy[0] != '\0')
		{
			PRINTS(dummy);
			PRINTS(" \0");
			PRINTN(fileSize);
			PRINTS("\n\r\0");
		}

		//cleans up
		k = 0;
		q = 0;
		fileSize = 0;
   		i += 32;
    }

	PRINTS("DISK USAGE: ");
	PRINTN(totalSize);
	PRINTS("\n\r\0");
	PRINTS("FREE SPACE: ");
	PRINTN(256-totalSize);
	PRINTS("\n\r\0");}

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
	        listFiles();
			PRINTS("\n\r\0");	
			break;
		case ECHO4:
			arg1(s);
			PRINTS(s);
			PRINTS("\n\r\0");
			break;
		case EXEC5:
			arg1(s);
			EXEC(s);	
			break;
		case HELP6:
			HELP;	
			break;
		case PRNT7:
	        arg1(s);
			PRNT(s);	
			break;
		case REMV8:
	        arg1(s);
			REMV(s);	
			break;
		case SENV9: 
			SENV;
			break;
		case SHOW10:
	        arg1(s);
			SHOW(s);
			break;
		case TWET11:
			//arg1(s);
	     	//PRINTS("ENTER TEXT\r\n\0");
			//TWET(s);
			break;
		case BADOPTION:
			PRINTS("BAD OPTION \n\r\0");
			break;
	}
}

void arg1(char *s)
{
	char file1[20];
	int size =0;
	int i=5;   // ignores copy 
	int a = 0;
	
	if(s[4] == '\0')
	{
		PRINTS("Please enter an argument \n\r\0");
		SCANS(s);
		PRINTS("\n\r\0");
		arg1(s);
	}

	while(s[i] != '\0')  // set file1 name
	{
		if (s[i] == '\0')
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
	a = 0;
	while(file1[a] != '\0')
	{
		s[a] = file1[a];
		++a;
	}
	s[a] = '\0';
}


void forCopy(char* s)
{
	char file1[15];
	char file2[15];
	int i=5;   // ignores copy 
	int a = 0;
	int size =0;
	while(s[i] != ' ')  // set file1 name
	{
		if (s[i] == ' ')
		{
			break;
		}
		if(s[i] != ' ')
		{
			file1[a] = s[i];
		}
		++i;
		++a;
	}

	file1[a]='\0';

	a=0;
	++i;
	while(s[i] != '\0')  // set file2 name
	{
		if (s[i]=='\0')
		{
			break;
		}
		
		file2[a]=s[i];
		++i;
		++a;
	}
	file2[a]='\0';

	interrupt(33,3,file1,buffer,&size);
      	buffer[7] = '2'; buffer[8] = '0';
	buffer[9] = '1'; buffer[10] = '9';
//	interrupt(33,0,buffer,0,0);
	interrupt(33,8,file2,buffer,size);
}

void getCommand(char* s, char* command)
{
	int i = 0;
	for (i=0; i<4; ++i)
	{
		command[i]=s[i];
	}
}
