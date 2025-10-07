section .text
    global ft_write
    extern __errno_location

ft_write:
    mov     rax, 1
    syscall
    cmp     rax, 0
    jl      .write_failed
    ret

.write_failed:
    push    rax
    call    __errno_location wrt ..plt
    pop     rcx
    neg     rcx
    mov     [rax], ecx
    mov     rax, -1
    ret
