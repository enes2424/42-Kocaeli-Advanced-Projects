section .text
    global ft_strlen

ft_strlen:
    mov     rax, rdi

.loop:
    cmp     byte [rax], 0
    je      .done
    inc     rax
    jmp     .loop

.done:
    sub     rax, rdi
    ret
