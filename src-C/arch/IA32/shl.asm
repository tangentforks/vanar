section .text
global _start

_start:
shl  eax, cl     ; eax shl ecx 

xchg ecx, edx    ; ecx shl edx
shl  edx, cl
xchg ecx, edx

xchg ecx, ebx    ; edx shl ebx
shl  edx, cl
xchg ecx, ebx

xchg ecx, esp    ; ebx shl esp
shl  ebx, cl
xchg ecx, esp

xchg ecx, ebp    ; esp shl ebp
shl  esp, cl
xchg ecx, ebp

xchg ecx, esi    ; ebp shl esi
shl  ebp, cl
xchg ecx, esp

xchg ecx, edi    ; esi shl edi
shl  esi, cl
xchg ecx, edi
