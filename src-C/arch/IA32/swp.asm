section .text
global _start

_start:
xchg ecx, eax
xchg edx, ecx
xchg ebx, edx
xchg esp, ebx
xchg ebp, esp
xchg esi, ebp
xchg edi, esi
