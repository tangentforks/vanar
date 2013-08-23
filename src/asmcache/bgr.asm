section .text
global _start

_start:
    jng e1
    jmp rax

e1: jng e2
    jmp r8

e2: jng e3
    jmp r9

e3: jng e4
    jmp r10

e4: jng e5
    jmp r11

e5: jng e6
    jmp r12

e6: jng e7
    jmp r13

e7: jng e8
    jmp r14
e8:
