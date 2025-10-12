BITS 32

section .multiboot

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
    in al, 0x64           
    test al, 1
    jz .wait

    in al, 0x60          
    movzx eax, al         
    ret

_start:
    cli
    call kernel_main