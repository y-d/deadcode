org 0x7C00

section .text

%macro sleep 1
    push dx
    mov ah, 86h
    mov cx, %1
    mov dx, 0
    int 0x15
    pop dx
%endmacro

start:
    mov si, msg

print_loop:
    mov al, [si]
    cmp al, 0
    je done
    mov ah, 0x0E
    mov bh, 0x00
    mov bl, 0x07
    int 0x10
    inc si
    sleep 2
    jmp print_loop

done:
    sleep 5
    
    mov ah, 0x06
    mov al, 0
    mov bh, 0x07
    mov cx, 0x0000
    mov dx, 0x184F
    int 0x10

    mov ah, 0x01
    mov bh, 0x00
    mov ch, 0x20
    mov cl, 0x20
    int 0x10

hang:
    jmp hang

msg db "You weren't so smart to restart.", 0x0D, 0x0A
    db "Enjoy the black screen (:", 0x0D, 0x0A

times 510 - ($ - $$) db 0
dw 0xAA55