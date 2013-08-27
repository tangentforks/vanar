section .text
global _start

_start:
    jnl e1
    jmp rax

e1: jnl e2
    jmp r8

e2: jnl e3
    jmp r9

e3: jnl e4
    jmp r10

e4: jnl e5
    jmp r11

e5: jnl e6
    jmp r12

e6: jnl e7
    jmp r13

e7: jnl e8
    jmp r14
e8:
