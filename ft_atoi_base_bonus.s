section .text
    global ft_atoi_base

ft_control_and_strlen:
    mov     rax, rdi

.loop:
    mov     cl, [rax]
    cmp     cl, 0
    je      .done
    cmp     cl, '+'
    je      .done_with_error
    cmp     cl, '-'
    je      .done_with_error
    cmp     cl, ' '
    je      .done_with_error
    cmp     cl, 9
    jl      .control_dup
    cmp     cl, 13
    jg      .control_dup
    jmp     .done_with_error

.control_dup:
    mov     rdx, rdi

.loop2:
    cmp     rdx, rax
    je      .skip
    cmp     [rdx], cl
    je      .done_with_error
    inc     rdx
    jmp     .loop2  

.skip:
    inc     rax
    jmp     .loop

.done:
    sub     rax, rdi
    ret

.done_with_error:
    mov     rax, -1
    ret

ft_atoi_base:
    push    rdi
    mov     rdi, rsi
    call    ft_control_and_strlen
    mov     rcx, rax
    pop     rdi
    cmp     rcx, 2
    jl      .invalid_base
    jmp     .skip_whitespace

.invalid_base:
    mov     rax, 0
    ret

.skip_whitespace:
    mov     dl, [rdi]
    cmp     dl, ' '
    je      .advance_whitespace
    cmp     dl, 9
    jl      .check_sign
    cmp     dl, 13
    jg      .check_sign

.advance_whitespace:
    inc     rdi
    jmp     .skip_whitespace

.check_sign:
    cmp     dl, '-'
    je      .negative_sign
    cmp     dl, '+'
    je      .positive_sign
    push    1
    jmp     .start_conversion

.negative_sign:
    inc     rdi
    push    -1
    jmp     .start_conversion

.positive_sign:
    inc     rdi
    push    1

.start_conversion:
    mov     rax, 0

.convert_loop:
    mov     dl, [rdi]
    test    dl, dl
    jz      .apply_sign_and_return
    movzx   r10, dl
    jmp     .lookup_base_index

.base_match:
    mul     rcx
    add     rax, r11
    inc     rdi
    jmp     .convert_loop

.lookup_base_index:
    push    rax
    mov     rax, rsi

.base_loop:
    cmp     byte [rax], 0
    je      .not_in_base
    cmp     [rax], dl
    je      .found_base_char
    inc     rax
    jmp     .base_loop

.found_base_char:
    sub     rax, rsi
    mov     r11, rax
    pop     rax
    jmp     .base_match

.not_in_base:
    pop     rax

.apply_sign_and_return:
    pop     rcx
    mul     rcx
    ret
