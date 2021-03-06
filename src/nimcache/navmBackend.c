/* Generated by Nimrod Compiler v0.9.2 */
/*   (c) 2012 Andreas Rumpf */
/* The generated code is subject to the original license. */
/* Compiled for: Linux, amd64, gcc */
/* Command for C compiler:
   gcc -c  -w  -I/home/claude/Projekte/Nimrod/lib -o /home/claude/Projekte/vanar/src/nimcache/navmBackend.o /home/claude/Projekte/vanar/src/nimcache/navmBackend.c */
#define NIM_INTBITS 64
#include "nimbase.h"

#include <sys/mman.h>

#include <stdio.h>

#include <stdlib.h>
typedef struct tnavmbackend91004 tnavmbackend91004;
typedef struct TNimObject TNimObject;
typedef struct TNimType TNimType;
typedef struct TNimNode TNimNode;
typedef struct tshmbuffer91012 tshmbuffer91012;
typedef struct NimStringDesc NimStringDesc;
typedef struct TGenericSeq TGenericSeq;
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
struct TNimObject {
TNimType* m_type;
};
struct tshmbuffer91012 {
  TNimObject Sup;
NU8* Pbuffer;
NI Cbuffer;
NIM_BOOL Finit;
};
struct tnavmbackend91004 {
  TNimObject Sup;
NI8 Vd;
NI8 Vr;
tshmbuffer91012 Tcodemem;
NI Ocodemem;
NIM_BOOL Finit;
};
struct TGenericSeq {
NI len;
NI reserved;
};
typedef NIM_CHAR TY611[100000001];
struct NimStringDesc {
  TGenericSeq Sup;
TY611 data;
};
struct TNimNode {
NU8 kind;
NI offset;
TNimType* typ;
NCSTRING name;
NI len;
TNimNode** sons;
};
typedef NimStringDesc* TY94095[1];
N_NIMCALL(void, init_91029)(tnavmbackend91004* ob, NI memsize);
N_NIMCALL(void, hiddenraiseassert_71220)(NimStringDesc* msg);
static N_INLINE(void, nimFrame)(TFrame* s);
static N_INLINE(void, popFrame)(void);
N_NIMCALL(void, release_91253)(tnavmbackend91004* ob);
N_NIMCALL(void, compile_91628)(tnavmbackend91004* ob, NU8 val);
static N_INLINE(NI, addInt)(NI a, NI b);
N_NOINLINE(void, raiseOverflow)(void);
N_NIMCALL(void, compile2_92028)(tnavmbackend91004* ob, NU16 val, NIM_BOOL bigendian);
N_NIMCALL(void, compile4_92308)(tnavmbackend91004* ob, NU32 val, NIM_BOOL bigendian);
N_NIMCALL(void, compile8_92620)(tnavmbackend91004* ob, NU64 val, NIM_BOOL bigendian);
N_NIMCALL(void, shminitexec_91207)(tshmbuffer91012* ob, NI memsize);
N_NIMCALL(void*, valloc_91214)(NI memsize);
N_NIMCALL(NimStringDesc*, copyString)(NimStringDesc* src);
static N_INLINE(void, writeln_91224)(FILE* f, NimStringDesc** x, NI xLen0);
N_NIMCALL(void, write_8862)(FILE* f, NimStringDesc* s);
N_NIMCALL(void, shmrelease_91407)(tshmbuffer91012* ob);
N_NIMCALL(void, shmpoke_91807)(tshmbuffer91012* ob, NI ofs, NU8 val);
N_NOINLINE(void, raiseIndexError)(void);
N_NIMCALL(void, shmbufferrangecheck_91816)(tshmbuffer91012* ob, NI ofs);
STRING_LITERAL(TMP249, "not ob.fInit == true ", 21);
STRING_LITERAL(TMP250, "not ob.fInit == false ", 22);
STRING_LITERAL(TMP878, "[initExec.valloc] mmap = MAP_FAILED", 35);
STRING_LITERAL(TMP879, "\012", 1);
STRING_LITERAL(TMP880, "[tShmBuffer.release] munmap < 0", 31);
STRING_LITERAL(TMP881, "[shmBufferRangeCheck] ofs > tShmBuffer.cBuffer", 46);
extern TFrame* frameptr_11625;
extern TNimType NTI1009; /* TObject */
TNimType NTI91004; /* tNavmBackend */
extern TNimType NTI107; /* int8 */
extern TNimType NTI107; /* int8 */
extern TNimType NTI91012; /* tShmBuffer */
extern TNimType NTI105; /* int */
extern TNimType NTI132; /* bool */

static N_INLINE(void, nimFrame)(TFrame* s) {
	(*s).prev = frameptr_11625;
	frameptr_11625 = s;
}

static N_INLINE(void, popFrame)(void) {
	frameptr_11625 = (*frameptr_11625).prev;
}

N_NIMCALL(void, init_91029)(tnavmbackend91004* ob, NI memsize) {
	nimfr("init", "navmBackend.nim")
	nimln(140, "navmBackend.nim");
	nimln(140, "navmBackend.nim");
	nimln(140, "navmBackend.nim");
	nimln(140, "navmBackend.nim");
	if (!!(!(((*ob).Finit == NIM_TRUE)))) goto LA2;
	{
		nimln(140, "navmBackend.nim");
		hiddenraiseassert_71220(((NimStringDesc*) &TMP249));
	}
	LA2: ;
	nimln(141, "navmBackend.nim");
	shminitexec_91207(&(*ob).Tcodemem, memsize);
	nimln(143, "navmBackend.nim");
	(*ob).Ocodemem = 0;
	nimln(144, "navmBackend.nim");
	(*ob).Finit = NIM_TRUE;
	nimln(145, "navmBackend.nim");
	(*ob).Vd = ((NI8) 1);
	nimln(146, "navmBackend.nim");
	(*ob).Vr = ((NI8) 1);
	popFrame();
}

N_NIMCALL(void, release_91253)(tnavmbackend91004* ob) {
	nimfr("release", "navmBackend.nim")
	nimln(149, "navmBackend.nim");
	nimln(149, "navmBackend.nim");
	nimln(149, "navmBackend.nim");
	nimln(149, "navmBackend.nim");
	if (!!(!(((*ob).Finit == NIM_FALSE)))) goto LA2;
	{
		nimln(149, "navmBackend.nim");
		hiddenraiseassert_71220(((NimStringDesc*) &TMP250));
	}
	LA2: ;
	nimln(150, "navmBackend.nim");
	shmrelease_91407(&(*ob).Tcodemem);
	nimln(151, "navmBackend.nim");
	(*ob).Finit = NIM_FALSE;
	popFrame();
}

static N_INLINE(NI, addInt)(NI a, NI b) {
	NI result;
	NIM_BOOL LOC2;
	result = 0;
	result = (NI)((NU64)(a) + (NU64)(b));
	LOC2 = (0 <= (NI)(result ^ a));
	if (LOC2) goto LA3;
	LOC2 = (0 <= (NI)(result ^ b));
	LA3: ;
	if (!LOC2) goto LA4;
	{
		goto BeforeRet;
	}
	LA4: ;
	raiseOverflow();
	BeforeRet: ;
	return result;
}

N_NIMCALL(void, compile_91628)(tnavmbackend91004* ob, NU8 val) {
	NI TMP251;
	nimfr("compile", "navmBackend.nim")
	nimln(154, "navmBackend.nim");
	nimln(154, "navmBackend.nim");
	nimln(154, "navmBackend.nim");
	nimln(154, "navmBackend.nim");
	if (!!(!(((*ob).Finit == NIM_FALSE)))) goto LA2;
	{
		nimln(154, "navmBackend.nim");
		hiddenraiseassert_71220(((NimStringDesc*) &TMP250));
	}
	LA2: ;
	nimln(155, "navmBackend.nim");
	shmpoke_91807(&(*ob).Tcodemem, (*ob).Ocodemem, val);
	nimln(156, "navmBackend.nim");
	nimln(156, "navmBackend.nim");
	TMP251 = addInt((*ob).Ocodemem, 1);
	(*ob).Ocodemem = (NI64)(TMP251);
	popFrame();
}

N_NIMCALL(void, compile2_92028)(tnavmbackend91004* ob, NU16 val, NIM_BOOL bigendian) {
	nimfr("compile2", "navmBackend.nim")
	nimln(159, "navmBackend.nim");
	nimln(159, "navmBackend.nim");
	nimln(159, "navmBackend.nim");
	nimln(159, "navmBackend.nim");
	if (!!(!(((*ob).Finit == NIM_FALSE)))) goto LA2;
	{
		nimln(159, "navmBackend.nim");
		hiddenraiseassert_71220(((NimStringDesc*) &TMP250));
	}
	LA2: ;
	nimln(160, "navmBackend.nim");
	nimln(696, "system.nim");
	nimln(696, "system.nim");
	if (!!((bigendian == NIM_FALSE))) goto LA6;
	{
		NU8 b1;
		NU8 b2;
		NI TMP252;
		NI TMP253;
		nimln(161, "navmBackend.nim");
		nimln(161, "navmBackend.nim");
		nimln(161, "navmBackend.nim");
		b1 = ((NU8) ((NU16)((NU16)((NU16)(val) >> (NU16)(((NU16) 8))) & ((NU16) 255))));
		nimln(162, "navmBackend.nim");
		nimln(162, "navmBackend.nim");
		b2 = ((NU8) ((NU16)(val & ((NU16) 255))));
		nimln(163, "navmBackend.nim");
		shmpoke_91807(&(*ob).Tcodemem, (*ob).Ocodemem, b2);
		nimln(164, "navmBackend.nim");
		nimln(164, "navmBackend.nim");
		TMP252 = addInt((*ob).Ocodemem, 1);
		shmpoke_91807(&(*ob).Tcodemem, (NI64)(TMP252), b1);
		nimln(165, "navmBackend.nim");
		nimln(165, "navmBackend.nim");
		TMP253 = addInt((*ob).Ocodemem, 2);
		(*ob).Ocodemem = (NI64)(TMP253);
	}
	goto LA5;
	LA6: ;
	{
		NU8 b1;
		NU8 b2;
		NI TMP254;
		NI TMP255;
		nimln(167, "navmBackend.nim");
		nimln(167, "navmBackend.nim");
		b1 = ((NU8) ((NU16)(val & ((NU16) 255))));
		nimln(168, "navmBackend.nim");
		nimln(168, "navmBackend.nim");
		nimln(168, "navmBackend.nim");
		b2 = ((NU8) ((NU16)((NU16)((NU16)(val) >> (NU16)(((NU16) 8))) & ((NU16) 255))));
		nimln(169, "navmBackend.nim");
		shmpoke_91807(&(*ob).Tcodemem, (*ob).Ocodemem, b2);
		nimln(170, "navmBackend.nim");
		nimln(170, "navmBackend.nim");
		TMP254 = addInt((*ob).Ocodemem, 1);
		shmpoke_91807(&(*ob).Tcodemem, (NI64)(TMP254), b1);
		nimln(171, "navmBackend.nim");
		nimln(171, "navmBackend.nim");
		TMP255 = addInt((*ob).Ocodemem, 2);
		(*ob).Ocodemem = (NI64)(TMP255);
	}
	LA5: ;
	popFrame();
}

N_NIMCALL(void, compile4_92308)(tnavmbackend91004* ob, NU32 val, NIM_BOOL bigendian) {
	nimfr("compile4", "navmBackend.nim")
	nimln(174, "navmBackend.nim");
	nimln(174, "navmBackend.nim");
	nimln(174, "navmBackend.nim");
	nimln(174, "navmBackend.nim");
	if (!!(!(((*ob).Finit == NIM_FALSE)))) goto LA2;
	{
		nimln(174, "navmBackend.nim");
		hiddenraiseassert_71220(((NimStringDesc*) &TMP250));
	}
	LA2: ;
	nimln(175, "navmBackend.nim");
	nimln(696, "system.nim");
	nimln(696, "system.nim");
	if (!!((bigendian == NIM_FALSE))) goto LA6;
	{
		NU8 b1;
		NU8 b2;
		NU8 b3;
		NU8 b4;
		NI TMP256;
		NI TMP257;
		NI TMP258;
		NI TMP259;
		nimln(176, "navmBackend.nim");
		nimln(176, "navmBackend.nim");
		nimln(176, "navmBackend.nim");
		b1 = ((NU8) ((NU32)((NU32)((NU32)(val) >> (NU32)(((NU32) 24))) & ((NU32) 255))));
		nimln(177, "navmBackend.nim");
		nimln(177, "navmBackend.nim");
		nimln(177, "navmBackend.nim");
		b2 = ((NU8) ((NU32)((NU32)((NU32)(val) >> (NU32)(((NU32) 16))) & ((NU32) 255))));
		nimln(178, "navmBackend.nim");
		nimln(178, "navmBackend.nim");
		nimln(178, "navmBackend.nim");
		b3 = ((NU8) ((NU32)((NU32)((NU32)(val) >> (NU32)(((NU32) 8))) & ((NU32) 255))));
		nimln(179, "navmBackend.nim");
		nimln(179, "navmBackend.nim");
		b4 = ((NU8) ((NU32)(val & ((NU32) 255))));
		nimln(180, "navmBackend.nim");
		shmpoke_91807(&(*ob).Tcodemem, (*ob).Ocodemem, b4);
		nimln(181, "navmBackend.nim");
		nimln(181, "navmBackend.nim");
		TMP256 = addInt((*ob).Ocodemem, 1);
		shmpoke_91807(&(*ob).Tcodemem, (NI64)(TMP256), b3);
		nimln(182, "navmBackend.nim");
		nimln(182, "navmBackend.nim");
		TMP257 = addInt((*ob).Ocodemem, 2);
		shmpoke_91807(&(*ob).Tcodemem, (NI64)(TMP257), b2);
		nimln(183, "navmBackend.nim");
		nimln(183, "navmBackend.nim");
		TMP258 = addInt((*ob).Ocodemem, 3);
		shmpoke_91807(&(*ob).Tcodemem, (NI64)(TMP258), b1);
		nimln(184, "navmBackend.nim");
		nimln(184, "navmBackend.nim");
		TMP259 = addInt((*ob).Ocodemem, 4);
		(*ob).Ocodemem = (NI64)(TMP259);
	}
	goto LA5;
	LA6: ;
	{
		NU8 b1;
		NU8 b2;
		NU8 b3;
		NU8 b4;
		NI TMP260;
		NI TMP261;
		NI TMP262;
		NI TMP263;
		nimln(186, "navmBackend.nim");
		nimln(186, "navmBackend.nim");
		b1 = ((NU8) ((NU32)(val & ((NU32) 255))));
		nimln(187, "navmBackend.nim");
		nimln(187, "navmBackend.nim");
		nimln(187, "navmBackend.nim");
		b2 = ((NU8) ((NU32)((NU32)((NU32)(val) >> (NU32)(((NU32) 8))) & ((NU32) 255))));
		nimln(188, "navmBackend.nim");
		nimln(188, "navmBackend.nim");
		nimln(188, "navmBackend.nim");
		b3 = ((NU8) ((NU32)((NU32)((NU32)(val) >> (NU32)(((NU32) 16))) & ((NU32) 255))));
		nimln(189, "navmBackend.nim");
		nimln(189, "navmBackend.nim");
		nimln(189, "navmBackend.nim");
		b4 = ((NU8) ((NU32)((NU32)((NU32)(val) >> (NU32)(((NU32) 24))) & ((NU32) 255))));
		nimln(190, "navmBackend.nim");
		shmpoke_91807(&(*ob).Tcodemem, (*ob).Ocodemem, b4);
		nimln(191, "navmBackend.nim");
		nimln(191, "navmBackend.nim");
		TMP260 = addInt((*ob).Ocodemem, 1);
		shmpoke_91807(&(*ob).Tcodemem, (NI64)(TMP260), b3);
		nimln(192, "navmBackend.nim");
		nimln(192, "navmBackend.nim");
		TMP261 = addInt((*ob).Ocodemem, 2);
		shmpoke_91807(&(*ob).Tcodemem, (NI64)(TMP261), b2);
		nimln(193, "navmBackend.nim");
		nimln(193, "navmBackend.nim");
		TMP262 = addInt((*ob).Ocodemem, 3);
		shmpoke_91807(&(*ob).Tcodemem, (NI64)(TMP262), b1);
		nimln(194, "navmBackend.nim");
		nimln(194, "navmBackend.nim");
		TMP263 = addInt((*ob).Ocodemem, 4);
		(*ob).Ocodemem = (NI64)(TMP263);
	}
	LA5: ;
	popFrame();
}

N_NIMCALL(void, compile8_92620)(tnavmbackend91004* ob, NU64 val, NIM_BOOL bigendian) {
	nimfr("compile8", "navmBackend.nim")
	nimln(197, "navmBackend.nim");
	nimln(197, "navmBackend.nim");
	nimln(197, "navmBackend.nim");
	nimln(197, "navmBackend.nim");
	if (!!(!(((*ob).Finit == NIM_FALSE)))) goto LA2;
	{
		nimln(197, "navmBackend.nim");
		hiddenraiseassert_71220(((NimStringDesc*) &TMP250));
	}
	LA2: ;
	nimln(198, "navmBackend.nim");
	nimln(696, "system.nim");
	nimln(696, "system.nim");
	if (!!((bigendian == NIM_FALSE))) goto LA6;
	{
		NU8 b1;
		NU8 b2;
		NU8 b3;
		NU8 b4;
		NU8 b5;
		NU8 b6;
		NU8 b7;
		NU8 b8;
		NI TMP264;
		NI TMP265;
		NI TMP266;
		NI TMP267;
		NI TMP268;
		NI TMP269;
		NI TMP270;
		NI TMP271;
		nimln(199, "navmBackend.nim");
		nimln(199, "navmBackend.nim");
		nimln(199, "navmBackend.nim");
		b1 = ((NU8) ((NU64)((NU64)((NU64)(val) >> (NU64)(((NU64) 56))) & ((NU64) 255))));
		nimln(200, "navmBackend.nim");
		nimln(200, "navmBackend.nim");
		nimln(200, "navmBackend.nim");
		b2 = ((NU8) ((NU64)((NU64)((NU64)(val) >> (NU64)(((NU64) 48))) & ((NU64) 255))));
		nimln(201, "navmBackend.nim");
		nimln(201, "navmBackend.nim");
		nimln(201, "navmBackend.nim");
		b3 = ((NU8) ((NU64)((NU64)((NU64)(val) >> (NU64)(((NU64) 40))) & ((NU64) 255))));
		nimln(202, "navmBackend.nim");
		nimln(202, "navmBackend.nim");
		nimln(202, "navmBackend.nim");
		b4 = ((NU8) ((NU64)((NU64)((NU64)(val) >> (NU64)(((NU64) 32))) & ((NU64) 255))));
		nimln(203, "navmBackend.nim");
		nimln(203, "navmBackend.nim");
		nimln(203, "navmBackend.nim");
		b5 = ((NU8) ((NU64)((NU64)((NU64)(val) >> (NU64)(((NU64) 24))) & ((NU64) 255))));
		nimln(204, "navmBackend.nim");
		nimln(204, "navmBackend.nim");
		nimln(204, "navmBackend.nim");
		b6 = ((NU8) ((NU64)((NU64)((NU64)(val) >> (NU64)(((NU64) 16))) & ((NU64) 255))));
		nimln(205, "navmBackend.nim");
		nimln(205, "navmBackend.nim");
		nimln(205, "navmBackend.nim");
		b7 = ((NU8) ((NU64)((NU64)((NU64)(val) >> (NU64)(((NU64) 8))) & ((NU64) 255))));
		nimln(206, "navmBackend.nim");
		nimln(206, "navmBackend.nim");
		b8 = ((NU8) ((NU64)(val & ((NU64) 255))));
		nimln(207, "navmBackend.nim");
		shmpoke_91807(&(*ob).Tcodemem, (*ob).Ocodemem, b8);
		nimln(208, "navmBackend.nim");
		nimln(208, "navmBackend.nim");
		TMP264 = addInt((*ob).Ocodemem, 1);
		shmpoke_91807(&(*ob).Tcodemem, (NI64)(TMP264), b7);
		nimln(209, "navmBackend.nim");
		nimln(209, "navmBackend.nim");
		TMP265 = addInt((*ob).Ocodemem, 2);
		shmpoke_91807(&(*ob).Tcodemem, (NI64)(TMP265), b6);
		nimln(210, "navmBackend.nim");
		nimln(210, "navmBackend.nim");
		TMP266 = addInt((*ob).Ocodemem, 3);
		shmpoke_91807(&(*ob).Tcodemem, (NI64)(TMP266), b5);
		nimln(211, "navmBackend.nim");
		nimln(211, "navmBackend.nim");
		TMP267 = addInt((*ob).Ocodemem, 4);
		shmpoke_91807(&(*ob).Tcodemem, (NI64)(TMP267), b4);
		nimln(212, "navmBackend.nim");
		nimln(212, "navmBackend.nim");
		TMP268 = addInt((*ob).Ocodemem, 5);
		shmpoke_91807(&(*ob).Tcodemem, (NI64)(TMP268), b3);
		nimln(213, "navmBackend.nim");
		nimln(213, "navmBackend.nim");
		TMP269 = addInt((*ob).Ocodemem, 6);
		shmpoke_91807(&(*ob).Tcodemem, (NI64)(TMP269), b2);
		nimln(214, "navmBackend.nim");
		nimln(214, "navmBackend.nim");
		TMP270 = addInt((*ob).Ocodemem, 7);
		shmpoke_91807(&(*ob).Tcodemem, (NI64)(TMP270), b1);
		nimln(215, "navmBackend.nim");
		nimln(215, "navmBackend.nim");
		TMP271 = addInt((*ob).Ocodemem, 8);
		(*ob).Ocodemem = (NI64)(TMP271);
	}
	goto LA5;
	LA6: ;
	{
		NU8 b1;
		NU8 b2;
		NU8 b3;
		NU8 b4;
		NU8 b5;
		NU8 b6;
		NU8 b7;
		NU8 b8;
		NI TMP272;
		NI TMP273;
		NI TMP274;
		NI TMP275;
		NI TMP276;
		NI TMP277;
		NI TMP278;
		NI TMP279;
		nimln(217, "navmBackend.nim");
		nimln(217, "navmBackend.nim");
		b1 = ((NU8) ((NU64)(val & ((NU64) 255))));
		nimln(218, "navmBackend.nim");
		nimln(218, "navmBackend.nim");
		nimln(218, "navmBackend.nim");
		b2 = ((NU8) ((NU64)((NU64)((NU64)(val) >> (NU64)(((NU64) 8))) & ((NU64) 255))));
		nimln(219, "navmBackend.nim");
		nimln(219, "navmBackend.nim");
		nimln(219, "navmBackend.nim");
		b3 = ((NU8) ((NU64)((NU64)((NU64)(val) >> (NU64)(((NU64) 16))) & ((NU64) 255))));
		nimln(220, "navmBackend.nim");
		nimln(220, "navmBackend.nim");
		nimln(220, "navmBackend.nim");
		b4 = ((NU8) ((NU64)((NU64)((NU64)(val) >> (NU64)(((NU64) 24))) & ((NU64) 255))));
		nimln(221, "navmBackend.nim");
		nimln(221, "navmBackend.nim");
		nimln(221, "navmBackend.nim");
		b5 = ((NU8) ((NU64)((NU64)((NU64)(val) >> (NU64)(((NU64) 32))) & ((NU64) 255))));
		nimln(222, "navmBackend.nim");
		nimln(222, "navmBackend.nim");
		nimln(222, "navmBackend.nim");
		b6 = ((NU8) ((NU64)((NU64)((NU64)(val) >> (NU64)(((NU64) 40))) & ((NU64) 255))));
		nimln(223, "navmBackend.nim");
		nimln(223, "navmBackend.nim");
		nimln(223, "navmBackend.nim");
		b7 = ((NU8) ((NU64)((NU64)((NU64)(val) >> (NU64)(((NU64) 48))) & ((NU64) 255))));
		nimln(224, "navmBackend.nim");
		nimln(224, "navmBackend.nim");
		nimln(224, "navmBackend.nim");
		b8 = ((NU8) ((NU64)((NU64)((NU64)(val) >> (NU64)(((NU64) 56))) & ((NU64) 255))));
		nimln(225, "navmBackend.nim");
		shmpoke_91807(&(*ob).Tcodemem, (*ob).Ocodemem, b8);
		nimln(226, "navmBackend.nim");
		nimln(226, "navmBackend.nim");
		TMP272 = addInt((*ob).Ocodemem, 1);
		shmpoke_91807(&(*ob).Tcodemem, (NI64)(TMP272), b7);
		nimln(227, "navmBackend.nim");
		nimln(227, "navmBackend.nim");
		TMP273 = addInt((*ob).Ocodemem, 2);
		shmpoke_91807(&(*ob).Tcodemem, (NI64)(TMP273), b6);
		nimln(228, "navmBackend.nim");
		nimln(228, "navmBackend.nim");
		TMP274 = addInt((*ob).Ocodemem, 3);
		shmpoke_91807(&(*ob).Tcodemem, (NI64)(TMP274), b5);
		nimln(229, "navmBackend.nim");
		nimln(229, "navmBackend.nim");
		TMP275 = addInt((*ob).Ocodemem, 4);
		shmpoke_91807(&(*ob).Tcodemem, (NI64)(TMP275), b4);
		nimln(230, "navmBackend.nim");
		nimln(230, "navmBackend.nim");
		TMP276 = addInt((*ob).Ocodemem, 5);
		shmpoke_91807(&(*ob).Tcodemem, (NI64)(TMP276), b3);
		nimln(231, "navmBackend.nim");
		nimln(231, "navmBackend.nim");
		TMP277 = addInt((*ob).Ocodemem, 6);
		shmpoke_91807(&(*ob).Tcodemem, (NI64)(TMP277), b2);
		nimln(232, "navmBackend.nim");
		nimln(232, "navmBackend.nim");
		TMP278 = addInt((*ob).Ocodemem, 7);
		shmpoke_91807(&(*ob).Tcodemem, (NI64)(TMP278), b1);
		nimln(233, "navmBackend.nim");
		nimln(233, "navmBackend.nim");
		TMP279 = addInt((*ob).Ocodemem, 8);
		(*ob).Ocodemem = (NI64)(TMP279);
	}
	LA5: ;
	popFrame();
}

N_NIMCALL(void*, valloc_91214)(NI memsize) {
	void* result;
	NI size;
	nimfr("valloc", "shmBuffer.nim")
	result = 0;
	nimln(51, "shmBuffer.nim");
	size = memsize;
	nimln(52, "shmBuffer.nim");
	nimln(704, "system.nim");
	if (!(1048575 < size)) goto LA2;
	{
		nimln(52, "shmBuffer.nim");
		size = 1048575;
	}
	LA2: ;
	nimln(53, "shmBuffer.nim");
	nimln(54, "shmBuffer.nim");
	nimln(54, "shmBuffer.nim");
	nimln(55, "shmBuffer.nim");
	result = mmap(NIM_NIL, size, (NI32)((NI32)(PROT_EXEC | PROT_READ) | PROT_WRITE), (NI32)(((NI32) 32) | MAP_SHARED), ((int) -1), 0);
	nimln(57, "shmBuffer.nim");
	nimln(57, "shmBuffer.nim");
	if (!(result == NIM_NIL)) goto LA6;
	{
		TY94095 LOC9;
		nimln(58, "shmBuffer.nim");
		memset((void*)LOC9, 0, sizeof(LOC9));
		nimln(58, "shmBuffer.nim");
		LOC9[0] = copyString(((NimStringDesc*) &TMP878));
		writeln_91224(stderr, LOC9, 1);
		nimln(59, "shmBuffer.nim");
		exit(0);
	}
	LA6: ;
	popFrame();
	return result;
}

N_NIMCALL(void, shminitexec_91207)(tshmbuffer91012* ob, NI memsize) {
	void* LOC1;
	nimfr("shmInitExec", "shmBuffer.nim")
	nimln(61, "shmBuffer.nim");
	nimln(61, "shmBuffer.nim");
	LOC1 = valloc_91214(memsize);
	(*ob).Pbuffer = ((NU8*) (LOC1));
	nimln(62, "shmBuffer.nim");
	(*ob).Cbuffer = memsize;
	nimln(63, "shmBuffer.nim");
	(*ob).Finit = NIM_TRUE;
	popFrame();
}

static N_INLINE(void, writeln_91224)(FILE* f, NimStringDesc** x, NI xLen0) {
	NimStringDesc* i_91238;
	NI i_91240;
	nimfr("writeln", "sysio.nim")
	i_91238 = 0;
	nimln(1343, "system.nim");
	i_91240 = 0;
	nimln(1344, "system.nim");
	while (1) {
		nimln(1344, "system.nim");
		nimln(1344, "system.nim");
		if (!(i_91240 < xLen0)) goto LA1;
		nimln(1345, "system.nim");
		i_91238 = x[i_91240];
		nimln(159, "sysio.nim");
		write_8862(f, i_91238);
		nimln(1346, "system.nim");
		i_91240 += 1;
	} LA1: ;
	nimln(160, "sysio.nim");
	write_8862(f, ((NimStringDesc*) &TMP879));
	popFrame();
}

N_NIMCALL(void, shmrelease_91407)(tshmbuffer91012* ob) {
	int erg;
	nimfr("shmRelease", "shmBuffer.nim")
	nimln(86, "shmBuffer.nim");
	nimln(86, "shmBuffer.nim");
	nimln(86, "shmBuffer.nim");
	nimln(86, "shmBuffer.nim");
	if (!!(!(((*ob).Finit == NIM_FALSE)))) goto LA2;
	{
		nimln(86, "shmBuffer.nim");
		hiddenraiseassert_71220(((NimStringDesc*) &TMP250));
	}
	LA2: ;
	nimln(87, "shmBuffer.nim");
	erg = munmap(((void*) ((*ob).Pbuffer)), (*ob).Cbuffer);
	nimln(88, "shmBuffer.nim");
	nimln(88, "shmBuffer.nim");
	if (!(erg < ((NI32) 0))) goto LA6;
	{
		TY94095 LOC9;
		nimln(89, "shmBuffer.nim");
		memset((void*)LOC9, 0, sizeof(LOC9));
		nimln(89, "shmBuffer.nim");
		LOC9[0] = copyString(((NimStringDesc*) &TMP880));
		writeln_91224(stderr, LOC9, 1);
		nimln(90, "shmBuffer.nim");
		exit(0);
	}
	LA6: ;
	popFrame();
}

N_NIMCALL(void, shmpoke_91807)(tshmbuffer91012* ob, NI ofs, NU8 val) {
	nimfr("shmPoke", "shmBuffer.nim")
	nimln(108, "shmBuffer.nim");
	shmbufferrangecheck_91816(ob, ofs);
	nimln(109, "shmBuffer.nim");
	if ((NU)(ofs) > (NU)(1048575)) raiseIndexError();
	((NU8*) ((*ob).Pbuffer))[(ofs)- 0] = val;
	popFrame();
}

N_NIMCALL(void, shmbufferrangecheck_91816)(tshmbuffer91012* ob, NI ofs) {
	nimfr("shmBufferRangeCheck", "shmBuffer.nim")
	nimln(98, "shmBuffer.nim");
	nimln(98, "shmBuffer.nim");
	nimln(98, "shmBuffer.nim");
	nimln(98, "shmBuffer.nim");
	if (!!(!(((*ob).Finit == NIM_FALSE)))) goto LA2;
	{
		nimln(98, "shmBuffer.nim");
		hiddenraiseassert_71220(((NimStringDesc*) &TMP250));
	}
	LA2: ;
	nimln(99, "shmBuffer.nim");
	nimln(704, "system.nim");
	if (!((*ob).Cbuffer < ofs)) goto LA6;
	{
		TY94095 LOC9;
		nimln(100, "shmBuffer.nim");
		memset((void*)LOC9, 0, sizeof(LOC9));
		nimln(100, "shmBuffer.nim");
		LOC9[0] = copyString(((NimStringDesc*) &TMP881));
		writeln_91224(stderr, LOC9, 1);
		nimln(101, "shmBuffer.nim");
		exit(0);
	}
	LA6: ;
	popFrame();
}
N_NOINLINE(void, navmBackendInit)(void) {
	nimfr("navmBackend", "navmBackend.nim")
	popFrame();
}

N_NOINLINE(void, navmBackendDatInit)(void) {
static TNimNode* TMP868[5];
static TNimNode TMP121[6];
NTI91004.size = sizeof(tnavmbackend91004);
NTI91004.kind = 17;
NTI91004.base = (&NTI1009);
NTI91004.flags = 3;
TMP868[0] = &TMP121[1];
TMP121[1].kind = 1;
TMP121[1].offset = offsetof(tnavmbackend91004, Vd);
TMP121[1].typ = (&NTI107);
TMP121[1].name = "vD";
TMP868[1] = &TMP121[2];
TMP121[2].kind = 1;
TMP121[2].offset = offsetof(tnavmbackend91004, Vr);
TMP121[2].typ = (&NTI107);
TMP121[2].name = "vR";
TMP868[2] = &TMP121[3];
TMP121[3].kind = 1;
TMP121[3].offset = offsetof(tnavmbackend91004, Tcodemem);
TMP121[3].typ = (&NTI91012);
TMP121[3].name = "tCodeMem";
TMP868[3] = &TMP121[4];
TMP121[4].kind = 1;
TMP121[4].offset = offsetof(tnavmbackend91004, Ocodemem);
TMP121[4].typ = (&NTI105);
TMP121[4].name = "oCodeMem";
TMP868[4] = &TMP121[5];
TMP121[5].kind = 1;
TMP121[5].offset = offsetof(tnavmbackend91004, Finit);
TMP121[5].typ = (&NTI132);
TMP121[5].name = "fInit";
TMP121[0].len = 5; TMP121[0].kind = 2; TMP121[0].sons = &TMP868[0];
NTI91004.node = &TMP121[0];
}

