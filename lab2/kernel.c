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
void printString();

void main()
{
 makeInterrupt21();
 printString("Enter a string to display on screen!\r\n\0",0);
 readString();
 while (1) ;
}

void printString(char* c, int d)
{
   /* fill this in */
   int i = 0;

   if(d == 0){
      while(c[i] != '\0'){
         interrupt(16, 14*256+c[i], 0, 0, 0);
         ++i;
      }
   }
   else if (d == 1){
      while(c[i] != '\0'){
         interrupt(23, 14*256+c[i], 0, 0, 0);
         ++i;
      }
   }

   return;
}

// takes string as input and then displays it
void readString()
{
   int i;
   char input[80];

   for(i = 0; i < 80; ++i){
      char* c = interrupt(22,0,0,0,0);

      //if user types a backspace
      if(*c == 8){
         interrupt(16, 14*256+input[i], 0, 0, 0);
         i--;
      }
      else {
	 input[i] = c;
	 
         //if use typed 'n' or the end of the string has been reached
         if(input[i] == 13 || i == 79){
	    input[i] = '\0';
            break;
         }
         else{
            interrupt(16, 14*256+input[i], 0, 0, 0);
         }
      }
   }
   //prints out a new line, and then the string from the user
   printString("\r\n\0",0);
   printString(input,0);
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
   switch(ax) {
      case 0:
	 printString(bx,cx);
         break;
/*    case 1: case 2: case 3: case 4: case 5: */
/*    case 6: case 7: case 8: case 9: case 10: */
/*    case 11: case 12: case 13: case 14: case 15: */
      default: 
         printString("General BlackDOS error.\r\n\0");
   }

   /* return; */
}
