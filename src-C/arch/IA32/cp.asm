section .text
global _start

_start:
cmp eax, ecx 
cmp ecx, edx
cmp edx, ebx 
cmp ebx, esp
cmp esp, ebp  
cmp ebp, esi 
cmp esi, edi
 