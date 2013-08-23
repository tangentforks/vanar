section .text
global _start

_start:
adc rax, 0x01020304
adc r8,  0x01020304
adc r9,  0x01020304
adc r10, 0x01020304
adc r11, 0x01020304
adc r12, 0x01020304
adc r13, 0x01020304
adc r14, 0x01020304