section .text
global _start

_start:
xor  rdx, rdx
idiv r8 

xor  rdx, rdx
mov  r15, rax
mov  rax, r8
idiv r9
mov  r8,  rax
mov  rax, r15

xor  rdx, rdx
mov  r15, rax
mov  rax, r9
idiv r10
mov  r9,  rax
mov  rax, r15

xor  rdx, rdx
mov  r15, rax
mov  rax, r10
idiv r11
mov  r10, rax
mov  rax, r15
 
xor  rdx, rdx
mov  r15, rax
mov  rax, r11
idiv r12
mov  r11, rax
mov  rax, r15

xor  rdx, rdx
mov  r15, rax
mov  rax, r12
idiv r13
mov  r12, rax
mov  rax, r15

xor  rdx, rdx
mov  r15, rax
mov  rax, r13
idiv r14
mov  r13, rax
mov  rax, r15
