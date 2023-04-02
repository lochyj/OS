gcc -c gen_initrd.c -o gen_initrd.o

gcc -o gen_init ./gen_initrd.o -lc
