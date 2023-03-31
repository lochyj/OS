nasm -f elf32 -o ./build/boot.out ./assemblies/boot.asm

gcc -m32 -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Wunused-function -Wpointer-sign -I . -I ./include/ -c -o ./build/kernel.out ./kernel/kernel.c

gcc -m32 -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Wunused-function -Wpointer-sign -I . -I ./include/ -c -o ./build/utilities.out ./kernel/utilities.c

gcc -m32 -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Wunused-function -Wpointer-sign -I . -I ./include/ -c -o ./build/display.out ./kernel/drivers/display/display.c

gcc -m32 -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Wunused-function -Wpointer-sign -I . -I ./include/ -c -o ./build/gdt.out ./kernel/system/gdt/gdt.c

gcc -m32 -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Wunused-function -Wpointer-sign -I . -I ./include/ -c -o ./build/idt.out ./kernel/system/interrupts/idt.c

gcc -m32 -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Wunused-function -Wpointer-sign -I . -I ./include/ -c -o ./build/isr.out ./kernel/system/interrupts/isr.c

gcc -m32 -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Wunused-function -Wpointer-sign -I . -I ./include/ -c -o ./build/irq.out ./kernel/system/interrupts/irq.c

gcc -m32 -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Wunused-function -Wpointer-sign -I . -I ./include/ -c -o ./build/clock.out ./kernel/system/clock/clock.c

gcc -m32 -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Wunused-function -Wpointer-sign -I . -I ./include/ -c -o ./build/keyboard.out ./kernel/drivers/keyboard/keyboard.c

gcc -m32 -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Wunused-function -Wpointer-sign -I . -I ./include/ -c -o ./build/debug.out ./kernel/system/debug/debug.c

gcc -m32 -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Wunused-function -Wpointer-sign -I . -I ./include/ -c -o ./build/kprintf.out ./kernel/utilities/kprintf.c

ld -m elf_i386 -T linker.ld -o ./build/kernel.bin ./build/boot.out ./build/kernel.out ./build/display.out ./build/utilities.out ./build/gdt.out ./build/idt.out ./build/isr.out ./build/irq.out ./build/clock.out ./build/keyboard.out ./build/debug.out ./build/kprintf.out -nostdlib

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