/* Generated by Nimrod Compiler v0.9.2 */
/*   (c) 2012 Andreas Rumpf */
/* The generated code is subject to the original license. */
/* Compiled for: Linux, amd64, gcc */
/* Command for C compiler:
   gcc -c  -w  -I/home/claude/Projekte/Nimrod/lib -o /home/claude/Projekte/vanar/src/nimcache/shmBuffer.o /home/claude/Projekte/vanar/src/nimcache/shmBuffer.c */
#define NIM_INTBITS 64
#include "nimbase.h"
typedef struct TNimType TNimType;
typedef struct TNimNode TNimNode;
typedef struct tshmbuffer91012 tshmbuffer91012;
typedef struct TNimObject TNimObject;
typedef N_NIMCALL_PTR(void, TY891) (void* p, NI op);
struct TNimType {
NI size;
NU8 kind;
NU8 flags;
TNimType* base;
TNimNode* node;
void* finalizer;
TY891 marker;
};
struct TNimNode {
NU8 kind;
NI offset;
TNimType* typ;
NCSTRING name;
NI len;
TNimNode** sons;
};
struct TNimObject {
TNimType* m_type;
};
struct tshmbuffer91012 {
  TNimObject Sup;
NU8* Pbuffer;
NI Cbuffer;
NIM_BOOL Finit;
};
static N_INLINE(void, nimFrame)(TFrame* s);
static N_INLINE(void, popFrame)(void);
extern TNimType NTI1009; /* TObject */
TNimType NTI91012; /* tShmBuffer */
extern TNimType NTI91018; /* ptr uint8 */
extern TNimType NTI105; /* int */
extern TNimType NTI132; /* bool */
extern TFrame* frameptr_11625;

static N_INLINE(void, nimFrame)(TFrame* s) {
	(*s).prev = frameptr_11625;
	frameptr_11625 = s;
}

static N_INLINE(void, popFrame)(void) {
	frameptr_11625 = (*frameptr_11625).prev;
}
N_NOINLINE(void, shmBufferInit)(void) {
	nimfr("shmBuffer", "shmBuffer.nim")
	popFrame();
}

N_NOINLINE(void, shmBufferDatInit)(void) {
static TNimNode* TMP869[3];
static TNimNode TMP245[4];
NTI91012.size = sizeof(tshmbuffer91012);
NTI91012.kind = 17;
NTI91012.base = (&NTI1009);
NTI91012.flags = 3;
TMP869[0] = &TMP245[1];
TMP245[1].kind = 1;
TMP245[1].offset = offsetof(tshmbuffer91012, Pbuffer);
TMP245[1].typ = (&NTI91018);
TMP245[1].name = "pBuffer";
TMP869[1] = &TMP245[2];
TMP245[2].kind = 1;
TMP245[2].offset = offsetof(tshmbuffer91012, Cbuffer);
TMP245[2].typ = (&NTI105);
TMP245[2].name = "cBuffer";
TMP869[2] = &TMP245[3];
TMP245[3].kind = 1;
TMP245[3].offset = offsetof(tshmbuffer91012, Finit);
TMP245[3].typ = (&NTI132);
TMP245[3].name = "fInit";
TMP245[0].len = 3; TMP245[0].kind = 2; TMP245[0].sons = &TMP869[0];
NTI91012.node = &TMP245[0];
}
