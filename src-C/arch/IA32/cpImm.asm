section .text
global _start

_start:
cmp eax, 0x01020304
cmp ecx, 0x01020304
cmp edx, 0x01020304
cmp ebx, 0x01020304
cmp esp, 0x01020304
cmp ebp, 0x01020304
cmp esi, 0x01020304
cmp edi, 0x01020304