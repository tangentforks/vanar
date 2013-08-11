section .text
global _start

_start:
and eax, 0x01020304
and ecx, 0x01020304
and edx, 0x01020304
and ebx, 0x01020304
and esp, 0x01020304
and ebp, 0x01020304
and esi, 0x01020304
and edi, 0x01020304