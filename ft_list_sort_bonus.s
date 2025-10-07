section .text
    global ft_list_sort

ft_list_sort:
.bubble_sort_start:
    mov     r9, 0
    mov     rax, [rdi]
    test    rax, rax
    jz      .exit
    mov     rcx, [rax + 8]
    test    rcx, rcx
    jz      .exit
    push    rdi
    mov     rdi, [rax]
    push    rsi
    mov     rsi, [rcx]
    push    r9
    call    [rsp + 8]
    pop     r9
    pop     rsi
    pop     rdi
    cmp     eax, 0
    jg      .swap_first_two
    mov     rax, [rdi]

.traverse_and_compare:
    mov     rcx, [rax + 8]
    mov     rdx, [rcx + 8]
    test    rdx, rdx
    jz      .check_if_done
    push    rax
    push    rdi
    mov     rdi, [rcx]
    push    rsi
    mov     rsi, [rdx]
    push    r9
    call    [rsp + 8]
    pop     r9
    pop     rsi
    pop     rdi
    cmp     eax, 0
    jg      .swap_others
    pop     rax
    mov     rax, [rax + 8]
    jmp     .traverse_and_compare

.check_if_done:
    cmp     r9, 1
    je      .bubble_sort_start
    ret

.swap_first_two:
    mov     r9, 1
    mov     rax, [rdi]
    mov     rcx, [rax + 8]
    mov     rdx, [rcx + 8]
    mov     [rax + 8], rdx
    mov     [rcx + 8], rax
    mov     [rdi], rcx
    mov     rax, [rdi]
    jmp     .traverse_and_compare

.swap_others:
    mov     r9, 1
    pop     rax
    mov     rcx, [rax + 8]
    mov     rdx, [rcx + 8]
    mov     r8, [rdx + 8]
    mov     [rax + 8], rdx
    mov     [rdx + 8], rcx
    mov     [rcx + 8], r8
    mov     rax, rdx
    jmp     .traverse_and_compare

.exit:
    ret
