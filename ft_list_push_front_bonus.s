section .text
    global ft_list_push_front
    extern malloc
    extern __errno_location

ft_list_push_front:
    push    rdi
    push    rsi
    mov     rdi, 16
    call    malloc wrt ..plt
    pop     rsi
    pop     rdi
    test    rax, rax
    jz      .malloc_failed
    mov     [rax], rsi
    mov     rdx, [rdi]
    mov     [rax + 8], rdx
    mov     [rdi], rax
    mov     rax, [rdi]
    ret

.malloc_failed:
    call    __errno_location wrt ..plt
    mov     dword [rax], 12
    mov     rax, 0
    ret