nasm -f elf32 -o ./build/boot.out ./kernel/boot.asm

gcc -m32 -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Wpointer-sign -I . -I ./include/ -c -o ./build/kernel.out ./kernel/kernel.c

gcc -m32 -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Wpointer-sign -I . -I ./include/  -c -o ./build/utilities.out ./kernel/utilities.c

gcc -m32 -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Wpointer-sign -I . -I ./include/  -c -o ./build/display.out ./kernel/display.c

gcc -m32 -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Wpointer-sign -I . -I ./include/  -c -o ./build/gdt.out ./kernel/gdt.c

gcc -m32 -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Wpointer-sign -I . -I ./include/  -c -o ./build/idt.out ./kernel/idt.c

gcc -m32 -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Wpointer-sign -I . -I ./include/  -c -o ./build/isr.out ./kernel/isr.c

gcc -m32 -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Wpointer-sign -I . -I ./include/  -c -o ./build/irq.out ./kernel/irq.c

gcc -m32 -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Wpointer-sign -I . -I ./include/  -c -o ./build/clock.out ./kernel/clock.c

gcc -m32 -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Wpointer-sign -I . -I ./include/  -c -o ./build/keyboard.out ./kernel/keyboard.c


ld -m elf_i386 -T linker.ld -o ./build/kernel.bin ./build/boot.out ./build/kernel.out ./build/display.out ./build/utilities.out ./build/gdt.out ./build/idt.out ./build/isr.out ./build/irq.out ./build/clock.out ./build/keyboard.out -nostdlib

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