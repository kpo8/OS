/* ACADEMIC INTEGRITY PLEDGE                                              */
/*                                                                        */
/* - I have not used source code obtained from another student nor        */
/*   any other unauthorized source, either modified or unmodified.        */
/*                                                                        */
/* - All source code and documentation used in my program is either       */
/*   my original work or was derived by me from the source code           */
/*   published in the textbook for this course or presented in            */
/*   class.                                                               */
/*                                                                        */
/* - I have not discussed coding details about this project with          */
/*   anyone other than my instructor. I understand that I may discuss     */
/*   the concepts of this program with other students and that another    */
/*   student may help me debug my program so long as neither of us        */
/*   writes anything during the discussion or modifies any computer       */
/*   file during the discussion.                                          */
/*                                                                        */
/* - I have violated neither the spirit nor letter of these restrictions. */
/*                                                                        */
/*                                                                        */
/*                                                                        */
/* Signed:Aleks Drobnjak, Kevin O'Neil, Garret Mcdonnell   Date:01/20/19  */
/*                                                                        */
/*                                                                        */
/* 3460:4/526 BlackDOS2020 kernel, Version 1.01, Spring 2018.             */

void handleInterrupt21(int,int,int,int);
void readSector(char*,int);
void writeSector(char*,int);
void clearScreen(int,int);
void readString();
void printLogo();
void printString(char* input);
void readInt(int*);
void writeInt(int);
int mod(int, int);
int div(int, int);
void error(int bx);
void readFile(char* fname, char* buffer, int* size);
void writeFile(char* name, char* buffer, int numberOfSectors);

void main()
{
	char buffer[12288]; 
	int size=0;
	makeInterrupt21();

	/* Step 0 – config file */
	interrupt(33,2,buffer,258,0);
	interrupt(33,12,buffer[0]+1,buffer[1]+1,0);
	printLogo();

	/* Step 1 – load/edit/print file */
	interrupt(33,3,"spc03\0",buffer,&size);
	buffer[7] = ‘2’; buffer[8] = ‘0’;
	buffer[9] = ‘1’; buffer[10] = ‘9’;
	interrupt(33,0,buffer,0,0);
	interrupt(33,0,"\r\n\0",0,0);

	/* Step 2 – write revised file */
	interrupt(33,8,"spr19\0",buffer,size);

	while(1);
}

void writeFile(char* name, char* buffer, int numberOfSectors)
{
	char bufferDirectory[512];
	char bufferMap[512];
	int k = 0;

	/*257 is where the directory is*/
	interrupt(33,2,bufferDirectory,257,0);
	/*256 is where the directory is*/
	interrupt(33,2,bufferMap,256,0);
	
	
	while(k < 512)
	{
		//free space 
		if(k % 32 == 0 && bufferDirectory[k] == 0)
		{
			interrupt(33,0,"Free Space found",0,0);
			break;
		}
		//no space
		else
		{
			interrupt(33,15,2,0,0);
			break;
		}
		++k;
	}
	//This condition happens that means we have a bad file name
//	if(name[i] == '\0')
//	{
//		interrupt(33,15,1,0,0);	
//	}	
}

void readFile(char* fname, char* buffer, int* size)
{
	/*257 is where the directory is*/
	int i =0;
	int k = 0;
	int q =0;
	char bufferDirectory[512];
	interrupt(33,2,bufferDirectory,257,0);
	
	while(fname[i] != '\0')
	{
		if(k == 511 && bufferDirectory[k] == '\0')
		{
			interrupt(33,15,0,0,0);
			break;
		}
		if(fname[i] == bufferDirectory[i])
		{
			++i;
			if(fname[i] == '\0')
			{
				break;
			}
		}
		++k;
	}
	if(fname[i] == '\0')
	{
		interrupt(33,0,"File found\r\n\0",0,0);	
		size = 512 + *size;
		
		while(q < size)
		{
			buffer[*size + q] = bufferDirectory[q];
			++q; 
		}

	}	
}	

void error(int bx)
{
	switch(bx)
	{
		case 0:
			interrupt(33,0,"File not found",0,0);
			break;
		case 1:
			interrupt(33,0,"Bad file name.",0,0);
			break;
		case 2:
			interrupt(33,0,"Disk full",0,0);
			break;
		default:
			interrupt(33,0,"General error",0,0);
			break;
	}
}


void readInt(int* n)
{
	int pos;
	char strToIntConvert[5];
	*n = 0;
	interrupt(33,1,&strToIntConvert,0,0);

	for(pos = 0; strToIntConvert[pos] != '\0'; ++pos)
	{
		*n = *n * 10 + strToIntConvert[pos] - '0';
	}
}

void writeInt(int x)
{
	char intToConvertStr[6];
	int pos = 3;
	intToConvertStr[pos] = '\0';
	--pos;
	while(x != 0)
	{
		intToConvertStr[pos] = (mod(x, 10) + '0');
		x = div(x, 10);
		--pos;	
	}
	interrupt(33,0, intToConvertStr,1,0);
}

int mod(int a, int b) 
{
	int x = a;
	while (x >= b) 
	{
		x = x - b;
	}
	return x;
}

int div(int a, int b) 
{
	int q = 0;
	while (q * b <= a) 
	{
		q++;
	}
	return (q - 1);
}

void printString(char* c, int d)
{
	/* fill this in */
	int i = 0;

	if(d == 0)
	{
		while(c[i] != '\0')
		{
			interrupt(16, 14*256+c[i], 0, 0, 0);
			++i;
		}
	}
	else if (d == 1)
	{
		while(c[i] != '\0')
		{
			interrupt(23, c[i], 0, 0, 0);
			++i;
		}
	}

	return;
}

// takes string as input and then displays it
void readString(char* input)
{
	int i =0;
	char* c;
	while(i < 80)
	{
		 c = interrupt(22,0,0,0,0);

		//if user types a backspace
		if(c == 8)
		{
			if(i != 0)
			{
				interrupt(16, 14*256+c, 0, 0, 0);
				i--;
			}
		}
		if(c != 8)
		{
			input[i] = c;

			//if use typed 'n' or the end of the string has been reached
			if(input[i] == 13 || i == 79)
			{
				input[i] = '\0';
				break;
			}
			else
			{
				interrupt(16, 14*256+input[i], 0, 0, 0);
				++i;
			}
		}
	}
	//prints out a new line, and then the string from the user
	printString("\r\n\0",0);
	printString(input,0);
	printString("\r\n\0",0);
}

void printLogo()
{
	printString("       ___   `._   ____  _            _    _____   ____   _____ \r\n\0",0);
	printString("      /   \\__/__> |  _ \\| |          | |  |  __ \\ / __ \\ / ____|\r\n\0",0);
	printString("     /_  \\  _/    | |_) | | __ _  ___| | _| |  | | |  | | (___ \r\n\0",0);
	printString("    // \\ /./      |  _ <| |/ _` |/ __| |/ / |  | | |  | |\\___ \\ \r\n\0",0);
	printString("   //   \\\\        | |_) | | (_| | (__|   <| |__| | |__| |____) |\r\n\0",0);
	printString("._/'     `\\.      |____/|_|\\__,_|\\___|_|\\_\\_____/ \\____/|_____/\r\n\0",0);
	printString(" BlackDOS2020 v. 1.01, c. 2018. Based on a project by M. Black. \r\n\0",0);
	printString(" Author(s): Aleks Drobnjak, Kevin O'Neil, Garret Mcdonnell.\r\n\r\n\0",0);
}

void readSector(char* buffer,int sector) 
{
	int AH = 2;  /* Read sector */
	int AL = 1;  /* number of sectors to read */
	int absSecNo = sector;
	int relSecNo = mod(absSecNo,18) + 1;
	int headNo = mod(div(absSecNo,18),2);
	int trackNo = div(absSecNo,36);
	int AX = AH * 256 + AL;
	int CX = trackNo * 256 + relSecNo;
	int DX = headNo * 256;

	/*interrupt(19,AH,AL,buffer,trackNo,relSecNo,headNo,0);*/
	interrupt(19,AX,buffer,CX,DX);
}

void writeSector(char* buffer,int sector)
{
	int AH = 3;  /* Write sector */
	int AL = 1;  /* number of sectors to write */
	int absSecNo = sector;
	int relSecNo = mod(absSecNo,18) + 1;
	int headNo = mod(div(absSecNo,18),2);
	int trackNo = div(absSecNo,36);
	int AX = AH * 256 + AL;
	int CX = trackNo * 256 + relSecNo;
	int DX = headNo * 256;

	interrupt(19,AX,buffer,CX,DX);
}

void clearScreen(int BX,int CX)
{
	printString("\r\n",0),printString("\r\n",0),printString("\r\n",0),printString("\r\n",0),printString("\r\n",0),printString("\r\n",0);
	printString("\r\n",0),printString("\r\n",0),printString("\r\n",0),printString("\r\n",0),printString("\r\n",0),printString("\r\n",0);
	printString("\r\n",0),printString("\r\n",0),printString("\r\n",0),printString("\r\n",0),printString("\r\n",0),printString("\r\n",0);
	printString("\r\n",0),printString("\r\n",0),printString("\r\n",0),printString("\r\n",0),printString("\r\n",0),printString("\r\n",0);

	interrupt(16,512,0,0,0);

	if ((BX>0 && BX<=8) && (CX>0 && CX<=16))
	{
		int dummy1 = BX-1;
		int dummy2 = CX-1;
		int temp = 4096 * dummy1 + 256 * dummy2;
		interrupt(16, 1536, temp, 0, 6223);
	}

}

/* ^^^^^^^^^^^^^^^^^^^^^^^^ */
/* MAKE FUTURE UPDATES HERE */

void handleInterrupt21(int ax, int bx, int cx, int dx)
{
	switch(ax) 
	{
		case 0:
			printString(bx,cx);
			break;
		case 1:
			readString(bx);
			break;
		case 2:
			readSector(bx,cx);
			break;
		case 3:
			readFile(bx, cx, dx);
			break;

		case 6:
			writeSector(bx,cx);
			break;
		case 8:
			 writeFile(bx,cx,dx);
			 break;
		/*  case 4: case 5: */
		/*  case 7: case 9: case 10: */
		/*  case 11:  */
		case 12:
			clearScreen(bx,cx);
			break;
		case 13:
			writeInt(bx);
			break;
		case 14:
			readInt(bx);
			break;
		case 15: 
			error(bx);
			break;
		default:
		printString("General BlackDOS error.\r\n\0");
	}

	/* return; */
}
