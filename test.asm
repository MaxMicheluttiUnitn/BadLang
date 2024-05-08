section .text
    global _start

_start:

    ; exit
    mov rdi, rbx
    mov rax, 60
    syscall 