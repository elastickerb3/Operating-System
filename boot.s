BITS 32

SECTION .multiboot
    dd 0x1BADB002
    dd 0x00000000
    dd 0xE4524FFE 

section .data
rm_regs:    times 0x34 db 0

SECTION .text
global _start
extern kernel_main

%define KBD_DATA    0x60   
%define KBD_STATUS  0x64 
global get_scancode

get_scancode:
.wait:
    in al, 0x64           ; KBD_STATUS
    test al, 1
    jz .wait

    in al, 0x60           ; KBD_DATA
    movzx eax, al         ; Null-extend AL nach EAX
    ret

_start:
    cli

    mov esp, 0x90000
    call kernel_main