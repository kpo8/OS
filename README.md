#LAB3

Kevin O'Neil, Garrett McDonnell, Aleks Drobnjak

Functon readSector()
Reads a sector from floppy disk into a char* buffer given a specific absolute sector number

Functon writeSector()
Writes a sector to floppy disk using a char* buffer given a specific absolute sector number

Function clearScreen()
Clears the BDOS screen in order to set background and foreground colors


Run: 

	./compileOS.sh

	 echo "c" | bochs -f bdos.txt