section .text
global _start

_start:
    jnz e1
    jmp rax

e1: jnz e2
    jmp r8

e2: jnz e3
    jmp r9

e3: jnz e4
    jmp r10

e4: jnz e5
    jmp r11

e5: jnz e6
    jmp r12

e6: jnz e7
    jmp r13

e7: jnz e8
    jmp r14
e8:
