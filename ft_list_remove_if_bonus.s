section .text
    global ft_list_remove_if
    extern free

ft_list_remove_if:
    mov     rax, [rdi]
    test    rax, rax
    jz      .exit
    push    rdi
    mov     rdi, [rax]
    push    rsi
    push    rdx
    push    rcx
    call    rdx
    pop     rcx
    pop     rdx
    pop     rsi
    pop     rdi
    cmp     rax, 0
    je      .remove_first
    mov     r8, [rdi]

.traverse_and_compare:
    mov     rax, [r8 + 8]
    test    rax, rax
    jz      .exit
    mov     rdi, [rax]
    push    rsi
    push    rdx
    push    rcx
    push    r8
    call    rdx
    pop     r8
    pop     rcx
    pop     rdx
    pop     rsi
    cmp     eax, 0
    je      .remove_another
    mov     r8, [r8 + 8]
    jmp     .traverse_and_compare

.remove_first:
    mov     rax, [rdi]
    mov     rdx, [rax + 8]
    mov     [rdi], rdx
    jmp     .free

.remove_another:
    mov     rax, [r8 + 8]
    mov     rdx, [rax + 8]
    mov     [r8 + 8], rdx
    jmp     .free

.free:
    mov     rdi, rax
    push    qword [rdi]
    push    rcx
    call    free wrt ..plt
    pop     rcx
    pop     rdi
    call    rcx

.exit:
    ret