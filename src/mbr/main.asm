org 0x7C00

%macro sleep 1
    ; credits: memz
    push dx
    mov ah, 86h
    mov cx, %1
    mov dx, 0
    int 15h
    pop dx
%endmacro

start:
    mov ah, 0x00
    mov al, 0x13
    int 0x10

    mov ax, 0xA000
    mov es, ax

    xor bh, bh    ; bh = 0
    xor bl, bl    ; bl = 0

main_loop:
    mov di, 0
    mov cx, 320*200
fill_screen:
    mov al, bl
    mov es:[di], al
    inc di
    loop fill_screen

    inc bl
    cmp bl, 25
    jl continue_fill
    xor bl, bl       ; bl = 0

continue_fill:
    inc bh
    cmp bh, 60      ; near eh 10 seconds
    je randomloop

    sleep 1
    jmp main_loop

randomloop:
    mov si, 0        ; y coordinate
    mov bx, 0        ; x coordinate
    mov al, 1        ; starting color
    xor eax, eax     ; eax = 0

random_pixel_loop:
    inc eax
    cmp eax, 320*200*60
    je main_loop

    mov di, si
    shl di, 8        ; di = y * 256
    mov dx, si
    shl dx, 6        ; dx = y * 64
    add di, dx       ; di = y*256 + y*64
    add di, bx       ; add x

    mov [es:di], al

    inc bx
    cmp bx, 319
    jl random_pixel_loop
    xor bx, bx
    inc si
    cmp si, 200
    jl random_pixel_loop
    jmp end_random

end_random:
    inc al
    cmp al, 24
    jl continue_color
    mov al, 1

continue_color:
    jmp random_pixel_loop

times 510 - ($ - $$) db 0
dw 0xAA55