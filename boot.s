BITS 32

SECTION .multiboot
    dd 0x1BADB002
    dd 0x00000000
    dd 0xE4524FFE 

section .data
keymap:
    db 0,0,'1','2','3','4','5','6','7','8','9','0',0,0,'D',0     ; 0x00–0x0F
    db 'q','w','e','r','t','z','u','i','o','p','ü','E' ,0,'a','s','d' ; 0x10–0x1F
    db 'f','g','h','j','k','l','ö','ä',0,'y','x','c','v','b','n','m'   ; 0x20–0x2F
    db 0, 0, 0, 0, 0, 0, 'S', 0, 0, 0, 0, 0, 0, 0, 0, 0
    times 256-64 db 0

SECTION .text

global _start
extern kernel_main
%define KBD_DATA    0x60        ; Datenport für Tastatur
%define KBD_STATUS  0x64        ; Statusregister der Tastatur

global get_key

global shutdown

get_key:
.wait:
    in al, KBD_STATUS
    test al, 1
    jz .wait
    in al, KBD_DATA
    cmp al, 0x80          ; Key Release ignorieren
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
shutdown:
    mov dx, 0x404        ; PM1a Control Register (Beispieladresse!)
    mov ax, (5 << 10) | (1 << 13) ; SLP_TYP + SLP_EN
    out dx, ax
