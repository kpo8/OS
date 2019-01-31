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
/* Signed:Kevin O'Neil                          Date:1/31/19              */
/*                                                                        */
/*                                                                        */
/* 3460:4/526 BlackDOS2020 kernel, Version 1.01, Spring 2019.             */

void handleInterrupt21(int,int,int,int);
void printLogo();

void main()
{
	makeInterrupt21();
	printLogo();
	interrupt(33,0,"Hello world from Kevin O'Neil, Garret Mcdonnell, Alex Drobnjak.\\r\n\0",1,0);
	while(1);
}

void printString(char* c, int d)
{
	/* fill this in */
	if(d == 0)
	{
		int i = 0; 
		while(c[i] != '\0')
		{
			interrupt(16, 14*256+c[i], 0, 0, d);
			i = i +1;
		}
		return;
	}
	if(d == 1)
	{	
		int i = 0; 
		while(c[i] != '\0')
		{
			interrupt(23,c[i], 0, 0, 0);
			i = i +1;
		}
		return;
	}
}

void printLogo()
{
   interrupt(33,0,"       ___   `._   ____  _            _    _____   ____   _____ \r\n\0",0,0);
   interrupt(33,0,"      /   \\__/__> |  _ \\| |          | |  |  __ \\ / __ \\ / ____|\r\n\0",0,0);
   interrupt(33,0,"     /_  \\  _/    | |_) | | __ _  ___| | _| |  | | |  | | (___ \r\n\0",0,0);
   interrupt(33,0,"    // \\ /./      |  _ <| |/ _` |/ __| |/ / |  | | |  | |\\___ \\ \r\n\0",0,0);
   interrupt(33,0,"   //   \\\\        | |_) | | (_| | (__|   <| |__| | |__| |____) |\r\n\0",0,0);
   interrupt(33,0,"._/'     `\\.      |____/|_|\\__,_|\\___|_|\\_\\_____/ \\____/|_____/\r\n\0",0,0);
   interrupt(33,0," BlackDOS2020 v. 1.01, c. 2018. Based on a project by M. Black. \r\n\0",0,0);
   interrupt(33,0," Author(s): Kevin O'Neil, Garret Mcdonnell, Alex Drobnjak.\r\n\r\n\0",0,0);
}

/* MAKE FUTURE UPDATES HERE */
/* VVVVVVVVVVVVVVVVVVVVVVVV */



/* ^^^^^^^^^^^^^^^^^^^^^^^^ */
/* MAKE FUTURE UPDATES HERE */

void handleInterrupt21(int ax, int bx, int cx, int dx)
{
	//return;
	
	switch(ax) 
	{ 
	      case 0: 
		      printString(bx,cx); 
		      break;
	/*      case 1: case 2: case 3: case 4: case 5: */
	/*      case 6: case 7: case 8: case 9: case 10: */
	/*      case 11: case 12: case 13: case 14: case 15: */
	      default: 
		      printString("General BlackDOS error.\r\n\0");
	}
}
