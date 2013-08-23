section .text
global _start

_start:
    jne e1
    jmp rax

e1: jne e2
    jmp r8

e2: jne e3
    jmp r9

e3: jne e4
    jmp r10

e4: jne e5
    jmp r11

e5: jne e6
    jmp r12

e6: jne e7
    jmp r13

e7: jne e8
    jmp r14
e8:
