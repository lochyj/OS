nasm -f elf32 -o ./build/boot.out ./assemblies/boot.asm

nasm -f elf32 -o ./build/pagingAsm.out ./assemblies/paging.asm

echo "Compiling kernel.c ..."
gcc -m32 -Wall -O -ffreestanding -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Wunused-function -Wpointer-sign -Wunused-function -Wunused-variable -I . -I ./include/ -c -o ./build/kernel.out ./kernel/kernel.c
echo "Compiling utilities.c ..."
gcc -m32 -Wall -O -ffreestanding -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Wunused-function -Wpointer-sign -Wunused-function -Wunused-variable -I . -I ./include/ -c -o ./build/utilities.out ./kernel/utilities.c
echo "Compiling display.c ..."
gcc -m32 -Wall -O -ffreestanding -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Wunused-function -Wpointer-sign -Wunused-function -Wunused-variable -I . -I ./include/ -c -o ./build/display.out ./kernel/drivers/display/display.c
echo "Compiling gdt.c ..."
gcc -m32 -Wall -O -ffreestanding -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Wunused-function -Wpointer-sign -Wunused-function -Wunused-variable -I . -I ./include/ -c -o ./build/gdt.out ./kernel/system/gdt/gdt.c
echo "Compiling idt.c ..."
gcc -m32 -Wall -O -ffreestanding -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Wunused-function -Wpointer-sign -Wunused-function -Wunused-variable -I . -I ./include/ -c -o ./build/idt.out ./kernel/system/interrupts/idt.c
echo "Compiling isr.c ..."
gcc -m32 -Wall -O -ffreestanding -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Wunused-function -Wpointer-sign -Wunused-function -Wunused-variable -I . -I ./include/ -c -o ./build/isr.out ./kernel/system/interrupts/isr.c
echo "Compiling irq.c ..."
gcc -m32 -Wall -O -ffreestanding -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Wunused-function -Wpointer-sign -Wunused-function -Wunused-variable -I . -I ./include/ -c -o ./build/irq.out ./kernel/system/interrupts/irq.c
echo "Compiling clock.c ..."
gcc -m32 -Wall -O -ffreestanding -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Wunused-function -Wpointer-sign -Wunused-function -Wunused-variable -I . -I ./include/ -c -o ./build/clock.out ./kernel/system/clock/clock.c
echo "Compiling keyboard.c ..."
gcc -m32 -Wall -O -ffreestanding -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Wunused-function -Wpointer-sign -Wunused-function -Wunused-variable -I . -I ./include/ -c -o ./build/keyboard.out ./kernel/drivers/keyboard/keyboard.c
echo "Compiling debug.c ..."
gcc -m32 -Wall -O -ffreestanding -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Wunused-function -Wpointer-sign -Wunused-function -Wunused-variable -I . -I ./include/ -c -o ./build/debug.out ./kernel/system/debug/debug.c
echo "Compiling kprintf.c ..."
gcc -m32 -Wall -O -ffreestanding -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Wunused-function -Wpointer-sign -Wunused-function -Wunused-variable -I . -I ./include/ -c -o ./build/kprintf.out ./kernel/utilities/kprintf.c
echo "Compiling ordered_array.c ..."
gcc -m32 -Wall -O -ffreestanding -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Wunused-function -Wpointer-sign -Wunused-function -Wunused-variable -I . -I ./include/ -c -o ./build/ordered_array.out ./kernel/utilities/ordered_array.c
echo "Compiling kmalloc.c ..."
gcc -m32 -Wall -O -ffreestanding -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Wunused-function -Wpointer-sign -Wunused-function -Wunused-variable -I . -I ./include/ -c -o ./build/kmalloc.out ./kernel/system/vmem/kmalloc.c
echo "Compiling page.c ..."
#gcc -m32 -Wall -O -ffreestanding -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Wunused-function -Wpointer-sign -Wunused-function -Wunused-variable -I . -I ./include/ -c -o ./build/page.out ./kernel/system/vmem/page.c
echo "Compiling heap.c ..."
#gcc -m32 -Wall -O -ffreestanding -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Wunused-function -Wpointer-sign -Wunused-function -Wunused-variable -I . -I ./include/ -c -o ./build/heap.out ./kernel/system/vmem/heap.c

echo "Linking..."
ld -m elf_i386 -T linker.ld -o ./build/kernel.bin ./build/boot.out ./build/kernel.out ./build/display.out ./build/utilities.out ./build/gdt.out ./build/idt.out ./build/isr.out ./build/irq.out ./build/clock.out ./build/keyboard.out ./build/debug.out ./build/kprintf.out ./build/ordered_array.out ./build/kmalloc.out -nostdlib

if grub2-file --is-x86-multiboot ./build/kernel.bin; then
  echo multiboot confirmed
else
  echo the file is not multiboot
fi

cp ./build/kernel.bin ./isodir/boot/kernel.bin
cp ./grub.cfg ./isodir/boot/grub/grub.cfg

grub2-mkrescue -o ./build/image/BlinkOS.iso isodir

#qemu-system-i386 -kernel ./build/kernel.bin

qemu-system-i386 -cdrom ./build/image/BlinkOS.iso