section .text
global _start

_start:
and eax, ecx 
and ecx, edx
and edx, ebx 
and ebx, esp
and esp, ebp  
and ebp, esi 
and esi, edi
 