section .text
global _start

_start:
mov cl,  r8b
shl rax, cl 
mov cl,  r9b
shl r8,  cl
mov cl,  r10b
shl r9,  cl
mov cl,  r11b 
shl r10, cl
mov cl,  r12b
shl r11, cl
mov cl,  r13b  
shl r12, cl
mov cl,  r14b 
shl r13, cl
 