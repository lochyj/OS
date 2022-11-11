clear

echo "---- Removing old files ----"

rm -rf ./out/*

echo "---- Building ----"

# Compiling the asm code

nasm ./boot/kernel_entry.asm -f elf32 -o ./out/kernel-entry.out

nasm ./boot/mbr.asm -f bin -o ./out/mbr.bin

nasm ./kernel/cpu/interrupt.asm -f elf32 -o ./out/asm.out

# Compiling the C code

gcc -m32 -ffreestanding -c ./kernel/kernel.c -o ./out/kernel.out -fno-pie -ffreestanding -fno-stack-protector

# Linking the files

ld -m elf_i386 -Ttext 0x1000 -shared ./out/kernel-entry.out ./out/kernel.out ./out/asm.out -e main -nostdlib -fstack-protector --oformat binary -o ./out/kernel.bin

cat ./out/mbr.bin ./out/kernel.bin > ./out/image.bin

echo "---- Running in QEMU ----"

#qemu-system-i386 -fda ./out/image.bin

qemu-system-i386 -S -fda ./out/image.bin -d guest_errors
