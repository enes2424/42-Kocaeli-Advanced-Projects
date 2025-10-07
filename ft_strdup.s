section .text
    global ft_strdup
    extern malloc
    extern __errno_location
    extern ft_strlen
    extern ft_strcpy

ft_strdup:
    push    rdi
    call    ft_strlen
    mov     rdi, rax
    inc     rdi
    call    malloc wrt ..plt
    pop     rsi
    test    rax, rax
    jz      .malloc_failed
    mov     rdi, rax
    call    ft_strcpy
    ret

.malloc_failed:
    call    __errno_location wrt ..plt
    mov     dword [rax], 12
    mov     rax, 0
    ret
