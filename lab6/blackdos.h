#define PRINTS(x)  interrupt(33,0,x,0,0)
#define PRINTN(x)  interrupt(33,13,x,0,0)
#define LPRINTS(x) interrupt(33,0,x,1,0)
#define LPRINTN(x) interrupt(33,13,x,1,0)
#define SCANS(x)   interrupt(33,1,x,0,0)
#define SCANN(x)   interrupt(33,14,&x,0,0)
#define END        interrupt(33,5,0,0,0)
#define BOOT       interrupt(33,11,0,0,0)
#define CLRS       interrupt(33,12,buffer[0]+1,buffer[1]+1,0)
#define EXEC(x)    {\
			interrupt(33,4,x,2,0);\
			interrupt(33,5,0,0,0);\
		   }
#define HELP    {\
			interrupt(33,0,"boot reboots OS\r\n\0",0,0);\
			interrupt(33,0,"clrs clears screen\r\n\0",0,0);\
			interrupt(33,0,"copy -arg1 -ar2 \r\n\0",0,0);\
			interrupt(33,0,"      copys file\r\n\0",0,0);\
			interrupt(33,0,"ddir displays disk directory\r\n\0",0,0);\
			interrupt(33,0,"exec starts program\r\n\0",0,0);\
			interrupt(33,0,"help displays this messager\r\n\0",0,0);\
			interrupt(33,0,"prnt file contents\r\n\0",0,0);\
			interrupt(33,0,"remv -arg1\r\n\0",0,0);\
			interrupt(33,0,"deletes file\r\n\0",0,0);\
			interrupt(33,0,"clrs clears screen\r\n\0",0,0);\
			interrupt(33,0,"senv  set environment variables; load and execute Stenv at segment 4\r\n\0",0,0);\
			interrupt(33,0,"show -arg1 \r\n\0",0,0);\
			interrupt(33,0,"displays file contents\r\n\0",0,0);\
			interrupt(33,0,"twet -arg1\r\n\0",0,0);\
			interrupt(33,0,"creates and saves text file, save to filename\r\n\0",0,0);\
}
#define SHOW(x) {\
			int size=0;\
			interrupt(33,3,x,buffer,&size);\
			buffer[7] = '2'; buffer[8] = '0';\
			buffer[9] = '1'; buffer[10] = '9';\
			interrupt(33,0,buffer,0,0);\
			interrupt(33,0,"\r\n\0",0,0);\
}
#define PRNT(x) {\
			int size=0;\
			interrupt(33,3,x,buffer,&size);\
			buffer[7] = '2'; buffer[8] = '0';\
			buffer[9] = '1'; buffer[10] = '9';\
			interrupt(33,0,buffer,1,0);\
}
#define TWET(x) {\
			int size=0;\
			char n[140];\
			interrupt(33,8,x,n,size);\
}
#define REMV(x)         interrupt(33,7,x,0,0)
