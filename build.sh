#!/bin/bash
clear

rm -rf ./build/*
rm -rf ./build/image/*

rm ./build/isodir/

mkdir ./build/
mkdir ./build/image/
mkdir ./build/isodir/boot/grub/

echo "---- Building ASM ----"

nasm -felf32 ./boot/boot.asm -o ./build/boot.out

echo "---- Compiling C ----"

gcc -m32 -g -O2 -fno-pie -ffreestanding -fno-stack-protector -I ./include/ -c ./kernel/kernel.c -o ./build/kernel.out

echo "---- Linking output files ----"

ld -m elf_i386 -T ./linker.ld -o ./out/kernel.bin ./out/boot.out ./out/kernel.out -nostdlib

echo "-------- x86 check --------"

if grub-file --is-x86-multiboot ./out/kernel.bin; then
  echo "Kernel binary is multiboot."
else
  echo "Kernel binary is not multiboot - check above for errors."
fi

echo "-------- copy --------"

cp ./build/kernel.bin ./build/isodir/boot/kernel.bin
cp ./grub.cfg ./build/isodir/boot/grub/grub.cfg

echo "-------- iso build --------"

cd ./build/
grub-mkrescue -o ./build/image/BlinkOS.iso isodir
cd ../

echo "---- Running in QEMU ----"

qemu-system-i386 -cdrom ./build/image/BlinkOS.iso -monitor stdio