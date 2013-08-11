section .text
global _start

_start:
imul eax, ecx 
imul ecx, edx
imul edx, ebx 
imul ebx, esp
imul esp, ebp  
imul ebp, esi 
imul esi, edi
 