section .text
    global ft_list_size

ft_list_size:
    mov     rax, 0
.loop:
    cmp     rdi, 0
    je     .done
    inc     rax
    mov     rdi, [rdi + 8]
    jmp     .loop
.done:
    ret