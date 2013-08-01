section .text
global _start

_start:
cmp rax, 0x01020304
cmp r8,  0x01020304
cmp r9,  0x01020304
cmp r10, 0x01020304
cmp r11, 0x01020304
cmp r12, 0x01020304
cmp r13, 0x01020304
cmp r14, 0x01020304