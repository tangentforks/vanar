section .text
global _start

_start:

movd mm0, edx
xor  edx, edx
idiv ecx
movd edx, mm0

movd mm0, edx
movd mm1, eax
movd mm2, ebx
mov  eax, ecx
mov  ebx, edx
xor  edx, edx
idiv ebx
mov  ecx, eax
movd ebx, mm2
movd eax, mm1
movd edx, mm0

movd mm0, eax
mov  eax, edx
xor  edx, edx
idiv ebx
mov  edx, eax
movd eax, mm0

movd mm0, eax
mov  eax, ebx
xor  edx, edx
idiv esp
mov  ebx, eax
movd eax, mm0

movd mm0, eax
mov  eax, esp
xor  edx, edx
idiv ebp
mov  esp, eax
movd eax, mm0

movd mm0, eax
mov  eax, ebp
xor  edx, edx
idiv esi
mov  ebp, eax
movd eax, mm0

movd mm0, eax
mov  eax, esi
xor  edx, edx
idiv edi
mov  esi, eax
movd eax, mm0
