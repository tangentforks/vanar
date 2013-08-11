section .text
global _start

_start:
xor eax, ecx 
xor ecx, edx
xor edx, ebx 
xor ebx, esp
xor esp, ebp  
xor ebp, esi 
xor esi, edi
 