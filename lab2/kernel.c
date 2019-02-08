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
void readString();
void printLogo();
void printString(char* input);
void readInt(int*);
void writeInt(int);
int mod(int, int);
int div(int, int);

void main()
{
	char food[25], adjective[25], color[25], animal[25];
	int temp;
	makeInterrupt21();
	printLogo();
	
	interrupt(33,0,"\r\nWelcome to the Mad Libs kernel.\r\n\0",0,0);
	interrupt(33,0,"Enter a food: \0",0,0);
	interrupt(33,1,food,0,0);
	
	temp = 0;
	while ((temp < 100) || (temp > 120)) 
	{
		interrupt(33,0,"Enter a number between 100 and 120: \0",0,0);
		interrupt(33,14,&temp,0,0);
	}
	
	interrupt(33,0,"Enter an adjective: \0",0,0);
	interrupt(33,1,adjective,0,0);
	interrupt(33,0,"Enter a color: \0",0,0);
	interrupt(33,1,color,0,0);
	interrupt(33,0,"Enter an animal: \0",0,0);
	interrupt(33,1,animal,0,0);
	interrupt(33,0,"Your note is on the printer, go get it.\r\n\0",0,0);
	interrupt(33,0,"Dear Professor O\'Neil,\r\n\0",1,0);
	interrupt(33,0,"\r\nI am so sorry that I am unable to turn in my program at this time.\r\n\0",1,0);
	interrupt(33,0,"First, I ate a rotten \0",1,0);
	interrupt(33,0,food,1,0);
	interrupt(33,0,", which made me turn \0",1,0);
	interrupt(33,0,color,1,0);
	interrupt(33,0," and extremely ill.\r\n\0",1,0);
	interrupt(33,0,"I came down with a fever of \0",1,0);
	interrupt(33,13,temp,1,0);
	interrupt(33,0,". Next my \0",1,0);
	interrupt(33,0,adjective,1,0);
	interrupt(33,0," pet \0",1,0);
	interrupt(33,0,animal,1,0);
	interrupt(33,0," must have\r\nsmelled the remains of the \0",1,0);
	interrupt(33,0,food,1,0);
	interrupt(33,0," on my computer, because he ate it. I am\r\n\0",1,0);
	interrupt(33,0,"currently rewriting the program and hope you will accept it late.\r\n\0",1,0);
	interrupt(33,0,"\r\nSincerely,\r\n\0",1,0);
	interrupt(33,0,"(your name here)\r\n\0",1,0);
	
	while(1);
	//Lab1 code
	/*
	char input[80];

	makeInterrupt21();
	interrupt(33,0,"Enter a string to display on screen!\r\n\0",0,0);
	interrupt(33,1,input,0,0);
	readString();
	while (1) ;
	*/
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

void readInt(int* n)
{
	int pos;
	char strToConvert[5];

	interrupt(33,1,&strToConvert,0,0);

	for(pos = 0; strToConvert[pos] != '\0'; ++pos)
	{
		*n = *n * 10 + strToConvert[pos] - '0';
	}

}

void writeInt(int x)
{
	char intToConvert[5];
	int pos = 0;

	while(x != 0)
	{
		intToConvert[pos++] = (mod(x, 10) + '0');
		x = div(x, 10);
	}
	
	interrupt(33,0, &intToConvert,1,0);
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

/* MAKE FUTURE UPDATES HERE */
/* VVVVVVVVVVVVVVVVVVVVVVVV */



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
		/*  case 2: case 3: case 4: case 5: */
		/*  case 6: case 7: case 8: case 9: case 10: */
		/*  case 11: case 12: */
		case 13:
			writeInt(bx);
			break;
		case 14:
			readInt(bx);
			break;
		/*  case 15: */
		default:
		printString("General BlackDOS error.\r\n\0");
	}

	/* return; */
}
