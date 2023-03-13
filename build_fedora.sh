clear

echo "---- Removing old files ----"

rm -rf ./out/*
rm -rf ./out/image/*

mkdir ./out/image
mkdir ./isodir/boot/grub

echo "---- Building ASM ----"

nasm -felf32 ./boot/boot.asm -o ./out/boot.out

nasm ./kernel/cpu/interrupt.asm -felf32 -o ./out/kasm.out

echo "---- Compiling C ----"

# Compiling the C code

gcc -m32 -g -O2 -fno-pie -ffreestanding -fno-stack-protector -I . -I ./libc/inc -c ./kernel/kernel.c -o ./out/kernel.out

echo "---- Linking output files ----"

ld -m elf_i386 -T ./linker.ld -o ./out/kernel.bin ./out/boot.out ./out/kasm.out ./out/kernel.out -nostdlib
# -e main -Ttext 0x9000

echo "-------- x86 check --------"
if grub2-file --is-x86-multiboot ./out/kernel.bin; then
  echo multiboot confirmed
else
  echo the file is not multiboot
fi

echo "-------- copy --------"

# Copy the kernel to the isodir
cp ./out/kernel.bin ./isodir/boot/kernel.bin
cp ./grub.cfg ./isodir/boot/grub/grub.cfg

echo "-------- iso build --------"

# Build the ISO file for running in QEMU
grub2-mkrescue -o ./out/image/BlinkOS.iso isodir

echo "---- Running in QEMU ----"

#qemu-system-i386 -kernel ./out/kernel.bin -monitor stdio

qemu-system-i386 -cdrom ./out/image/BlinkOS.iso -monitor stdio
