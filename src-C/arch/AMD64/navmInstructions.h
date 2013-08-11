/*****************************************************************************
  PROJECT:     NAVM AOT COMPILER
  DESCRIPTION: COMPILER FOR THE NAVM LEVEL 1 ISA AS IL REPRESENTATION
  INFO:        ALL BASIC NAVM LEVEL 1 INSTRUCTIONs FOR AMD64 COMP. PROCESSORS
  AUTHOR:      MATTHIAS SCHIRM
  COPYRIGHT:   (C) 2013 MATTHIAS SCHIRM, ALL RIGHTS RESERVED
  LICENCE:     BSD STYLE, SEE LICENCE.TXT

               Copyright (c) 2013, Matthias Schirm
               All rights reserved.

               Redistribution and use in source and binary forms, with or
               without modification, are permitted provided that the
               following conditions are met:

               1. Redistributions of source code must retain the above
                  copyright notice, this list of conditions and the
                  following disclaimer.

               2. Redistributions in binary form must reproduce the above
                  copyright notice, this list of conditions and the
                  following disclaimer in the documentation and/or other
                  materials provided with the distribution.

               3. Neither the name of the author nor the names of its
                  contributors may be used to endorse or promote products
                  derived from this software without specific prior
                  written permission.

               THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
               CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
               INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
               MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
               DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
               CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
               SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
               NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
               LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
               HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
               CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
               OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
               SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  COMPILER:    GCC V4 ENGINE
  STANDARD:    ANSI C 1989
  HISTORY:     7-15-2013: FIRST VERSION
  TO-DO:       NONE AT CURRENT
  ****************************************************************************/

/* optional helping routines. These routines ARE processor AND implementation
   dependent */

void errorHalt (char *msg)
  {fprintf (stderr, "%s\n", msg); exit (0);}

void compCode (pNavmBackend handle, uByte code)
  {if (handle->oMem < handle->cMemSize)
        handle->pMem[handle->oMem++] = (uByte) code;
   else errorHalt ("[compByte] oMem > cMemSize");}

void compImm64 (pNavmBackend handle, sWord value)
  {uImm imm; imm.sImm = value;
   if (handle->oMem < handle->cMemSize)
       {handle->pMem[handle->oMem++] = (uByte) imm.aSlices[0];
        handle->pMem[handle->oMem++] = (uByte) imm.aSlices[1];
        handle->pMem[handle->oMem++] = (uByte) imm.aSlices[2];
        handle->pMem[handle->oMem++] = (uByte) imm.aSlices[3];
        handle->pMem[handle->oMem++] = (uByte) imm.aSlices[4];
        handle->pMem[handle->oMem++] = (uByte) imm.aSlices[5];
        handle->pMem[handle->oMem++] = (uByte) imm.aSlices[6];
        handle->pMem[handle->oMem++] = (uByte) imm.aSlices[7];}
   else errorHalt ("[compImm64] oMem > cMemSize");}

void compImm32 (pNavmBackend handle, int32_t value)
  {uImm32 imm; imm.sImm = value;
   if (handle->oMem < handle->cMemSize)
       {handle->pMem[handle->oMem++] = (uByte) imm.aSlices[0];
        handle->pMem[handle->oMem++] = (uByte) imm.aSlices[1];
        handle->pMem[handle->oMem++] = (uByte) imm.aSlices[2];
        handle->pMem[handle->oMem++] = (uByte) imm.aSlices[3];}
   else errorHalt ("[compImm32] oMem > cMemSize");}

void compIncD (pNavmBackend handle) {handle->vD++;}

void compDecD (pNavmBackend handle)
  {if (handle->vD > 0) handle->vD--;
      else errorHalt ("[compDecD] vD < 1");}

void compDisAsmImm64 (pNavmBackend h, char *m, sWord val)
  {if (h->fDisAsm != false)
      {fprintf (stderr, "vD: %li | ",h->vD);
       fprintf (stderr, "%x:\t", (int) h->oMem);
       fprintf (stderr, "%s", m);
       fprintf (stderr, "%lx\n", val);}}

void compDisAsmImm32 (pNavmBackend h, char *m, int32_t val)
  {if (h->fDisAsm != false)
      {fprintf (stderr, "vD: %li | ",h->vD);
       fprintf (stderr, "%x:\t", (int) h->oMem);
       fprintf (stderr, "%s", m);
       fprintf (stderr, "%x\n", val);}}

void compDisAsmImm8 (pNavmBackend h, char *m, uByte val)
  {if (h->fDisAsm != false)
      {fprintf (stderr, "vD: %li | ",h->vD);
       fprintf (stderr, "%x:\t", (int) h->oMem);
       fprintf (stderr, "%s", m);
       fprintf (stderr, "%i\n", (int) val);}}

void compDisAsm (pNavmBackend h, char *m)
  {if (h->fDisAsm != false)
      {fprintf (stderr, "vD: %li | ",h->vD);
       fprintf (stderr, "%x:\t", (int) h->oMem);
       fprintf (stderr, "%s\n", m);}}

/* -------------------------------------------------------------------------
   INFO: There can exist two versions for each base instruction, one which
   maps to the register file dependent of vD and a shorter immediate version
   which modifies TOS directly. Because the orthogonality of immediate
   instructions differs largely between CPU architectures, the following
   constants define for which instructions immediate-direct versions exist.
   Please note in addition that these constants are CPU dependent of course
   must exist for all 16 base instructions!
   ------------------------------------------------------------------------- */

const bool cfNavmBackendImmLI  = false;
const bool cfNavmBackendImmLD  = false;
const bool cfNavmBackendImmST  = false;
const bool cfNavmBackendImmCP  = true;
const bool cfNavmBackendImmADD = true;
const bool cfNavmBackendImmDIV = false;
const bool cfNavmBackendImmMUL = true;
const bool cfNavmBackendImmAND = true;
const bool cfNavmBackendImmGOR = true;
const bool cfNavmBackendImmXOR = true;
const bool cfNavmBackendImmSHL = true;
const bool cfNavmBackendImmSHR = true;
const bool cfNavmBackendImmDUP = false;
const bool cfNavmBackendImmDRP = false;
const bool cfNavmBackendImmSWP = false;
const bool cfNavmBackendImmROL = false;

/* -------------------------------------------------------------------------
   INFO: These constants define the word lenght of all avariable
   immediate-direct procedures in bytes.
   ------------------------------------------------------------------------- */

const uWord cfNavmBackendImmLiLen  = 64;
const uWord cfNavmBackendImmLdLen  = 32;
const uWord cfNavmBackendImmStLen  = 32;
const uWord cfNavmBackendImmCpLen  = 8;
const uWord cfNavmBackendImmAddLen = 32;
const uWord cfNavmBackendImmMulLen = 32;
const uWord cfNavmBackendImmDivLen = 32;
const uWord cfNavmBackendImmAndLen = 32;
const uWord cfNavmBackendImmGorLen = 32;
const uWord cfNavmBackendImmXorLen = 32;
const uWord cfNavmBackendImmShlLen = 8;
const uWord cfNavmBackendImmShrLen = 8;

/* -------------------------------------------------------------------------
   INFO: These are needed characteristics of the processor architecture,
   all must be defined!
   ------------------------------------------------------------------------- */

enum {navmArchOutOfOrderRegShed, navmArchOutOfOrder};

const uByte  cNavmBackendMinImm   = 1;
const uByte  cNavmBackendMaxImm   = 4;
const char  *cNavmBackendCpuType  = "CISC";
const uByte  cNavmBackendArchType = navmArchOutOfOrderRegShed;

/* -------------------------------------------------------------------------
   INFO: Following functions representing the 16 base instructions of the
   NAVM Level 1 instruction set. All declarations are fixed and must exist
   because there extending the backend interface! For the immediate-direct
   versions this applies only to the variants which constant defination is
   set to true.
   ------------------------------------------------------------------------- */

void navmBackendCompLI (pNavmBackend h, sWord val)
  {switch (h->vD)
     {case 1: {compDisAsmImm64 (h, "MOV  RAX, ", val);
               compCode (h, 0x48); compCode (h, 0xB8); compImm64 (h, val);
               compIncD (h);
               break;}
      case 2: {compDisAsmImm64 (h, "MOV  R8,  ", val);
               compCode (h, 0x49); compCode (h, 0xB8); compImm64 (h, val);
               compIncD (h);
               break;}
      case 3: {compDisAsmImm64 (h, "MOV  R9,  ", val);
               compCode (h, 0x49); compCode (h, 0xB9); compImm64 (h, val);
               compIncD (h);
               break;}
      case 4: {compDisAsmImm64 (h, "MOV  R10, ", val);
               compCode (h, 0x49); compCode (h, 0xBA); compImm64 (h, val);
               compIncD (h);
               break;}
      case 5: {compDisAsmImm64 (h, "MOV  R11, ", val);
               compCode (h, 0x49); compCode (h, 0xBB); compImm64 (h, val);
               compIncD (h);
               break;}
      case 6: {compDisAsmImm64 (h, "MOV  R12, ", val);
               compCode (h, 0x49); compCode (h, 0xBC); compImm64 (h, val);
               compIncD (h);
               break;}
      case 7: {compDisAsmImm64 (h, "MOV  R13, ", val);
               compCode (h, 0x49); compCode (h, 0xBD); compImm64 (h, val);
               compIncD (h);
               break;}
      case 8: {compDisAsmImm64 (h, "MOV  R14, ", val);
               compCode (h, 0x49); compCode (h, 0xBE); compImm64 (h, val);
               compIncD (h);
               break;}
      default: errorHalt ("[navmBackendCompLI] vD > 8 | vD < 1"); }}

void navmBackendCompLD (pNavmBackend h)
  {compDecD (h); compDecD (h);
   switch (h->vD)
     {case 1: {compDisAsm (h, "MOV  RAX, [RAX]");
               compCode (h, 0x48); compCode (h, 0x8B); compCode (h, 0x00);
               compIncD (h);
               break;}
      case 2: {compDisAsm (h, "MOV  R8,  [R8]");
               compCode (h, 0x4D); compCode (h, 0x8B); compCode (h, 0x00);
               compIncD (h);
               break;}
      case 3: {compDisAsm (h, "MOV  R9,  [R9]");
               compCode (h, 0x4D); compCode (h, 0x8B); compCode (h, 0x09);
               compIncD (h);
               break;}
      case 4: {compDisAsm (h, "MOV  R10, [R10]");
               compCode (h, 0x4D); compCode (h, 0x8B); compCode (h, 0x12);
               compIncD (h);
               break;}
      case 5: {compDisAsm (h, "MOV  R11, [R11]");
               compCode (h, 0x4D); compCode (h, 0x8B); compCode (h, 0x1B);
               compCode (h, 0x24);
               compIncD (h);
               break;}
      case 6: {compDisAsm (h, "MOV  R12, [R12]");
               compCode (h, 0x4D); compCode (h, 0x8B); compCode (h, 0x24);
               compCode (h, 0x24); compIncD (h);
               break;}
      case 7: {compDisAsm (h, "MOV  R13, [R13]");
               compCode (h, 0x4D); compCode (h, 0x8B); compCode (h, 0x6D);
               compCode (h, 0x00); compIncD (h);
               break;}
      default: errorHalt ("[navmBackendCompLD] vD > 7 | vD < 1");}}

void navmBackendCompST (pNavmBackend h)
  {compDecD (h); compDecD (h);
   switch (h->vD)
     {case 2: {compDisAsm (h, "MOV  [R9],  R8");
               compCode (h, 0x4D); compCode (h, 0x89); compCode (h, 0x01);
               compDecD (h);       compDecD (h);
               break;}
      case 3: {compDisAsm (h, "MOV  [R10], R9");
               compCode (h, 0x4D); compCode (h, 0x89); compCode (h, 0x0A);
               compDecD (h);       compDecD (h);
               break;}
      case 4: {compDisAsm (h, "MOV  [R11], R10");
               compCode (h, 0x4D); compCode (h, 0x89); compCode (h, 0x13);
               compDecD (h);       compDecD (h);
               break;}
      case 5: {compDisAsm (h, "MOV  [R12], R11");
               compCode (h, 0x4D); compCode (h, 0x89); compCode (h, 0x1C);
               compCode (h, 0x24); compDecD (h);
               compDecD (h);       compDecD (h);
               break;}
      case 6: {compDisAsm (h, "MOV  [R13], R12");
               compCode (h, 0x4D); compCode (h, 0x89); compCode (h, 0x65);
               compCode (h, 0x00); compDecD (h);
               compDecD (h);       compDecD (h);
               break;}
      case 7: {compDisAsm (h, "MOV  [R14], R13");
               compCode (h, 0x4D); compCode (h, 0x89); compCode (h, 0x2E);
               compDecD (h);       compDecD (h);
               break;}
      default: errorHalt ("[navmBackendCompST] vD > 7 | vD < 2");}}

void navmBackendCompCP (pNavmBackend h)
  {compDecD (h); compDecD (h);
   switch (h->vD)
     {case 1: {compDisAsm (h, "CMP  RAX, R8");
               compCode (h, 0x4C); compCode (h, 0x39); compCode (h, 0xC0);
               compIncD (h);
               break;}
      case 2: {compDisAsm (h, "CMP  R8,  R9");
               compCode (h, 0x4D); compCode (h, 0x39); compCode (h, 0xC8);
               compIncD (h);
               break;}
      case 3: {compDisAsm (h, "CMP  R9,  R10");
               compCode (h, 0x4D); compCode (h, 0x39); compCode (h, 0xD1);
               compIncD (h);
               break;}
      case 4: {compDisAsm (h, "CMP  R10, R11");
               compCode (h, 0x4D); compCode (h, 0x39); compCode (h, 0xDA);
               compIncD (h);
               break;}
      case 5: {compDisAsm (h, "CMP  R11, R12");
               compCode (h, 0x4D); compCode (h, 0x39); compCode (h, 0xE3);
               compIncD (h);
               break;}
      case 6: {compDisAsm (h, "CMP  R12, R13");
               compCode (h, 0x4D); compCode (h, 0x39); compCode (h, 0xED);
               compIncD (h);
               break;}
      case 7: {compDisAsm (h, "CMP  R13, R14");
               compCode (h, 0x4D); compCode (h, 0x39); compCode (h, 0xF5);
               compIncD (h);
               break;}
      default: errorHalt ("[navmBackendCompCP] vD > 7 | vD < 1");}}

void navmBackendCompCpImm (pNavmBackend h, int32_t val)
  {compDecD (h);
   switch (h->vD)
     {case 1: {compDisAsmImm32 (h, "CMP  RAX, ", val);
               compCode  (h, 0x48); compCode (h, 0x3D); compImm32 (h, val);
               compIncD (h);
               break;}
      case 2: {compDisAsmImm32 (h, "CMP  R8,  ", val);
               compCode  (h, 0x49); compCode (h, 0x81); compCode (h, 0xF8);
               compImm64 (h, val);
               compIncD (h);
               break;}
      case 3: {compDisAsmImm32 (h, "CMP  R9,  ", val);
               compCode  (h, 0x49); compCode (h, 0x81); compCode (h, 0xF9);
               compImm64 (h, val);
               compIncD (h);
               break;}
      case 4: {compDisAsmImm32 (h, "CMP  R10, ", val);
               compCode  (h, 0x49); compCode (h, 0x81); compCode (h, 0xFA);
               compImm64 (h, val);
               compIncD (h);
               break;}
      case 5: {compDisAsmImm32 (h, "CMP  R11, ", val);
               compCode  (h, 0x49); compCode (h, 0x81); compCode (h, 0xFB);
               compImm64 (h, val);
               compIncD (h);
               break;}
      case 6: {compDisAsmImm32 (h, "CMP  R12, ", val);
               compCode  (h, 0x49); compCode (h, 0x81); compCode (h, 0xFC);
               compImm64 (h, val);
               compIncD (h);
               break;}
      case 7: {compDisAsmImm32 (h, "CMP  R13, ", val);
               compCode  (h, 0x49); compCode (h, 0x81); compCode (h, 0xFD);
               compImm64 (h, val);
               compIncD (h);
               break;}
      default: errorHalt ("[navmBackendCompCpImm] vD > 7 | vD < 1");}}

void navmBackendCompADD (pNavmBackend h)
  {compDecD (h); compDecD (h);
   switch (h->vD)
     {case 1: {compDisAsm (h, "ADD  RAX, R8");
               compCode (h, 0x4C); compCode (h, 0x01); compCode (h, 0xC0);
               compIncD (h);
               break;}
      case 2: {compDisAsm (h, "ADD  R8,  R9");
               compCode (h, 0x4D); compCode (h, 0x01); compCode (h, 0xC8);
               compIncD (h);
               break;}
      case 3: {compDisAsm (h, "ADD  R9,  R10");
               compCode (h, 0x4D); compCode (h, 0x01); compCode (h, 0xD1);
               compIncD (h);
               break;}
      case 4: {compDisAsm (h, "ADD  R10, R11");
               compCode (h, 0x4D); compCode (h, 0x01); compCode (h, 0xDA);
               compIncD (h);
               break;}
      case 5: {compDisAsm (h, "ADD  R11, R12");
               compCode (h, 0x4D); compCode (h, 0x01); compCode (h, 0xE3);
               compIncD (h);
               break;}
      case 6: {compDisAsm (h, "ADD  R12, R13");
               compCode (h, 0x4D); compCode (h, 0x01); compCode (h, 0xEC);
               compIncD (h);
               break;}
      case 7: {compDisAsm (h, "ADD  R13, R14");
               compCode (h, 0x4D); compCode (h, 0x01); compCode (h, 0xF5);
               compIncD (h);
               break;}
      default: errorHalt ("[navmBackendCompADD] vD > 7 | vD < 1");}}

void navmBackendCompAddImm (pNavmBackend h, int32_t val)
  {compDecD (h);
   switch (h->vD)
     {case 1: {compDisAsmImm32 (h, "ADD  RAX, ", val);
               compCode  (h, 0x48); compCode (h, 0x05); compImm32 (h, val);
               compIncD (h);
               break;}
      case 2: {compDisAsmImm32 (h, "ADD  R8,  ", val);
               compCode  (h, 0x49); compCode (h, 0x81); compCode (h, 0xC0);
               compImm32 (h, val);
               compIncD (h);
               break;}
      case 3: {compDisAsmImm32 (h, "ADD  R9,  ", val);
               compCode  (h, 0x49); compCode (h, 0x81); compCode (h, 0xC1);
               compImm32 (h, val);
               compIncD (h);
               break;}
      case 4: {compDisAsmImm32 (h, "ADD  R10, ", val);
               compCode  (h, 0x49); compCode (h, 0x81); compCode (h, 0xC2);
               compImm32 (h, val);
               compIncD (h);
               break;}
      case 5: {compDisAsmImm32 (h, "ADD  R11, ", val);
               compCode  (h, 0x49); compCode (h, 0x81); compCode (h, 0xC3);
               compImm32 (h, val);
               compIncD (h);
               break;}
      case 6: {compDisAsmImm32 (h, "ADD  R12, ", val);
               compCode  (h, 0x49); compCode (h, 0x81); compCode (h, 0xC4);
               compImm32 (h, val);
               compIncD (h);
               break;}
      case 7: {compDisAsmImm32 (h, "ADD  R13, ", val);
               compCode  (h, 0x49); compCode (h, 0x81); compCode (h, 0xC5);
               compImm32 (h, val);
               compIncD (h);
               break;}
      default: errorHalt ("[navmBackendCompAddImm] vD > 7 | vD < 1");}}

void navmBackendCompDIV (pNavmBackend h)
  {compDecD (h); compDecD (h);
   switch (h->vD)
     {case 1: {compDisAsm (h, "XOR  RDX, RDX|IDIV R8");
               compCode (h, 0x48); compCode (h, 0x31); compCode (h, 0xD2);
               compCode (h, 0x49); compCode (h, 0xF7); compCode (h, 0xF8);
               compIncD (h);
               break;}
      case 2: {compDisAsm (h, "XOR  RDX, RDX|MOV R15, RAX|MOV RAX,  R8|IDIV R9");
               compDisAsm (h, "MOV  R8,  RAX|MOV RAX, R15");
               compCode (h, 0x48); compCode (h, 0x31); compCode (h, 0xD2);
               compCode (h, 0x49); compCode (h, 0x89); compCode (h, 0xC7);
               compCode (h, 0x4C); compCode (h, 0x89); compCode (h, 0xC0);
               compCode (h, 0x49); compCode (h, 0xF7); compCode (h, 0xF9);
               compCode (h, 0x49); compCode (h, 0x89); compCode (h, 0xC0);
               compCode (h, 0x4C); compCode (h, 0x89); compCode (h, 0xF8);
               compIncD (h);
               break;}
      case 3: {compDisAsm (h, "XOR  RDX, RDX|MOV R15, RAX|MOV RAX,  R9|IDIV R10");
               compDisAsm (h, "MOV  R9,  RAX|MOV RAX, R15");
               compCode (h, 0x48); compCode (h, 0x31); compCode (h, 0xD2);
               compCode (h, 0x49); compCode (h, 0x89); compCode (h, 0xC7);
               compCode (h, 0x4C); compCode (h, 0x89); compCode (h, 0xC8);
               compCode (h, 0x49); compCode (h, 0xF7); compCode (h, 0xFA);
               compCode (h, 0x49); compCode (h, 0x89); compCode (h, 0xC1);
               compCode (h, 0x4C); compCode (h, 0x89); compCode (h, 0xF8);
               compIncD (h);
               break;}
      case 4: {compDisAsm (h, "XOR  RDX, RDX|MOV R15, RAX|MOV RAX, R10|IDIV R11");
               compDisAsm (h, "MOV  R10, RAX|MOV RAX, R15");
               compCode (h, 0x48); compCode (h, 0x31); compCode (h, 0xD2);
               compCode (h, 0x49); compCode (h, 0x89); compCode (h, 0xC7);
               compCode (h, 0x4C); compCode (h, 0x89); compCode (h, 0xD0);
               compCode (h, 0x49); compCode (h, 0xF7); compCode (h, 0xFB);
               compCode (h, 0x49); compCode (h, 0x89); compCode (h, 0xC2);
               compCode (h, 0x4C); compCode (h, 0x89); compCode (h, 0xF8);
               compIncD (h);
               break;}
      case 5: {compDisAsm (h, "XOR  RDX, RDX|MOV R15, RAX|MOV RAX, R11|IDIV R12");
               compDisAsm (h, "MOV  R11, RAX|MOV RAX, R15");
               compCode (h, 0x48); compCode (h, 0x31); compCode (h, 0xD2);
               compCode (h, 0x49); compCode (h, 0x89); compCode (h, 0xC7);
               compCode (h, 0x4C); compCode (h, 0x89); compCode (h, 0xD8);
               compCode (h, 0x49); compCode (h, 0xF7); compCode (h, 0xFC);
               compCode (h, 0x49); compCode (h, 0x89); compCode (h, 0xC3);
               compCode (h, 0x4C); compCode (h, 0x89); compCode (h, 0xF8);
               compIncD (h);
               break;}
      case 6: {compDisAsm (h, "XOR  RDX, RDX|MOV R15, RAX|MOV RAX, R12|IDIV R13");
               compDisAsm (h, "MOV  R12, RAX|MOV RAX, R15");
               compCode (h, 0x48); compCode (h, 0x31); compCode (h, 0xD2);
               compCode (h, 0x49); compCode (h, 0x89); compCode (h, 0xC7);
               compCode (h, 0x4C); compCode (h, 0x89); compCode (h, 0xE0);
               compCode (h, 0x49); compCode (h, 0xF7); compCode (h, 0xFD);
               compCode (h, 0x49); compCode (h, 0x89); compCode (h, 0xC4);
               compCode (h, 0x4C); compCode (h, 0x89); compCode (h, 0xF8);
               compIncD (h);
               break;}
      case 7: {compDisAsm (h, "XOR  RDX, RDX|MOV R15, RAX|MOV RAX, R13|IDIV R14");
               compDisAsm (h, "MOV  R13, RAX|MOV RAX, R15");
               compCode (h, 0x48); compCode (h, 0x31); compCode (h, 0xD2);
               compCode (h, 0x49); compCode (h, 0x89); compCode (h, 0xC7);
               compCode (h, 0x4C); compCode (h, 0x89); compCode (h, 0xE8);
               compCode (h, 0x49); compCode (h, 0xF7); compCode (h, 0xFE);
               compCode (h, 0x49); compCode (h, 0x89); compCode (h, 0xC5);
               compCode (h, 0x4C); compCode (h, 0x89); compCode (h, 0xF8);
               compIncD (h);
               break;}
      default: errorHalt ("[navmBackendCompAddImm] vD > 7 | vD < 1");}}

void navmBackendCompDivImm (pNavmBackend h, sWord val)
  {errorHalt ("[navmBackendCompDivImm] !");}

void navmBackendCompMUL (pNavmBackend h)
  {compDecD (h); compDecD (h);
   switch (h->vD)
     {case 1: {compDisAsm (h, "IMUL RAX, R8");
               compCode (h, 0x49); compCode (h, 0x0F); compCode (h, 0xAF);
               compCode (h, 0xC0); compIncD (h);
               break;}
      case 2: {compDisAsm (h, "IMUL R8,  R9");
               compCode (h, 0x4D); compCode (h, 0x0F); compCode (h, 0xAF);
               compCode (h, 0xC1); compIncD (h);
               break;}
      case 3: {compDisAsm (h, "IMUL R9,  R10");
               compCode (h, 0x4D); compCode (h, 0x0F); compCode (h, 0xAF);
               compCode (h, 0xCA); compIncD (h);
               break;}
      case 4: {compDisAsm (h, "IMUL R10, R11");
               compCode (h, 0x4D); compCode (h, 0x0F); compCode (h, 0xAF);
               compCode (h, 0xD3); compIncD (h);
               break;}
      case 5: {compDisAsm (h, "IMUL R11, R12");
               compCode (h, 0x4D); compCode (h, 0x0F); compCode (h, 0xAF);
               compCode (h, 0xDC); compIncD (h);
               break;}
      case 6: {compDisAsm (h, "IMUL R12, R13");
               compCode (h, 0x4D); compCode (h, 0x0F); compCode (h, 0xAF);
               compCode (h, 0xE5); compIncD (h);
               break;}
      case 7: {compDisAsm (h, "IMUL R13, R14");
               compCode (h, 0x4D); compCode (h, 0x0F); compCode (h, 0xAF);
               compCode (h, 0xEE); compIncD (h);
               break;}
      default: errorHalt ("[navmBackendCompMUL] vD > 7 | vD < 1");}}

void navmBackendCompMulImm (pNavmBackend h, int32_t val)
  {compDecD (h);
   switch (h->vD)
     {case 1: {compDisAsmImm32 (h, "IMUL RAX, ", val);
               compCode  (h, 0x48); compCode (h, 0x69); compCode (h, 0xC0);
               compImm32 (h, val);
               compIncD (h);
               break;}
      case 2: {compDisAsmImm32 (h, "IMUL R8,  ", val);
               compCode  (h, 0x4D); compCode (h, 0x69); compCode (h, 0xC0);
               compImm32 (h, val);
               compIncD (h);
               break;}
      case 3: {compDisAsmImm32 (h, "IMUL R9,  ", val);
               compCode  (h, 0x4D); compCode (h, 0x69); compCode (h, 0xC9);
               compImm32 (h, val);
               compIncD (h);
               break;}
      case 4: {compDisAsmImm32 (h, "IMUL R10, ", val);
               compCode  (h, 0x4D); compCode (h, 0x69); compCode (h, 0xD2);
               compImm32 (h, val);
               compIncD (h);
               break;}
      case 5: {compDisAsmImm32 (h, "IMUL R11, ", val);
               compCode  (h, 0x4D); compCode (h, 0x69); compCode (h, 0xDB);
               compImm32 (h, val);
               compIncD (h);
               break;}
      case 6: {compDisAsmImm32 (h, "IMUL R12, ", val);
               compCode  (h, 0x4D); compCode (h, 0x69); compCode (h, 0xE4);
               compImm32 (h, val);
               compIncD (h);
               break;}
      case 7: {compDisAsmImm32 (h, "IMUL R13, ", val);
               compCode  (h, 0x4D); compCode (h, 0x69); compCode (h, 0xED);
               compImm32 (h, val);
               compIncD (h);
               break;}
      default: errorHalt ("[navmBackendCompMulImm] vD > 7 | vD < 1");}}

void navmBackendCompAND (pNavmBackend h)
  {compDecD (h); compDecD (h);
   switch (h->vD)
     {case 1: {compDisAsm (h, "AND  RAX, R8");
               compCode (h, 0x4C); compCode (h, 0x21); compCode (h, 0xC0);
               compIncD (h);
               break;}
      case 2: {compDisAsm (h, "AND  R8,  R9");
               compCode (h, 0x4D); compCode (h, 0x21); compCode (h, 0xC8);
               compIncD (h);
               break;}
      case 3: {compDisAsm (h, "AND  R9,  R10");
               compCode (h, 0x4D); compCode (h, 0x21); compCode (h, 0xD1);
               compIncD (h);
               break;}
      case 4: {compDisAsm (h, "AND  R10, R11");
               compCode (h, 0x4D); compCode (h, 0x21); compCode (h, 0xDA);
               compIncD (h);
               break;}
      case 5: {compDisAsm (h, "AND  R11, R12");
               compCode (h, 0x4D); compCode (h, 0x21); compCode (h, 0xE3);
               compIncD (h);
               break;}
      case 6: {compDisAsm (h, "AND  R12, R13");
               compCode (h, 0x4D); compCode (h, 0x21); compCode (h, 0xEC);
               compIncD (h);
               break;}
      case 7: {compDisAsm (h, "AND  R13, R14");
               compCode (h, 0x4D); compCode (h, 0x21); compCode (h, 0xF5);
               compIncD (h);
               break;}
      default: errorHalt ("[navmBackendCompAND] vD > 7 | vD < 1");}}

void navmBackendCompAndImm (pNavmBackend h, int32_t val)
  {compDecD (h);
   switch (h->vD)
     {case 1: {compDisAsmImm32 (h, "AND  RAX, ", val);
               compCode  (h, 0x48); compCode (h, 0x25); compImm32 (h, val);
               compIncD (h);
               break;}
      case 2: {compDisAsmImm32 (h, "AND  R8,  ", val);
               compCode  (h, 0x49); compCode (h, 0x81); compCode (h, 0xE0);
               compImm32 (h, val);
               compIncD (h);
               break;}
      case 3: {compDisAsmImm32 (h, "AND  R9,  ", val);
               compCode  (h, 0x49); compCode (h, 0x81); compCode (h, 0xE1);
               compImm32 (h, val);
               compIncD (h);
               break;}
      case 4: {compDisAsmImm32 (h, "AND  R10, ", val);
               compCode  (h, 0x49); compCode (h, 0x81); compCode (h, 0xE2);
               compImm32 (h, val);
               compIncD (h);
               break;}
      case 5: {compDisAsmImm32 (h, "AND  R11, ", val);
               compCode  (h, 0x49); compCode (h, 0x81); compCode (h, 0xE3);
               compImm32 (h, val);
               compIncD (h);
               break;}
      case 6: {compDisAsmImm32 (h, "AND  R12, ", val);
               compCode  (h, 0x49); compCode (h, 0x81); compCode (h, 0xE4);
               compImm32 (h, val);
               compIncD (h);
               break;}
      case 7: {compDisAsmImm32 (h, "AND  R13, ", val);
               compCode  (h, 0x49); compCode (h, 0x81); compCode (h, 0xE5);
               compImm32 (h, val);
               compIncD (h);
               break;}
      default: errorHalt ("[navmBackendCompAndImm] vD > 7 | vD < 1");}}

void navmBackendCompGOR (pNavmBackend h)
  {compDecD (h); compDecD (h);
   switch (h->vD)
     {case 1: {compDisAsm (h, "OR   RAX, R8");
               compCode (h, 0x4C); compCode (h, 0x09); compCode (h, 0xC0);
               compIncD (h);
               break;}
      case 2: {compDisAsm (h, "OR   R8,  R9");
               compCode (h, 0x4D); compCode (h, 0x09); compCode (h, 0xC8);
               compIncD (h);
               break;}
      case 3: {compDisAsm (h, "OR   R9,  R10");
               compCode (h, 0x4D); compCode (h, 0x09); compCode (h, 0xD1);
               compIncD (h);
               break;}
      case 4: {compDisAsm (h, "OR   R10, R11");
               compCode (h, 0x4D); compCode (h, 0x09); compCode (h, 0xDA);
               compIncD (h);
               break;}
      case 5: {compDisAsm (h, "OR   R11, R12");
               compCode (h, 0x4D); compCode (h, 0x09); compCode (h, 0xE3);
               compIncD (h);
               break;}
      case 6: {compDisAsm (h, "OR   R12, R13");
               compCode (h, 0x4D); compCode (h, 0x09); compCode (h, 0xEC);
               compIncD (h);
               break;}
      case 7: {compDisAsm (h, "OR   R13, R14");
               compCode (h, 0x4D); compCode (h, 0x09); compCode (h, 0xF5);
               compIncD (h);
               break;}
      default: errorHalt ("[navmBackendCompGOR] vD > 7 | vD < 1");}}

void navmBackendCompGorImm (pNavmBackend h, int32_t val)
  {compDecD (h);
   switch (h->vD)
     {case 1: {compDisAsmImm32 (h, "OR   RAX, ", val);
               compCode (h, 0x48); compCode (h, 0x0D); compImm32 (h, val);
               compIncD (h);
               break;}
      case 2: {compDisAsmImm32 (h, "OR   R8,  ", val);
               compCode  (h, 0x49); compCode (h, 0x81); compCode (h, 0xC8);
               compImm32 (h, val);
               compIncD  (h);
               break;}
      case 3: {compDisAsmImm32 (h, "OR   R9,  ", val);
               compCode  (h, 0x49); compCode (h, 0x81); compCode (h, 0xC9);
               compImm32 (h, val);
               compIncD  (h);
               break;}
      case 4: {compDisAsmImm32 (h, "OR   R10, ", val);
               compCode  (h, 0x49); compCode (h, 0x81); compCode (h, 0xCA);
               compImm32 (h, val);
               compIncD  (h);
               break;}
      case 5: {compDisAsmImm32 (h, "OR   R11, ", val);
               compCode  (h, 0x49); compCode (h, 0x81); compCode (h, 0xCB);
               compImm32 (h, val);
               compIncD  (h);
               break;}
      case 6: {compDisAsmImm32 (h, "OR   R12, ", val);
               compCode  (h, 0x49); compCode (h, 0x81); compCode (h, 0xCC);
               compImm32 (h, val);
               compIncD  (h);
               break;}
      case 7: {compDisAsmImm32 (h, "OR   R13, ", val);
               compCode  (h, 0x49); compCode (h, 0x81); compCode (h, 0xCD);
               compImm32 (h, val);
               compIncD  (h);
               break;}
      default: errorHalt ("[navmBackendCompGorImm] vD > 7 | vD < 1");}}

void navmBackendCompXOR (pNavmBackend h)
  {compDecD (h); compDecD (h);
   switch (h->vD)
     {case 1: {compDisAsm (h, "XOR  RAX, R8");
               compCode (h, 0x4C); compCode (h, 0x31); compCode (h, 0xC0);
               compIncD (h);
               break;}
      case 2: {compDisAsm (h, "XOR  R8,  R9");
               compCode (h, 0x4D); compCode (h, 0x31); compCode (h, 0xC8);
               compIncD (h);
               break;}
      case 3: {compDisAsm (h, "XOR  R9,  R10");
               compCode (h, 0x4D); compCode (h, 0x31); compCode (h, 0xD1);
               compIncD (h);
               break;}
      case 4: {compDisAsm (h, "XOR  R10, R11");
               compCode (h, 0x4D); compCode (h, 0x31); compCode (h, 0xDA);
               compIncD (h);
               break;}
      case 5: {compDisAsm (h, "XOR  R11, R12");
               compCode (h, 0x4D); compCode (h, 0x31); compCode (h, 0xE3);
               compIncD (h);
               break;}
      case 6: {compDisAsm (h, "XOR  R12, R13");
               compCode (h, 0x4D); compCode (h, 0x31); compCode (h, 0xEC);
               compIncD (h);
               break;}
      case 7: {compDisAsm (h, "XOR  R13, R14");
               compCode (h, 0x4D); compCode (h, 0x31); compCode (h, 0xF5);
               compIncD (h);
               break;}
      default: errorHalt ("[navmBackendCompXOR] vD > 7 | vD < 1");}}

void navmBackendCompXorImm (pNavmBackend h, int32_t val)
  {compDecD (h);
   switch (h->vD)
     {case 1: {compDisAsmImm32 (h, "XOR  RAX, ", val);
               compCode (h, 0x48); compCode (h, 0x35); compImm32 (h, val);
               compIncD (h);
               break;}
      case 2: {compDisAsmImm32 (h, "XOR  R8,  ", val);
               compCode  (h, 0x49); compCode (h, 0x81); compCode (h, 0xF0);
               compImm32 (h, val);
               compIncD  (h);
               break;}
      case 3: {compDisAsmImm32 (h, "XOR  R9,  ", val);
               compCode  (h, 0x49); compCode (h, 0x81); compCode (h, 0xF1);
               compImm32 (h, val);
               compIncD  (h);
               break;}
      case 4: {compDisAsmImm32 (h, "XOR  R10, ", val);
               compCode  (h, 0x49); compCode (h, 0x81); compCode (h, 0xF2);
               compImm32 (h, val);
               compIncD  (h);
               break;}
      case 5: {compDisAsmImm32 (h, "XOR  R11, ", val);
               compCode  (h, 0x49); compCode (h, 0x81); compCode (h, 0xF3);
               compImm32 (h, val);
               compIncD  (h);
               break;}
      case 6: {compDisAsmImm32 (h, "XOR  R12, ", val);
               compCode  (h, 0x49); compCode (h, 0x81); compCode (h, 0xF4);
               compImm32 (h, val);
               compIncD  (h);
               break;}
      case 7: {compDisAsmImm32 (h, "XOR  R13, ", val);
               compCode  (h, 0x49); compCode (h, 0x81); compCode (h, 0xF5);
               compImm32 (h, val);
               compIncD  (h);
               break;}
      default: errorHalt ("[navmBackendCompXorImm] vD > 7 | vD < 1");}}

void navmBackendCompSHL (pNavmBackend h)
  {compDecD (h); compDecD (h);
   switch (h->vD)
     {case 1: {compDisAsm (h, "MOV  CL,  R8B|SHL RAX, CL");
               compCode (h, 0x44); compCode (h, 0x88); compCode (h, 0xC1);
               compCode (h, 0x48); compCode (h, 0xD3); compCode (h, 0xE0);
               compIncD (h);
               break;}
      case 2: {compDisAsm (h, "MOV  CL,  R9B|SHL R8, CL");
               compCode (h, 0x44); compCode (h, 0x88); compCode (h, 0xC9);
               compCode (h, 0x49); compCode (h, 0xD3); compCode (h, 0xE0);
               compIncD (h);
               break;}
      case 3: {compDisAsm (h, "MOV  CL,  R10B|SHL R9, CL");
               compCode (h, 0x44); compCode (h, 0x88); compCode (h, 0xD1);
               compCode (h, 0x49); compCode (h, 0xD3); compCode (h, 0xE1);
               compIncD (h);
               break;}
      case 4: {compDisAsm (h, "MOV  CL,  R11B|SHL R10, CL");
               compCode (h, 0x44); compCode (h, 0x88); compCode (h, 0xD9);
               compCode (h, 0x49); compCode (h, 0xD3); compCode (h, 0xE2);
               compIncD (h);
               break;}
      case 5: {compDisAsm (h, "MOV  CL,  R12B|SHL R11, CL");
               compCode (h, 0x44); compCode (h, 0x88); compCode (h, 0xE1);
               compCode (h, 0x49); compCode (h, 0xD3); compCode (h, 0xE3);
               compIncD (h);
               break;}
      case 6: {compDisAsm (h, "MOV  CL,  R13B|SHL R12, CL");
               compCode (h, 0x44); compCode (h, 0x88); compCode (h, 0xE9);
               compCode (h, 0x49); compCode (h, 0xD3); compCode (h, 0xE4);
               compIncD (h);
               break;}
      case 7: {compDisAsm (h, "MOV  CL,  R14B|SHL R13, CL");
               compCode (h, 0x44); compCode (h, 0x88); compCode (h, 0xF1);
               compCode (h, 0x49); compCode (h, 0xD3); compCode (h, 0xE5);
               compIncD (h);
               break;}
      default: errorHalt ("[navmBackendCompSHL] vD > 7 | vD < 1");}}

void navmBackendCompShlImm (pNavmBackend h, uint8_t val)
  {compDecD (h);
   switch (h->vD)
     {case 1: {compDisAsmImm8 (h, "SHL  RAX, ", val);
               compCode (h, 0x48); compCode (h, 0xC1); compCode (h, 0xE0);
               compCode (h, val);
               compIncD (h);
               break;}
      case 2: {compDisAsmImm8 (h, "SHL  R8,  ", val);
               compCode (h, 0x49); compCode (h, 0xC1); compCode (h, 0xE0);
               compCode (h, val);
               compIncD (h);
               break;}
      case 3: {compDisAsmImm8 (h, "SHL  R9,  ", val);
               compCode (h, 0x49); compCode (h, 0xC1); compCode (h, 0xE1);
               compCode (h, val);
               compIncD (h);
               break;}
      case 4: {compDisAsmImm8 (h, "SHL  R10, ", val);
               compCode (h, 0x49); compCode (h, 0xC1); compCode (h, 0xE2);
               compCode (h, val);
               compIncD (h);
               break;}
      case 5: {compDisAsmImm8 (h, "SHL  R11, ", val);
               compCode (h, 0x49); compCode (h, 0xC1); compCode (h, 0xE3);
               compCode (h, val);
               compIncD (h);
               break;}
      case 6: {compDisAsmImm8 (h, "SHL  R12, ", val);
               compCode (h, 0x49); compCode (h, 0xC1); compCode (h, 0xE4);
               compCode (h, val);
               compIncD (h);
               break;}
      case 7: {compDisAsmImm8 (h, "SHL  R13, ", val);
               compCode (h, 0x49); compCode (h, 0xC1); compCode (h, 0xE5);
               compCode (h, val);
               compIncD (h);
               break;}
      default: errorHalt ("[navmBackendCompShlImm] vD > 7 | vD < 1");}}

void navmBackendCompSHR (pNavmBackend h)
  {compDecD (h); compDecD (h);
   switch (h->vD)
     {case 1: {compDisAsm (h, "MOV  CL,  R8B|SHR RAX, CL");
               compCode (h, 0x44); compCode (h, 0x88); compCode (h, 0xC1);
               compCode (h, 0x48); compCode (h, 0xD3); compCode (h, 0xE8);
               compIncD (h);
               break;}
      case 2: {compDisAsm (h, "MOV  CL,  R9B|SHR R8, CL");
               compCode (h, 0x44); compCode (h, 0x88); compCode (h, 0xC9);
               compCode (h, 0x49); compCode (h, 0xD3); compCode (h, 0xE8);
               compIncD (h);
               break;}
      case 3: {compDisAsm (h, "MOV  CL,  R10B|SHR R9, CL");
               compCode (h, 0x44); compCode (h, 0x88); compCode (h, 0xD1);
               compCode (h, 0x49); compCode (h, 0xD3); compCode (h, 0xE9);
               compIncD (h);
               break;}
      case 4: {compDisAsm (h, "MOV  CL,  R11B|SHR R10, CL");
               compCode (h, 0x44); compCode (h, 0x88); compCode (h, 0xD9);
               compCode (h, 0x49); compCode (h, 0xD3); compCode (h, 0xEA);
               compIncD (h);
               break;}
      case 5: {compDisAsm (h, "MOV  CL,  R12B|SHR R11, CL");
               compCode (h, 0x44); compCode (h, 0x88); compCode (h, 0xE1);
               compCode (h, 0x49); compCode (h, 0xD3); compCode (h, 0xEB);
               compIncD (h);
               break;}
      case 6: {compDisAsm (h, "MOV  CL,  R13B|SHR R12, CL");
               compCode (h, 0x44); compCode (h, 0x88); compCode (h, 0xE9);
               compCode (h, 0x49); compCode (h, 0xD3); compCode (h, 0xEC);
               compIncD (h);
               break;}
      case 7: {compDisAsm (h, "MOV  CL,  R14B|SHR R13, CL");
               compCode (h, 0x44); compCode (h, 0x88); compCode (h, 0xF1);
               compCode (h, 0x49); compCode (h, 0xD3); compCode (h, 0xED);
               compIncD (h);
               break;}
      default: errorHalt ("[navmBackendCompSHR] vD > 7 | vD < 1");}}

void navmBackendCompShrImm (pNavmBackend h, uint8_t val)
  {compDecD (h);
   switch (h->vD)
     {case 1: {compDisAsmImm8 (h, "SHR  RAX, ", val);
               compCode (h, 0x48); compCode (h, 0xC1); compCode (h, 0xE8);
               compCode (h, val);
               compIncD (h);
               break;}
      case 2: {compDisAsmImm8 (h, "SHR  R8,  ", val);
               compCode (h, 0x49); compCode (h, 0xC1); compCode (h, 0xE8);
               compCode (h, val);
               compIncD (h);
               break;}
      case 3: {compDisAsmImm8 (h, "SHR  R9,  ", val);
               compCode (h, 0x49); compCode (h, 0xC1); compCode (h, 0xE9);
               compCode (h, val);
               compIncD (h);
               break;}
      case 4: {compDisAsmImm8 (h, "SHR  R10, ", val);
               compCode (h, 0x49); compCode (h, 0xC1); compCode (h, 0xEA);
               compCode (h, val);
               compIncD (h);
               break;}
      case 5: {compDisAsmImm8 (h, "SHR  R11, ", val);
               compCode (h, 0x49); compCode (h, 0xC1); compCode (h, 0xEB);
               compCode (h, val);
               compIncD (h);
               break;}
      case 6: {compDisAsmImm8 (h, "SHR  R12, ", val);
               compCode (h, 0x49); compCode (h, 0xC1); compCode (h, 0xEC);
               compCode (h, val);
               compIncD (h);
               break;}
      case 7: {compDisAsmImm8 (h, "SHR  R13, ", val);
               compCode (h, 0x49); compCode (h, 0xC1); compCode (h, 0xED);
               compCode (h, val);
               compIncD (h);
               break;}
      default: errorHalt ("[navmBackendCompShrImm] vD > 7 | vD < 1");}}

void navmBackendCompDUP (pNavmBackend h)
  {switch (h->vD)
     {case 1: {compDisAsm (h, "MOV  R8,  RAX");
               compCode (h, 0x49); compCode (h, 0x89); compCode (h, 0xC0);
               compIncD (h);
               break;}
      case 2: {compDisAsm (h, "MOV  R9,  R8");
               compCode (h, 0x4D); compCode (h, 0x89); compCode (h, 0xC1);
               compIncD (h);
               break;}
      case 3: {compDisAsm (h, "MOV  R10, R9");
               compCode (h, 0x4D); compCode (h, 0x89); compCode (h, 0xCA);
               compIncD (h);
               break;}
      case 4: {compDisAsm (h, "MOV  R11, R10");
               compCode (h, 0x4D); compCode (h, 0x89); compCode (h, 0xD3);
               compIncD (h);
               break;}
      case 5: {compDisAsm (h, "MOV  R12, R11");
               compCode (h, 0x4D); compCode (h, 0x89); compCode (h, 0xDC);
               compIncD (h);
               break;}
      case 6: {compDisAsm (h, "MOV  R13, R12");
               compCode (h, 0x4D); compCode (h, 0x89); compCode (h, 0xE5);
               compIncD (h);
               break;}
      case 7: {compDisAsm (h, "MOV  R14, R13");
               compCode (h, 0x4D); compCode (h, 0x89); compCode (h, 0xEE);
               compIncD (h);
               break;}
      default: errorHalt ("[navmBackendCompDUP] vD > 7 | vD < 1");}}

void navmBackendCompDRP (pNavmBackend h) {compDecD (h);}

void navmBackendCompSWP (pNavmBackend h)
  {switch (h->vD)
     {case 1: {compDisAsm (h, "XCHG RAX, R8");
               compCode (h, 0x49); compCode (h, 0x90);
               break;}
      case 2: {compDisAsm (h, "XCHG R8,  R9");
               compCode (h, 0x4D); compCode (h, 0x87); compCode (h, 0xC8);
               break;}
      case 3: {compDisAsm (h, "XCHG R9,  R10");
               compCode (h, 0x4D); compCode (h, 0x87); compCode (h, 0xD1);
               break;}
      case 4: {compDisAsm (h, "XCHG R10, R11");
               compCode (h, 0x4D); compCode (h, 0x87); compCode (h, 0xDA);
               break;}
      case 5: {compDisAsm (h, "XCHG R11, R12");
               compCode (h, 0x4D); compCode (h, 0x87); compCode (h, 0xE3);
               break;}
      case 6: {compDisAsm (h, "XCHG R12, R13");
               compCode (h, 0x4D); compCode (h, 0x87); compCode (h, 0xEC);
               break;}
      case 7: {compDisAsm (h, "XCHG R13, R14");
               compCode (h, 0x4D); compCode (h, 0x87); compCode (h, 0xF5);
               break;}
      default: errorHalt ("[navmBackendCompSWP] vD > 7 | vD < 1");}}

void navmBackendCompROT (pNavmBackend h)
  {}

void  navmBackendExecute (pNavmBackend h, uWord ofs)
  {navmBackendTempAdr = h->pMem + ofs;

   asm volatile (".byte 0xCC");

   asm volatile ("push %rcx");
   asm volatile ("push %rdx");
   asm volatile ("push %r8");
   asm volatile ("push %r9");
   asm volatile ("push %r10");
   asm volatile ("push %r11");
   asm volatile ("push %r12");
   asm volatile ("push %r13");
   asm volatile ("push %r14");
   asm volatile ("push %r15");

   asm volatile ("mov  navmBackendTempAdr, %rax");
   asm volatile ("call *%rax");

   asm volatile ("pop  %r15");
   asm volatile ("pop  %r14");
   asm volatile ("pop  %r13");
   asm volatile ("pop  %r12");
   asm volatile ("pop  %r11");
   asm volatile ("pop  %r10");
   asm volatile ("pop  %r9");
   asm volatile ("pop  %r8");
   asm volatile ("pop  %rdx");
   asm volatile ("pop  %rcx");}
