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
void deleteFile(char* name);

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
	buffer[7] = '2'; buffer[8] = '0';
	buffer[9] = '1'; buffer[10] = '9';
	interrupt(33,0,buffer,0,0);
	interrupt(33,0,"\r\n\0",0,0);

	/* Step 2 – write revised file */
	interrupt(33,8,"sp19\0",buffer,size);
	
	/* Step 3 – delete original file */
	interrupt(33,7,"spc03\0",0,0);

	while(1);
}

void deleteFile(char* name) 
{
	/*257 is where the directory is*/
	int i =0;
	int k = 0;
	int q =0;
	int holder=0;
	int getNameLength =0;
	int numberArray[24];
	char bufferDirectory[512];
	char bufferMap[512];

	interrupt(33,2,bufferDirectory,257,0);
	interrupt(33,2,bufferMap,256,0);

	while(name[i] != '\0')
	{
		++getNameLength;
		++i;
	}
	i =0;

	// Goes through all 16 files in directory
	while(i < 512)
	{	
		// Checks to see if filename matches specified file
		for(k=0; k< getNameLength; ++k)
		{

			if(bufferDirectory[i +k] != name[k])
			{
				break;
			}
		}
		// If the file name matches
		if(k == getNameLength)
		{
			interrupt(33,0,"File found, deleting\r\n\0",0,0);
			//If match set first byte of name to zero	
			bufferDirectory[i]= '0';
			interrupt(33,6,bufferDirectory,257,0);
			
			i=i+8;
				
			// Finds  sectors from the map
			while(q<512)
			{
				//overwrites if true
				if(bufferDirectory[i] == q)
				{
					bufferMap[q] = 0;
					++i;
				}
				++q;
			}
			
			interrupt(33,6,bufferMap,256,0);
			break;

		}
		i += 32;
	}
	// If file wasn't found
	if(i+1 > 512-getNameLength)
	{
		interrupt(33,15,0,0,0);
	}

}

void writeFile(char* name, char* buffer, int numberOfSectors)
{
	char bufferDirectory[512];
	char bufferMap[512];
	int k = 32;
	int getNameLength = 0;
	int i = 0;
	int f = 0;
	int g = 0;
	int q = 0;
	int bytesToFill = 0;
	int pos = 0;
	int emptySectors = 0;
	/*257 is where the directory is*/
	interrupt(33,2,bufferDirectory,257,0);
	/*256 is where the directory is*/
	interrupt(33,2,bufferMap,256,0);
	
	while(name[i] != '\0')
	{
		++getNameLength;
		++i;
	}

	
	while(k < 512)
	{

		//free space 
		if(bufferDirectory[k] == 0)
		{
			
			interrupt(33,0,"Free Space found\r\n\0",0,0);

			// Inserts the file name filling the end with 0
			while(q != getNameLength)
			{
				bufferDirectory[k + q] = name[q];
				++q;
			}

			if (getNameLength < 8)
			{

				bytesToFill = 8 - getNameLength;
				pos = 0;
				while (bytesToFill != 0)
				{
					bufferDirectory[k + getNameLength + pos] = 0;
					++ pos; -- bytesToFill;
				}

			}

			// Finds the required amount of empty sectors to put the file in
			while(f < numberOfSectors)
			{
				g = 0;
				// Finds empty sectors from the map
				while(bufferMap[g] != 0)
				{
					++g;
				}

				// Updates map and directory
				bufferMap[g] = 255;
				bufferDirectory[k + 8 + f] = g;

				// writes 512 bits at a time to the specified empty sector
				interrupt(33,6,buffer + 512 * f,g,0);
				++f;
			}


			// fills in remaining directory sectors with a 0
			emptySectors = 24 - f;
			while(emptySectors != 0)
			{
			//	interrupt(33,0,"s\r\n\0",0,0);
				bufferDirectory[k + 8 + f] = 0;
				--emptySectors;
			}

			// Writes the map and directory back
			interrupt(33,0,"Wrote back",0,0);
			interrupt(33,6,bufferMap,256,0);
			interrupt(33,6,bufferDirectory,257,0);
			break;
		}
		//check for name
		if(bufferDirectory[k] != 0)
		{
			for(i=0; i< getNameLength; ++i)
			{
				if(bufferDirectory[i+k] != name[i])
				{
					break;
				}
			}
			if(i == getNameLength)
			{
				interrupt(33,15,1,0,0);
				break;
			}
		}

		//full disk
		else
		{
			interrupt(33,15,2,0,0);
			break;
		}
		k += 32;
	}
}

void readFile(char* fname, char* buffer, int* size)
{
	/*257 is where the directory is*/
	int i =0;
	int k = 0;
	int getNameLength=0;
	int q =0;
	char bufferDirectory[512];
	interrupt(33,2,bufferDirectory,257,0);
	
	while(fname[i] != '\0')
	{
		++getNameLength;
		++i;
	}
	i =0;

	// Goes through all 16 files in directory
	while(i < 512)
	{	
		// Checks to see if filename matches specified file
		for(k=0; k< getNameLength; ++k)
		{

			if(bufferDirectory[i +k] != fname[k])
			{
				break;
			}
		}
		// If the file name matches
		if(k == getNameLength)
		{
			interrupt(33,0,"File found\r\n\0",0,0);
			// After finding file, using the 24 remaining bytes of the sector, read each sector until '0' is reached
			// denoting the end of the file
			while(q < 24)
			{
				int currentSector = bufferDirectory[i + 8 + q];

				// Read each sector into buffer
				if(currentSector != 0){
					readSector(buffer + 512 * q, currentSector);
				}
				else
				{
					// Updates the amounts of sectors requried by file
					*size = q;
					break;
				}

				++q;				
			}
			break;
		}
		i += 32;
	}
	// If file wasn't found
	if(i+1 > 512-getNameLength)
	{
		interrupt(33,15,0,0,0);
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
		case 7:
			deleteFile(bx);
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
