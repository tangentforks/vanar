section .text
global _start

_start:
sbb rax, 0x01020304
sbb r8,  0x01020304
sbb r9,  0x01020304
sbb r10, 0x01020304
sbb r11, 0x01020304
sbb r12, 0x01020304
sbb r13, 0x01020304
sbb r14, 0x01020304