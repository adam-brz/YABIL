SECTION .text

global asm_sub_arrays

; extern void asm_sub_arrays(const bigint_base_t *a, std::size_t a_size,
;                            const bigint_base_t *b, std::size_t b_size,
;                            bigint_base_t *r);
;
; Data registers:
;   rdi - a
;   rsi - a_size
;   rdx - b
;   rcx - b_size
;   r8 - r
;
; Assumptions:
; - a_size >= b_size, a_size > 0
; - r - allocated memory, filled with 0 for max(a_size, b_size) elements
;

asm_sub_arrays:
    xor rax, rax ; i = 0
    clc          ; clear carry flag
    pushf        ; push carry flag

    test rcx, rcx ; b_size == 0
    jz L1_end     ; skip first loop

L1:                                 ; for i in range(0, b_size)
    mov r10, qword [rdi + rax*8]    ; tmp = a[i]

    popf                            ; restore carry flag
    sbb r10, qword [rdx + rax*8]    ; tmp -= b[i] + carry
    pushf                           ; preserve carry flag

    mov qword [r8 + rax*8], r10     ; r[i] = tmp

    inc rax      ; ++i
    cmp rax, rcx ; i < b_size
    jb L1

L1_end:
    cmp rax, rsi ; i >= a_size
    jnb L2_end

L2:
    mov r10, qword [rdi + rax*8] ; tmp = a[i]

    popf
    sbb r10, 0 ; tmp -= carry
    pushf

    mov qword [r8 + rax*8], r10  ; r[i] = tmp

    inc rax      ; ++i
    cmp rax, rsi ; i < a_size
    jb L2

L2_end:
    popf
    ret
