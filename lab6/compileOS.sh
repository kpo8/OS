!#/bin/sh
#Execute this command to give .sh script permissions chmod +x compileOS.sh.
#Compiles assembly code for boatloader
echo "Building Bootloader"
nasm bootload.asm
#make empty floppy.img
dd if=/dev/zero of=floppya.img bs=512 count=2880
#copy bootload.asm to empty img
dd if=bootload of=floppya.img bs=512 count=1 conv=notrunc
#copy map to floppya.img
dd if=map of=floppya.img bs=512 count=1 seek=256 conv=notrunc
#copy config to floppya.img
dd if=config of=floppya.img bs=512 count=1 seek=258 conv=notrunc
#kernal
echo "building Kernel"
#build assembly code first
as86 kernel.asm -o kasm.o
bcc -ansi -c -o kernel.o kernel.c
#link kernal.o to kernel.asm
ld86 -o kernel -d kernel.o kasm.o

bcc -ansi -c -o fib.o fib.c
bcc -ansi -c -o cal.o cal.c
bcc -ansi -c -o t3.o t3.c
as86 blackdos.asm -o bdos_asm.o
ld86 -o fib -d fib.o bdos_asm.o
ld86 -o cal -d cal.o bdos_asm.o
ld86 -o t3 -d t3.o bdos_asm.o

#copy kernel to image
dd if=kernel of=floppya.img bs=512 conv=notrunc seek=259


#This maybe lab4 specific but it cannot hurt having it in here
gcc loadFile.c -o loadfile

#Compile shell file
bcc -ansi -c -o shell.o shell.c
ld86 -o shell -d shell.o bdos_asm.o

echo "loading fib exec"
./loadfile fib

echo "loading test exec"
./loadfile test
echo "loading Stenv"
./loadfile Stenv

echo "loading t3"
./loadfile t3

echo "loading cal"
./loadfile cal


#load shell to kernel
echo "loading shell"
./loadfile shell

#echo "tarring lab"
#cd ..
#tar -czvf kevinONeil_GarrettMcDonnell_AleksDrobnjak.tar.gz lab3
#mv kevinONeil_GarretMcDonnel_AleksDrobnjak.tar.gz  lab3/ 
