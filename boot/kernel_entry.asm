[bits 32]
;_start:

; Including the interrupt asm code to be accessed by the kernel later
%include "./kernel/cpu/interrupt.asm"
[extern main]
call main
jmp $