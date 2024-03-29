
# TODO: Use make file...

nasm -f elf32 -o ./build/boot.out ./assemblies/boot.asm

nasm -f elf32 -o ./build/gdt_asm.out ./kernel/system/gdt/gdt.asm

nasm -f elf32 -o ./build/isr_asm.out ./kernel/system/interrupts/isr.asm

echo "Compiling kernel.c ..."
gcc -m32 -Wall -O -ffreestanding -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Wunused-function -Wpointer-sign -Wunused-function -Wunused-variable -I ./ -I ./include/ -c -o ./build/kernel.out ./kernel/kernel.c
echo "Compiling utilities.c ..."
gcc -m32 -Wall -O -ffreestanding -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Wunused-function -Wpointer-sign -Wunused-function -Wunused-variable -I ./ -I ./include/ -c -o ./build/utilities.out ./kernel/utilities.c
echo "Compiling display.c ..."
gcc -m32 -Wall -O -ffreestanding -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Wunused-function -Wpointer-sign -Wunused-function -Wunused-variable -I ./ -I ./include/ -c -o ./build/display.out ./kernel/drivers/display/display.c
echo "Compiling cursor.c ..."
gcc -m32 -Wall -O -ffreestanding -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Wunused-function -Wpointer-sign -Wunused-function -Wunused-variable -I ./ -I ./include/ -c -o ./build/cursor.out ./kernel/drivers/display/cursor.c
echo "Compiling ports.c ..."
gcc -m32 -Wall -O -ffreestanding -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Wunused-function -Wpointer-sign -Wunused-function -Wunused-variable -I ./ -I ./include/ -c -o ./build/ports.out ./kernel/drivers/io/ports.c
echo "Compiling gdt.c ..."
gcc -m32 -Wall -O -ffreestanding -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Wunused-function -Wpointer-sign -Wunused-function -Wunused-variable -I ./ -I ./include/ -c -o ./build/gdt.out ./kernel/system/gdt/gdt.c
echo "Compiling idt.c ..."
gcc -m32 -Wall -O -ffreestanding -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Wunused-function -Wpointer-sign -Wunused-function -Wunused-variable -I ./ -I ./include/ -c -o ./build/idt.out ./kernel/system/interrupts/idt.c
echo "Compiling pic.c ..."
gcc -m32 -Wall -O -ffreestanding -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Wunused-function -Wpointer-sign -Wunused-function -Wunused-variable -I ./ -I ./include/ -c -o ./build/pic.out ./kernel/system/interrupts/pic.c


echo "Compiling libc ..."
source ./libc/build.sh
echo "Compiled libc ..."

echo "Linking..."
ld -m elf_i386 -T linker.ld -o ./build/kernel.bin ./build/boot.out ./build/kernel.out \
./build/utilities.out ./build/string.out ./build/display.out ./build/cursor.out \
./build/ports.out ./build/gdt.out ./build/gdt_asm.out ./build/isr_asm.out \
./build/idt.out ./build/pic.out -nostdlib

if grub2-file --is-x86-multiboot ./build/kernel.bin; then
  echo "multiboot confirmed"
else
  echo "the file is not multiboot"
fi

cp ./build/kernel.bin ./isodir/boot/kernel.bin
cp ./grub.cfg ./isodir/boot/grub/grub.cfg

grub2-mkrescue -o ./build/image/BlinkOS.iso isodir

#qemu-system-i386 -kernel ./build/kernel.bin

qemu-system-i386 -cdrom ./build/image/BlinkOS.iso