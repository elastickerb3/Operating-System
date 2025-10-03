BITS 32

SECTION .multiboot
    dd 0x1BADB002
    dd 0x00000000
    dd 0xE4524FFE 

section .data
keymap:
    db 0,0,'1','2','3','4','5','6','7','8','9','0',0,0,'D',0     
    db 'q','w','e','r','t','z','u','i','o','p','ü','E' ,0,'a','s','d'
    db 'f','g','h','j','k','l','ö','ä',0,'y','x','c','v','b','n','m'
    db 0, 0, 0, 0, 0, 0, 'S', 0, 0, 0, 0, 0, 0, 0, 0, 0
    times 256-64 db 0
rm_regs:    times 0x34 db 0

SECTION .text

global _start
extern kernel_main
%define KBD_DATA    0x60   
%define KBD_STATUS  0x64 

global get_key

global reboot

get_key:
.wait:
    in al, KBD_STATUS
    test al, 1
    jz .wait
    in al, KBD_DATA
    cmp al, 0x80    
    jae .not_letter
    movzx eax, al
    mov al, [keymap + eax]
    ret

.not_letter:
    xor al, al
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