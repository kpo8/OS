!#/bin/sh
#Execute this command to give .sh script permissions chmod +x compileOS.sh.
#Compiles assembly code for boatloader
echo "Building Bootloader"
nasm bootload.asm
#make empty floppy.img
dd if=/dev/zero of=floppya.img bs=512 count=2880
#copy bootload.asm to empty img
dd if=bootload of=floppya.img bs=512 count=1 conv=notrunc
#hexdump it to make sure it is right
hexdump -C floppya.img
#kernal
echo "building Kernel"
#build assembly code first
as86 kernel.asm -o kasm.o
bcc -ansi -c -o kernel.o kernel.c
#link kernal.o to kernel.asm
ld86 -o kernel -d kernel.o kasm.o
#copy image to bootloader
dd if=kernel of=floppya.img bs=512 conv=notrunc seek=259
echo "tarring lab"
cd ..
tar -czvf kevinONeil_GarretMcDonnel_AleksDrobnjak.tar.gz lab1
mv kevinONeil_GarretMcDonnel_AleksDrobnjak.tar.gz  lab2/ 
