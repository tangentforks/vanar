section .text
global _start

_start:
mov [ecx], eax
mov [edx], ecx
mov [ebx], edx
mov [edx], esp
mov [esp], ebp
mov [ebp], esi
mov [esi], edi