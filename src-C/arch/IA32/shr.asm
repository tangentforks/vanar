section .text
global _start

_start:
shr  eax, cl     ; eax shl ecx 

xchg ecx, edx    ; ecx shl edx
shr  edx, cl
xchg ecx, edx

xchg ecx, ebx    ; edx shl ebx
shr  edx, cl
xchg ecx, ebx

xchg ecx, esp    ; ebx shl esp
shr  ebx, cl
xchg ecx, esp

xchg ecx, ebp    ; esp shl ebp
shr  esp, cl
xchg ecx, ebp

xchg ecx, esi    ; ebp shl esi
shr  ebp, cl
xchg ecx, esp

xchg ecx, edi    ; esi shl edi
shr  esi, cl
xchg ecx, edi
