section .text
global _start

_start:
mov cl,  r8b
shr rax, cl 
mov cl,  r9b
shr r8,  cl
mov cl,  r10b
shr r9,  cl
mov cl,  r11b 
shr r10, cl
mov cl,  r12b
shr r11, cl
mov cl,  r13b  
shr r12, cl
mov cl,  r14b 
shr r13, cl
 