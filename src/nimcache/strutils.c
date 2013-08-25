/* Generated by Nimrod Compiler v0.9.2 */
/*   (c) 2012 Andreas Rumpf */
/* The generated code is subject to the original license. */
/* Compiled for: Linux, amd64, gcc */
/* Command for C compiler:
   gcc -c  -w  -I/home/claude/Projekte/Nimrod/lib -o /home/claude/Projekte/vanar/src/nimcache/strutils.o /home/claude/Projekte/vanar/src/nimcache/strutils.c */
#define NIM_INTBITS 64
#include "nimbase.h"

#include <string.h>
typedef struct NimStringDesc NimStringDesc;
typedef struct TGenericSeq TGenericSeq;
struct TGenericSeq {
NI len;
NI reserved;
};
typedef NIM_CHAR TY611[100000001];
struct NimStringDesc {
  TGenericSeq Sup;
TY611 data;
};
N_NOINLINE(void, raiseOverflow)(void);
N_NIMCALL(NimStringDesc*, nimIntToStr)(NI x);
static N_INLINE(NI, subInt)(NI a, NI b);
static N_INLINE(void, appendChar)(NimStringDesc* dest, NIM_CHAR c);
static N_INLINE(void, appendString)(NimStringDesc* dest, NimStringDesc* src);
N_NIMCALL(NimStringDesc*, rawNewString)(NI space);
static N_INLINE(NI, addInt)(NI a, NI b);
static N_INLINE(void, nimFrame)(TFrame* s);
static N_INLINE(void, popFrame)(void);
N_NIMCALL(NimStringDesc*, mnewString)(NI len);
N_NIMCALL(NimStringDesc*, mnewString)(NI len);
N_NOINLINE(void, raiseIndexError)(void);
static N_INLINE(NI64, addInt64)(NI64 a, NI64 b);
STRING_LITERAL(TMP287, "0123456789ABCDEF", 16);
extern TFrame* frameptr_11625;

static N_INLINE(NI, subInt)(NI a, NI b) {
	NI result;
	NIM_BOOL LOC2;
	result = 0;
	result = (NI)((NU64)(a) - (NU64)(b));
	LOC2 = (0 <= (NI)(result ^ a));
	if (LOC2) goto LA3;
	LOC2 = (0 <= (NI)(result ^ (NI)((NU64) ~(b))));
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

static N_INLINE(void, appendChar)(NimStringDesc* dest, NIM_CHAR c) {
	(*dest).data[((*dest).Sup.len)- 0] = c;
	(*dest).data[((NI64)((*dest).Sup.len + 1))- 0] = 0;
	(*dest).Sup.len += 1;
}

static N_INLINE(void, appendString)(NimStringDesc* dest, NimStringDesc* src) {
	memcpy(((NCSTRING) (&(*dest).data[((*dest).Sup.len)- 0])), ((NCSTRING) ((*src).data)), (NI64)((*src).Sup.len + 1));
	(*dest).Sup.len += (*src).Sup.len;
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

static N_INLINE(void, nimFrame)(TFrame* s) {
	(*s).prev = frameptr_11625;
	frameptr_11625 = s;
}

static N_INLINE(void, popFrame)(void) {
	frameptr_11625 = (*frameptr_11625).prev;
}

N_NIMCALL(NimStringDesc*, nsuIntToStr)(NI x, NI minchars) {
	NimStringDesc* result;
	NI i_76550;
	NI HEX3Atmp_76553;
	NI TMP160;
	NI res_76555;
	nimfr("intToStr", "strutils.nim")
	result = 0;
	nimln(341, "strutils.nim");
	nimln(341, "strutils.nim");
	nimln(341, "strutils.nim");
	if (x == (IL64(-9223372036854775807) - IL64(1))) raiseOverflow();
	result = nimIntToStr((NI64)abs(x));
	i_76550 = 0;
	HEX3Atmp_76553 = 0;
	nimln(342, "strutils.nim");
	nimln(342, "strutils.nim");
	nimln(342, "strutils.nim");
	TMP160 = subInt(minchars, result->Sup.len);
	HEX3Atmp_76553 = (NI64)(TMP160);
	nimln(1293, "system.nim");
	res_76555 = 1;
	nimln(1294, "system.nim");
	while (1) {
		NimStringDesc* LOC2;
		nimln(1294, "system.nim");
		if (!(res_76555 <= HEX3Atmp_76553)) goto LA1;
		nimln(1293, "system.nim");
		i_76550 = res_76555;
		nimln(343, "strutils.nim");
		nimln(343, "strutils.nim");
		LOC2 = 0;
		LOC2 = rawNewString(result->Sup.len + 1);
appendChar(LOC2, 48);
appendString(LOC2, result);
		result = LOC2;
		nimln(1296, "system.nim");
		res_76555 = addInt(res_76555, 1);
	} LA1: ;
	nimln(344, "strutils.nim");
	nimln(344, "strutils.nim");
	if (!(x < 0)) goto LA4;
	{
		NimStringDesc* LOC7;
		nimln(345, "strutils.nim");
		nimln(345, "strutils.nim");
		LOC7 = 0;
		LOC7 = rawNewString(result->Sup.len + 1);
appendChar(LOC7, 45);
appendString(LOC7, result);
		result = LOC7;
	}
	LA4: ;
	popFrame();
	return result;
}

N_NIMCALL(NimStringDesc*, nsuRepeatChar)(NI count, NIM_CHAR c) {
	NimStringDesc* result;
	NI i_76856;
	NI HEX3Atmp_76857;
	NI TMP221;
	NI res_76859;
	nimfr("repeatChar", "strutils.nim")
	result = 0;
	nimln(434, "strutils.nim");
	nimln(434, "strutils.nim");
	result = mnewString(count);
	i_76856 = 0;
	HEX3Atmp_76857 = 0;
	nimln(435, "strutils.nim");
	nimln(435, "strutils.nim");
	TMP221 = subInt(count, 1);
	HEX3Atmp_76857 = (NI64)(TMP221);
	nimln(1293, "system.nim");
	res_76859 = 0;
	nimln(1294, "system.nim");
	while (1) {
		nimln(1294, "system.nim");
		if (!(res_76859 <= HEX3Atmp_76857)) goto LA1;
		nimln(1293, "system.nim");
		i_76856 = res_76859;
		nimln(435, "strutils.nim");
		if ((NU)(i_76856) > (NU)(result->Sup.len)) raiseIndexError();
		result->data[i_76856] = c;
		nimln(1296, "system.nim");
		res_76859 = addInt(res_76859, 1);
	} LA1: ;
	popFrame();
	return result;
}

static N_INLINE(NI64, addInt64)(NI64 a, NI64 b) {
	NI64 result;
	NIM_BOOL LOC2;
	result = 0;
	result = (NI64)((NU64)(a) + (NU64)(b));
	LOC2 = (0 <= (NI64)(result ^ a));
	if (LOC2) goto LA3;
	LOC2 = (0 <= (NI64)(result ^ b));
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

N_NIMCALL(NimStringDesc*, nsuToHex)(NI64 x, NI len) {
	NimStringDesc* result;
	NI64 shift;
	NI j_76529;
	NI HEX3Atmp_76532;
	NI TMP286;
	NI res_76534;
	nimfr("toHex", "strutils.nim")
	result = 0;
	shift = 0;
	nimln(331, "strutils.nim");
	nimln(331, "strutils.nim");
	result = mnewString(len);
	j_76529 = 0;
	HEX3Atmp_76532 = 0;
	nimln(332, "strutils.nim");
	nimln(332, "strutils.nim");
	TMP286 = subInt(len, 1);
	HEX3Atmp_76532 = (NI64)(TMP286);
	nimln(1277, "system.nim");
	res_76534 = HEX3Atmp_76532;
	nimln(1278, "system.nim");
	while (1) {
		NI64 TMP288;
		nimln(700, "system.nim");
		if (!(0 <= res_76534)) goto LA1;
		nimln(1277, "system.nim");
		j_76529 = res_76534;
		nimln(333, "strutils.nim");
		if ((NU)(j_76529) > (NU)(result->Sup.len)) raiseIndexError();
		nimln(333, "strutils.nim");
		nimln(333, "strutils.nim");
		nimln(333, "strutils.nim");
		if ((NU)((NI32)(((NI32)(NU32)(NU64)((NI64)((NU64)(x) >> (NU64)(shift)))) & ((NI32) 15))) > (NU)(((NimStringDesc*) &TMP287)->Sup.len)) raiseIndexError();
		result->data[j_76529] = ((NimStringDesc*) &TMP287)->data[(NI32)(((NI32)(NU32)(NU64)((NI64)((NU64)(x) >> (NU64)(shift)))) & ((NI32) 15))];
		nimln(334, "strutils.nim");
		nimln(334, "strutils.nim");
		TMP288 = addInt64(shift, 4);
		shift = (NI64)(TMP288);
		nimln(1280, "system.nim");
		res_76534 = subInt(res_76534, 1);
	} LA1: ;
	popFrame();
	return result;
}
N_NOINLINE(void, strutilsInit)(void) {
	nimfr("strutils", "strutils.nim")
	popFrame();
}

N_NOINLINE(void, strutilsDatInit)(void) {
}

