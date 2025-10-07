section .text
    global ft_strcpy

ft_strcpy:
    mov     rax, rdi

.loop:
    mov     dl, [rsi]
    test    dl, dl
    jz      .done
    mov     [rdi], dl
    inc     rsi
    inc     rdi
    jmp     .loop

.done:
    mov     byte [rdi], 0
    ret