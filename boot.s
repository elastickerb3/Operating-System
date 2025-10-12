BITS 32

SECTION .multiboot
    dd 0x1BADB002
    dd 0x00000000
    dd 0xE4524FFE 

section .data
rm_regs:    times 0x34 db 0

SECTION .text

global keymap
global shift_keymap

global _start
extern kernel_main

%define KBD_DATA    0x60   
%define KBD_STATUS  0x64 

global get_scancode

global reboot

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
.hang:
    hlt
    jmp .hang
reboot:
    cli
    xor ax, ax
    mov ss, ax
    mov sp, 0x7c00
    sti
    mov ax, 0x5300
    mov bx, 0x0000
    int 0x15
    jc apm_not_present   
    mov ax, 0x5307
    mov bx, 0x0001     
    mov cx, 0x0003      
    int 0x15
    jc apm_failed    

hang:
    hlt
    jmp hang

apm_not_present:
    hlt
    jmp $

apm_failed:
    hlt
    jmp $