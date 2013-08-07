section .text
global _start

_start:
mov eax, [ecx]
mov ecx, [edx]
mov edx, [ebx]
mov ebx, [esp]
mov esp, [ebp]
mov ebp, [esi]
mov esi, [edi]