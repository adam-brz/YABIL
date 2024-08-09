SECTION .text

global asm_add_arrays

; extern void asm_add_arrays(const bigint_base_t *a, std::size_t a_size,
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
; - r - allocated memory, filled with 0 for max(a_size, b_size) + 1 elements
;

asm_add_arrays:
    xor rax, rax ; i = 0
    clc          ; clear carry flag
    pushf        ; push carry flag

    test rcx, rcx ; b_size == 0
    jz L1_end     ; skip first loop

L1:                      ; for i in range(0, b_size)
    mov r10, qword [rdi] ; tmp = a[i]
    lea rdi, [rdi + 8]

    popf                 ; restore carry flag
    adc r10, qword [rdx] ; tmp += b[i] + carry
    pushf                ; preserve carry flag
    lea rdx, [rdx + 8]

    mov qword [r8], r10  ; r[i] = tmp
    lea r8, [r8 + 8]

    inc rax      ; ++i
    cmp rax, rcx ; i < b_size
    jb L1

L1_end:
    cmp rax, rsi ; i >= a_size
    jnb L2_end

L2:
    xor r10, r10 ; tmp = 0
    popf
    adc r10, qword [rdi] ; tmp += a[i] + carry
    pushf
    lea rdi, [rdi + 8]

    mov qword [r8], r10  ; r[i] = tmp
    lea r8, [r8 + 8]

    inc rax      ; ++i
    cmp rax, rsi ; i < a_size
    jb L2

L2_end:
    popf
    adc qword [r8], 0  ; r[i] += carry

    ret
