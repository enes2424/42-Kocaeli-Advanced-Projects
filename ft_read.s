section .text
    global ft_read
    extern __errno_location

ft_read:
    mov     rax, 0
    syscall
    cmp     rax, 0
    jl      .read_failed
    ret

.read_failed:
    push    rax
    call    __errno_location wrt ..plt
    pop     rcx
    neg     rcx
    mov     [rax], ecx
    mov     rax, -1
    ret
