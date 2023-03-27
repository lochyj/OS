clear

mkdir ./build/
mkdir ./build/image/
mkdir ./build/isodir/boot/grub/

echo "---- Building ASM ----"

nasm -felf32 ./boot/boot.asm -o ./build/boot.out

nasm -f elf32 ./assemblies/interrupts.asm -o ./build/kernel_asm.out

echo "---- Compiling C ----"

gcc -m32 -g -O2 -fno-pie -ffreestanding -fno-stack-protector -I ./include/ -I ./ -c ./kernel/kernel.c -o ./build/kernel.out

echo "---- Linking output files ----"

ld -m elf_i386 -T ./linker.ld -o ./build/kernel.bin ./build/boot.out ./build/kernel.out ./build/kernel_asm.out -nostdlib

echo "-------- x86 check --------"

if grub2-file --is-x86-multiboot ./build/kernel.bin; then
  echo "Kernel binary is multiboot."
else
  echo "Kernel binary is not multiboot - check above for errors."
fi

echo "-------- copy --------"

cp ./build/kernel.bin ./build/isodir/boot/kernel.bin
cp ./grub.cfg ./build/isodir/boot/grub/grub.cfg

echo "-------- iso build --------"

grub2-mkrescue -o ./build/image/BlinkOS.iso ./build/isodir

echo "---- Running in QEMU ----"

qemu-system-i386 -cdrom ./build/image/BlinkOS.iso -monitor stdio