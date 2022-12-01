global _start
[bits 32]

_start:
    [extern kmain]
    ;   int 0x13 ; TODO <--
    call kmain
    jmp $