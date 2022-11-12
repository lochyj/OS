clear

echo "---- Removing old files ----"

rm -rf ./out/*
rm -rf ./out/image/*

mkdir ./out/image

echo "---- Building ASM ----"

# Compiling the asm code

nasm ./boot/kernel_entry.asm -f elf32 -o ./out/kernel-entry.out

nasm ./boot/mbr.asm -f bin -o ./out/mbr.bin

nasm ./kernel/cpu/interrupt.asm -f elf32 -o ./out/asm.out

echo "---- Compiling C ----"

# Compiling the C code

gcc -m32 -fno-pie -ffreestanding -fno-stack-protector -c ./kernel/kernel.c -o ./out/kernel.out
#-g 

echo "---- Linking output files ----"
# Linking the files
# TODO: Fix linking issue where it won't link the out files properly and creates a file that is smaller than it should be
ld -m elf_i386 -shared -fstack-protector -o ./out/kernel.bin -Ttext 0x1000 ./out/kernel-entry.out ./out/asm.out ./out/kernel.out --oformat binary 
# -e main -nostdlib

echo "---- Adding MBR bin to kernel bin ----"

cat ./out/mbr.bin ./out/kernel.bin > ./out/image/image.img

echo "---- Running in QEMU ----"

#qemu-system-i386 -d int -no-reboot -fda ./out/image/image.img <-- debug

qemu-system-i386 -d int -no-reboot -fda ./out/image/image.img
