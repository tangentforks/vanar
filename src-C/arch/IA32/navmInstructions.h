/*****************************************************************************
  PROJECT:     NAVM AOT COMPILER
  DESCRIPTION: COMPILER FOR THE NAVM LEVEL 1 ISA AS IL REPRESENTATION
  INFO:        ALL BASIC NAVM LEVEL 1 INSTRUCTIONs FOR IA32 COMP. PROCESSORS
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
        *(handle->pMem + handle->oMem++) = (uByte) code;
   else errorHalt ("[compByte] oMem > cMemSize");}

void compImm32 (pNavmBackend handle, int32_t value)
  {uImm32 imm; imm.sImm = value;
   if (handle->oMem < handle->cMemSize)
       {*(handle->pMem + handle->oMem++) = (uByte) imm.aSlices[0];
        *(handle->pMem + handle->oMem++) = (uByte) imm.aSlices[1];
        *(handle->pMem + handle->oMem++) = (uByte) imm.aSlices[2];
        *(handle->pMem + handle->oMem++) = (uByte) imm.aSlices[3];}
   else errorHalt ("[compImm32] oMem > cMemSize");}

void compIncD (pNavmBackend handle) {handle->vD++;}

void compDecD (pNavmBackend handle)
  {if (handle->vD > 0) handle->vD--;
      else errorHalt ("[compDecD] vD < 1");}

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

const uWord cfNavmBackendImmLiLen  = 32;
const uWord cfNavmBackendImmLdLen  = 32;
const uWord cfNavmBackendImmStLen  = 32;
const uWord cfNavmBackendImmCpLen  = 32;
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
     {case 1: {compDisAsmImm32 (h, "MOV  EAX, ", val);
               compCode (h, 0xB8); compImm32 (h, val); compIncD (h);
               break;}
      case 2: {compDisAsmImm32 (h, "MOV  ECX, ", val);
               compCode (h, 0xB9); compImm32 (h, val); compIncD (h);
               break;}
      case 3: {compDisAsmImm32 (h, "MOV  EDX, ", val);
               compCode (h, 0xBA); compImm32 (h, val); compIncD (h);
               break;}
      case 4: {compDisAsmImm32 (h, "MOV  EBX, ", val);
               compCode (h, 0xBB); compImm32 (h, val); compIncD (h);
               break;}
      case 5: {compDisAsmImm32 (h, "MOV  ESP, ", val);
               compCode (h, 0xBC); compImm32 (h, val); compIncD (h);
               break;}
      case 6: {compDisAsmImm32 (h, "MOV  EBP, ", val);
               compCode (h, 0xBD); compImm32 (h, val); compIncD (h);
               break;}
      case 7: {compDisAsmImm32 (h, "MOV  ESI, ", val);
               compCode (h, 0xBE); compImm32 (h, val); compIncD (h);
               break;}
      case 8: {compDisAsmImm32 (h, "MOV  EDI, ", val);
               compCode (h, 0xBF); compImm32 (h, val); compIncD (h);
               break;}
      default: errorHalt ("[navmBackendCompLI] vD > 8 | vD < 1"); }}

void navmBackendCompLD (pNavmBackend h)
  {compDecD (h); compDecD (h);
   switch (h->vD)
     {case 1: {compDisAsm (h, "MOV  EAX, [ECX]");
               compCode (h, 0x8B); compCode (h, 0x01); compIncD (h);
               break;}
      case 2: {compDisAsm (h, "MOV  ECX, [EDX]");
               compCode (h, 0x8B); compCode (h, 0x0A); compIncD (h);
               break;}
      case 3: {compDisAsm (h, "MOV  EDX, [EBX]");
               compCode (h, 0x8B); compCode (h, 0x13); compIncD (h);
               break;}
      case 4: {compDisAsm (h, "MOV  EBX, [ESP]");
               compCode (h, 0x8B); compCode (h, 0x1C); compCode (h, 0x24);
               compIncD (h);
               break;}
      case 5: {compDisAsm (h, "MOV  ESP, [EBP]");
               compCode (h, 0x8B); compCode (h, 0x65); compCode (h, 0x00);
               compIncD (h);
               break;}
      case 6: {compDisAsm (h, "MOV  EBP, [ESI]");
               compCode (h, 0x8B); compCode (h, 0x2E);
               compIncD (h);
               break;}
      case 7: {compDisAsm (h, "MOV  ESI, [EDI]");
               compCode (h, 0x8B); compCode (h, 0x37);
               compIncD (h);
               break;}
      default: errorHalt ("[navmBackendCompLD] vD > 7 | vD < 1");}}

void navmBackendCompST (pNavmBackend h)
  {compDecD (h); compDecD (h);
   switch (h->vD)
     {case 2: {compDisAsm (h, "MOV  [EDX], ECX");
               compCode (h, 0x89); compCode (h, 0x0A); compDecD (h);
               compDecD (h);
               break;}
      case 3: {compDisAsm (h, "MOV  [EBX], EDX");
               compCode (h, 0x89); compCode (h, 0x13); compDecD (h);
               compDecD (h);
               break;}
      case 4: {compDisAsm (h, "MOV  [EDX], ESP");
               compCode (h, 0x89); compCode (h, 0x22); compDecD (h);
               compDecD (h);
               break;}
      case 5: {compDisAsm (h, "MOV  [ESP], EBP");
               compCode (h, 0x89); compCode (h, 0x2C); compCode (h, 0x24);
               compDecD (h);       compDecD (h);
               break;}
      case 6: {compDisAsm (h, "MOV  [EBP], ESI");
               compCode (h, 0x89); compCode (h, 0x75); compCode (h, 0x00);
               compDecD (h);       compDecD (h);
               break;}
      case 7: {compDisAsm (h, "MOV  [ESI], EDI");
               compCode (h, 0x89); compCode (h, 0x3E); compDecD (h);
               compDecD (h);
               break;}
      default: errorHalt ("[navmBackendCompST] vD > 7 | vD < 2");}}

void navmBackendCompCP (pNavmBackend h)
  {compDecD (h); compDecD (h);
   switch (h->vD)
     {case 1: {compDisAsm (h, "CMP  EAX, ECX");
               compCode (h, 0x39); compCode (h, 0xC8); compIncD (h);
               break;}
      case 2: {compDisAsm (h, "CMP  ECX, EDX");
               compCode (h, 0x39); compCode (h, 0xD1); compIncD (h);
               break;}
      case 3: {compDisAsm (h, "CMP  EDX, EBX");
               compCode (h, 0x39); compCode (h, 0xDA); compIncD (h);
               break;}
      case 4: {compDisAsm (h, "CMP  EBX, ESP");
               compCode (h, 0x39); compCode (h, 0xE3); compIncD (h);
               break;}
      case 5: {compDisAsm (h, "CMP  ESP, EBP");
               compCode (h, 0x39); compCode (h, 0xEC); compIncD (h);
               break;}
      case 6: {compDisAsm (h, "CMP  EBP, ESI");
               compCode (h, 0x39); compCode (h, 0xF5); compIncD (h);
               break;}
      case 7: {compDisAsm (h, "CMP  ESI, EDI");
               compCode (h, 0x39); compCode (h, 0xFE); compIncD (h);
               break;}
      default: errorHalt ("[navmBackendCompCP] vD > 7 | vD < 1");}}

void navmBackendCompCpImm (pNavmBackend h, int32_t val)
  {compDecD (h);
   switch (h->vD)
     {case 1: {compDisAsmImm32 (h, "CMP  EAX, ", val);
               compCode (h, 0x3D); compImm32 (h, val); compIncD (h);
               break;}
      case 2: {compDisAsmImm32 (h, "CMP  ECX, ", val);
               compCode (h, 0x81); compCode (h, 0xF9); compImm32 (h, val);
               compIncD (h);
               break;}
      case 3: {compDisAsmImm32 (h, "CMP  EDX, ", val);
               compCode (h, 0x81); compCode (h, 0xFA); compImm32 (h, val);
               compIncD (h);
               break;}
      case 4: {compDisAsmImm32 (h, "CMP  EBX, ", val);
               compCode (h, 0x81); compCode (h, 0xFB); compImm32 (h, val);
               compIncD (h);
               break;}
      case 5: {compDisAsmImm32 (h, "CMP  ESP, ", val);
               compCode (h, 0x81); compCode (h, 0xFC); compImm32 (h, val);
               compIncD (h);
               break;}
      case 6: {compDisAsmImm32 (h, "CMP  EBP, ", val);
               compCode (h, 0x81); compCode (h, 0xFD); compImm32 (h, val);
               compIncD (h);
               break;}
      case 7: {compDisAsmImm32 (h, "CMP  ESI, ", val);
               compCode (h, 0x81); compCode (h, 0xFE); compImm32 (h, val);
               compIncD (h);
               break;}
      default: errorHalt ("[navmBackendCompCpImm] vD > 7 | vD < 1");}}

void navmBackendCompADD (pNavmBackend h)
  {compDecD (h); compDecD (h);
   switch (h->vD)
     {case 1: {compDisAsm (h, "ADD  EAX, ECX");
               compCode (h, 0x01); compCode (h, 0xC8); compIncD (h);
               break;}
      case 2: {compDisAsm (h, "ADD  ECX, EDX");
               compCode (h, 0x01); compCode (h, 0xD1); compIncD (h);
               break;}
      case 3: {compDisAsm (h, "ADD  EDX, EBX");
               compCode (h, 0x01); compCode (h, 0xDA); compIncD (h);
               break;}
      case 4: {compDisAsm (h, "ADD  EBX, ESP");
               compCode (h, 0x01); compCode (h, 0xE3); compIncD (h);
               break;}
      case 5: {compDisAsm (h, "ADD  ESP, EBP");
               compCode (h, 0x01); compCode (h, 0xEC); compIncD (h);
               break;}
      case 6: {compDisAsm (h, "ADD  EBP, ESI");
               compCode (h, 0x01); compCode (h, 0xF5); compIncD (h);
               break;}
      case 7: {compDisAsm (h, "ADD  ESI, EDI");
               compCode (h, 0x01); compCode (h, 0xFE); compIncD (h);
               break;}
      default: errorHalt ("[navmBackendCompADD] vD > 7 | vD < 1");}}

void navmBackendCompAddImm (pNavmBackend h, int32_t val)
  {compDecD (h);
   switch (h->vD)
     {case 1: {compDisAsmImm32 (h, "ADD  EAX, ", val);
               compCode (h, 0x05); compImm32 (h, val); compIncD (h);
               break;}
      case 2: {compDisAsmImm32 (h, "ADD  ECX, ", val);
               compCode (h, 0x81); compCode (h, 0xC1); compImm32 (h, val);
               compIncD (h);
               break;}
      case 3: {compDisAsmImm32 (h, "ADD  EDX, ", val);
               compCode (h, 0x81); compCode (h, 0xC2); compImm32 (h, val);
               compIncD (h);
               break;}
      case 4: {compDisAsmImm32 (h, "ADD  EBX, ", val);
               compCode (h, 0x81); compCode (h, 0xC3); compImm32 (h, val);
               compIncD (h);
               break;}
      case 5: {compDisAsmImm32 (h, "ADD  ESP, ", val);
               compCode (h, 0x81); compCode (h, 0xC4); compImm32 (h, val);
               compIncD (h);
               break;}
      case 6: {compDisAsmImm32 (h, "ADD  EBP, ", val);
               compCode (h, 0x81); compCode (h, 0xC5); compImm32 (h, val);
               compIncD (h);
               break;}
      case 7: {compDisAsmImm32 (h, "ADD  ESI, ", val);
               compCode (h, 0x81); compCode (h, 0xC6; compImm32 (h, val);
               compIncD (h);
               break;}
      default: errorHalt ("[navmBackendCompAddImm] vD > 7 | vD < 1");}}

void navmBackendCompDIV (pNavmBackend h)
  {compDecD (h); compDecD (h);
   switch (h->vD)
     {case 1: {compDisAsm (h, "MOVD MM0, EDX|XOR EDX, EDX|IDIV ECX|MOVD EAX, MM0");
               compCode (h, 0x0F); compCode (h, 0x6E); compCode (h, 0xC2);
               compCode (h, 0x31); compCode (h, 0xD2);
               compCode (h, 0xF7); compCode (h, 0xF9);
               compCode (h, 0x0F); compCode (h, 0x7E); compCode (h, 0xC2);
               compIncD (h);
               break;}
      case 2: {compDisAsm (h, "MOVD MM0, EDX|MOVD MM1, EAX|MOVD MM2, EBX|MOV EAX, EBX");
               compDisAsm (h, "MOV  EBX, EDX|XOR  EDX, EDX|IDIV EBX     |MOV ECX, EAX");
               compDisAsm (h, "MOVD EBX, MM2|MOVD EAX, MM1|MOVD EDX, MM0");               
               compCode (h, 0x0F); compCode (h, 0x6E); compCode (h, 0xC2);
               compCode (h, 0x0F); compCode (h, 0x6E); compCode (h, 0xC8);
               compCode (h, 0x0F); compCode (h, 0x6E); compCode (h, 0xD3);
               compCode (h, 0x89); compCode (h, 0xC8);
               compCode (h, 0x89); compCode (h, 0xD3);
               compCode (h, 0x31); compCode (h, 0xD2);
               compCode (h, 0xF7); compCode (h, 0xFB);
               compCode (h, 0x89); compCode (h, 0xC1);
               compCode (h, 0x0F); compCode (h, 0x7E); compCode (h, 0xD3);
               compCode (h, 0x0F); compCode (h, 0x7E); compCode (h, 0xC8);
               compCode (h, 0x0F); compCode (h, 0x7E); compCode (h, 0xC2);               
               compIncD (h);
               break;}
      case 3: {compDisAsm (h, "MOVD MM0, EAX|MOV  EAX, EDX|XOR EDX, EDX|IDIV EBX");
               compDisAsm (h, "MOV  EDX, EAX|MOVD EAX, MM0");
               compCode (h, 0x0F); compCode (h, 0x6E); compCode (h, 0xC0);
               compCode (h, 0x89); compCode (h, 0xD0);
               compCode (h, 0x31); compCode (h, 0xD2);
               compCode (h, 0xF7); compCode (h, 0xFB);
               compCode (h, 0x89); compCode (h, 0xC2);
               compCode (h, 0x0F); compCode (h, 0x7E); compCode (h, 0xC0);
               compIncD (h);
               break;}
      case 4: {compDisAsm (h, "MOVD MM0, EAX|MOV  EAX, EBX|XOR EDX, EDX|IDIV ESP");
               compDisAsm (h, "MOV  EBX, EAX|MOVD EAX, MM0");
               compCode (h, 0x0F); compCode (h, 0x6E); compCode (h, 0xC0);
               compCode (h, 0x89); compCode (h, 0xD8);
               compCode (h, 0x31); compCode (h, 0xD2);
               compCode (h, 0xF7); compCode (h, 0xFC);
               compCode (h, 0x89); compCode (h, 0xC3);
               compCode (h, 0x0F); compCode (h, 0x7E); compCode (h, 0xC0);
               compIncD (h);
               break;}
      case 5: {compDisAsm (h, "MOVD MM0, EAX|MOV  EAX, ESP|XOR EDX, EDX|IDIV EBP");
               compDisAsm (h, "MOV  ESP, EAX|MOVD EAX, MM0");
               compCode (h, 0x0F); compCode (h, 0x6E); compCode (h, 0xC0);
               compCode (h, 0x89); compCode (h, 0xE0);
               compCode (h, 0x31); compCode (h, 0xD2);
               compCode (h, 0xF7); compCode (h, 0xFD);
               compCode (h, 0x89); compCode (h, 0xC4);
               compCode (h, 0x0F); compCode (h, 0x7E); compCode (h, 0xC0);
               compIncD (h);
               break;}
      case 6: {compDisAsm (h, "MOVD MM0, EAX|MOV  EAX, EBP|XOR EDX, EDX|IDIV ESI");
               compDisAsm (h, "MOV  EBP, EAX|MOVD EAX, MM0");
               compCode (h, 0x0F); compCode (h, 0x6E); compCode (h, 0xC0);
               compCode (h, 0x89); compCode (h, 0xE8);
               compCode (h, 0x31); compCode (h, 0xD2);
               compCode (h, 0xF7); compCode (h, 0xFE);
               compCode (h, 0x89); compCode (h, 0xC5);
               compCode (h, 0x0F); compCode (h, 0x7E); compCode (h, 0xC0);
               compIncD (h);
               break;}
      case 7: {compDisAsm (h, "MOVD MM0, EAX|MOV  EAX, ESI|XOR EDX, EDX|IDIV EDI");
               compDisAsm (h, "MOV  ESI, EAX|MOVD EAX, MM0");
               compCode (h, 0x0F); compCode (h, 0x6E); compCode (h, 0xC0);
               compCode (h, 0x89); compCode (h, 0xF0);
               compCode (h, 0x31); compCode (h, 0xD2);
               compCode (h, 0xF7); compCode (h, 0xFF);
               compCode (h, 0x89); compCode (h, 0xC6);
               compCode (h, 0x0F); compCode (h, 0x7E); compCode (h, 0xC0);
               compIncD (h);
               break;}
      default: errorHalt ("[navmBackendCompDIV] vD > 7 | vD < 1");}}

void navmBackendCompDivImm (pNavmBackend h, sWord val)
  {errorHalt ("[navmBackendCompDivImm] !");}
  
void navmBackendCompMUL (pNavmBackend h)
  {compDecD (h); compDecD (h);
   switch (h->vD)
     {case 1: {compDisAsm (h, "IMUL EAX, ECX");
               compCode (h, 0x0F); compCode (h, 0xAF); compCode (h, 0xC1);
               compIncD (h);
               break;}
      case 2: {compDisAsm (h, "IMUL ECX, EDX");
               compCode (h, 0x0F); compCode (h, 0xAF); compCode (h, 0xCA);
               compIncD (h);
               break;}
      case 3: {compDisAsm (h, "IMUL EDX, EBX");
               compCode (h, 0x0F); compCode (h, 0xAF); compCode (h, 0xD3);
               compIncD (h);
               break;}
      case 4: {compDisAsm (h, "IMUL EBX, ESP");
               compCode (h, 0x0F); compCode (h, 0xAF); compCode (h, 0xDC);
               compIncD (h);
               break;}
      case 5: {compDisAsm (h, "IMUL ESP, EBP");
               compCode (h, 0x0F); compCode (h, 0xAF); compCode (h, 0xE5);
               compIncD (h);
               break;}
      case 6: {compDisAsm (h, "IMUL EBP, ESI");
               compCode (h, 0x0F); compCode (h, 0xAF); compCode (h, 0xEE);
               compIncD (h);
               break;}
      case 7: {compDisAsm (h, "IMUL ESI, EDI");
               compCode (h, 0x0F); compCode (h, 0xAF); compCode (h, 0xF7);
               compIncD (h);
               break;}
      default: errorHalt ("[navmBackendCompMUL] vD > 7 | vD < 1");}}
                                     
void navmBackendCompMulImm (pNavmBackend h, int32_t val)
  {compDecD (h);
   switch (h->vD)
     {case 1: {compDisAsmImm32 (h, "IMUL EAX, ", val);
               compCode  (h, 0x69); compCode (h, 0xC0); compImm32 (h, val);
               compIncD (h);
               break;}
      case 2: {compDisAsmImm32 (h, "IMUL ECX, ", val);
               compCode  (h, 0x69); compCode (h, 0xC9); compImm32 (h, val);
               compIncD (h);
               break;}
      case 3: {compDisAsmImm32 (h, "IMUL EDX, ", val);
               compCode  (h, 0x69); compCode (h, 0xD2); compImm32 (h, val);
               compIncD (h);
               break;}
      case 4: {compDisAsmImm32 (h, "IMUL EBX, ", val);
               compCode  (h, 0x69); compCode (h, 0xDB); compImm32 (h, val);
               compIncD (h);
               break;}
      case 5: {compDisAsmImm32 (h, "IMUL ESP, ", val);
               compCode  (h, 0x69); compCode (h, 0xE4); compImm32 (h, val);
               compIncD (h);
               break;}
      case 6: {compDisAsmImm32 (h, "IMUL EBP, ", val);
               compCode  (h, 0x69); compCode (h, 0xED); compImm32 (h, val);
               compIncD (h);
               break;}
      case 7: {compDisAsmImm32 (h, "IMUL ESI, ", val);
               compCode  (h, 0x69); compCode (h, 0xF6); compImm32 (h, val);
               compIncD (h);
               break;}
      default: errorHalt ("[navmBackendCompMulImm] vD > 7 | vD < 1");}}

void navmBackendCompAND (pNavmBackend h)
  {compDecD (h); compDecD (h);
   switch (h->vD)
     {case 1: {compDisAsm (h, "AND  EAX, ECX");
               compCode (h, 0x21); compCode (h, 0xC8); compIncD (h);
               break;}
      case 2: {compDisAsm (h, "AND  ECX, EDX");
               compCode (h, 0x21); compCode (h, 0xD1); compIncD (h);
               break;}
      case 3: {compDisAsm (h, "AND  EDX, EBX");
               compCode (h, 0x21); compCode (h, 0xDA); compIncD (h);
               break;}
      case 4: {compDisAsm (h, "AND  EBX, ESP");
               compCode (h, 0x21); compCode (h, 0xE3); compIncD (h);
               break;}
      case 5: {compDisAsm (h, "AND  ESP, EBP");
               compCode (h, 0x21); compCode (h, 0xEC); compIncD (h);
               break;}
      case 6: {compDisAsm (h, "AND  EBP, ESI");
               compCode (h, 0x21); compCode (h, 0xF5); compIncD (h);
               break;}
      case 7: {compDisAsm (h, "AND  ESI, EDI");
               compCode (h, 0x21); compCode (h, 0xFE); compIncD (h);
               break;}
      default: errorHalt ("[navmBackendCompAND] vD > 7 | vD < 1");}}

void navmBackendCompAndImm (pNavmBackend h, int32_t val)
  {compDecD (h);
   switch (h->vD)
     {case 1: {compDisAsmImm32 (h, "AND  EAX, ", val);
               compCode (h, 0x25); compImm32 (h, val);  compIncD (h);
               break;}
      case 2: {compDisAsmImm32 (h, "AND  ECX, ", val);
               compCode  (h, 0x81); compCode (h, 0xE1); compImm32 (h, val);
               compIncD (h);
               break;}
      case 3: {compDisAsmImm32 (h, "AND  EDX,  ", val);
               compCode  (h, 0x81); compCode (h, 0xE2); compImm32 (h, val);
               compIncD (h);
               break;}
      case 4: {compDisAsmImm32 (h, "AND  EBX, ", val);
               compCode  (h, 0x81); compCode (h, 0xE3); compImm32 (h, val);
               compIncD (h);
               break;}
      case 5: {compDisAsmImm32 (h, "AND  ESP, ", val);
               compCode  (h, 0x81); compCode (h, 0xE4); compImm32 (h, val);
               compIncD (h);
               break;}
      case 6: {compDisAsmImm32 (h, "AND  EBP, ", val);
               compCode  (h, 0x81); compCode (h, 0xE5); compImm32 (h, val);
               compIncD (h);
               break;}
      case 7: {compDisAsmImm32 (h, "AND  ESI, ", val);
               compCode  (h, 0x81); compCode (h, 0xE6); compImm32 (h, val);
               compIncD (h);
               break;}
      default: errorHalt ("[navmBackendCompAndImm] vD > 7 | vD < 1");}}

void navmBackendCompGOR (pNavmBackend h)
  {compDecD (h); compDecD (h);
   switch (h->vD)
     {case 1: {compDisAsm (h, "OR   EAX, ECX");
               compCode (h, 0x09); compCode (h, 0xC8); compIncD (h);
               break;}
      case 2: {compDisAsm (h, "OR   ECX, EDX");
               compCode (h, 0x09); compCode (h, 0xD1); compIncD (h);
               break;}
      case 3: {compDisAsm (h, "OR   EDX, EBX");
               compCode (h, 0x09); compCode (h, 0xDA); compIncD (h);
               break;}
      case 4: {compDisAsm (h, "OR   EBX, ESP");
               compCode (h, 0x09); compCode (h, 0xE3); compIncD (h);
               break;}
      case 5: {compDisAsm (h, "OR   ESP, EBP");
               compCode (h, 0x09); compCode (h, 0xEC); compIncD (h);
               break;}
      case 6: {compDisAsm (h, "OR   EBP, ESI");
               compCode (h, 0x09); compCode (h, 0xF5); compIncD (h);
               break;}
      case 7: {compDisAsm (h, "OR   ESI, EDI");
               compCode (h, 0x09); compCode (h, 0xFE); compIncD (h);
               break;}
      default: errorHalt ("[navmBackendCompGOR] vD > 7 | vD < 1");}}

void navmBackendCompGorImm (pNavmBackend h, int32_t val)
  {compDecD (h);
   switch (h->vD)
     {case 1: {compDisAsmImm32 (h, "OR   EAX, ", val);
               compCode (h, 0x0D); compImm32 (h, val);  compIncD (h);
               break;}
      case 2: {compDisAsmImm32 (h, "OR   ECX, ", val);
               compCode  (h, 0x81); compCode (h, 0xC9); compImm32 (h, val);
               compIncD (h);
               break;}
      case 3: {compDisAsmImm32 (h, "OR   EDX,  ", val);
               compCode  (h, 0x81); compCode (h, 0xCA); compImm32 (h, val);
               compIncD (h);
               break;}
      case 4: {compDisAsmImm32 (h, "OR   EBX, ", val);
               compCode  (h, 0x81); compCode (h, 0xCB); compImm32 (h, val);
               compIncD (h);
               break;}
      case 5: {compDisAsmImm32 (h, "OR   ESP, ", val);
               compCode  (h, 0x81); compCode (h, 0xCC); compImm32 (h, val);
               compIncD (h);
               break;}
      case 6: {compDisAsmImm32 (h, "OR   EBP, ", val);
               compCode  (h, 0x81); compCode (h, 0xCD); compImm32 (h, val);
               compIncD (h);
               break;}
      case 7: {compDisAsmImm32 (h, "OR   ESI, ", val);
               compCode  (h, 0x81); compCode (h, 0xCE); compImm32 (h, val);
               compIncD (h);
               break;}
      default: errorHalt ("[navmBackendCompAndImm] vD > 7 | vD < 1");}}
                                                                                                                                                                                                                                                                                                                                                                
void navmBackendCompXOR (pNavmBackend h)
  {compDecD (h); compDecD (h);
   switch (h->vD)
     {case 1: {compDisAsm (h, "XOR  EAX, ECX");
               compCode (h, 0x31); compCode (h, 0xC8); compIncD (h);
               break;}
      case 2: {compDisAsm (h, "XOR  ECX, EDX");
               compCode (h, 0x31); compCode (h, 0xD1); compIncD (h);
               break;}
      case 3: {compDisAsm (h, "XOR  EDX, EBX");
               compCode (h, 0x31); compCode (h, 0xDA); compIncD (h);
               break;}
      case 4: {compDisAsm (h, "XOR  EBX, ESP");
               compCode (h, 0x31); compCode (h, 0xE3); compIncD (h);
               break;}
      case 5: {compDisAsm (h, "XOR  ESP, EBP");
               compCode (h, 0x31); compCode (h, 0xEC); compIncD (h);
               break;}
      case 6: {compDisAsm (h, "XOR  EBP, ESI");
               compCode (h, 0x31); compCode (h, 0xF5); compIncD (h);
               break;}
      case 7: {compDisAsm (h, "XOR  ESI, EDI");
               compCode (h, 0x31); compCode (h, 0xFE); compIncD (h);
               break;}
      default: errorHalt ("[navmBackendCompXOR] vD > 7 | vD < 1");}}

void navmBackendCompXorImm (pNavmBackend h, int32_t val)
  {compDecD (h);
   switch (h->vD)
     {case 1: {compDisAsmImm32 (h, "XOR  EAX, ", val);
               compCode (h, 0x35); compImm32 (h, val); compIncD (h);
               break;}
      case 2: {compDisAsmImm32 (h, "XOR  ECX, ", val);
               compCode (h, 0x81); compCode (h, 0xF1); compImm32 (h, val);
               compIncD  (h);
               break;}
      case 3: {compDisAsmImm32 (h, "XOR  EDX, ", val);
               compCode (h, 0x81); compCode (h, 0xF2); compImm32 (h, val);
               compIncD  (h);
               break;}
      case 4: {compDisAsmImm32 (h, "XOR  EBX, ", val);
               compCode (h, 0x81); compCode (h, 0xF3); compImm32 (h, val);
               compIncD  (h);
               break;}
      case 5: {compDisAsmImm32 (h, "XOR  ESP, ", val);
               compCode (h, 0x81); compCode (h, 0xF4); compImm32 (h, val);
               compIncD  (h);
               break;}
      case 6: {compDisAsmImm32 (h, "XOR  EBP, ", val);
               compCode (h, 0x81); compCode (h, 0xF5); compImm32 (h, val);
               compIncD  (h);
               break;}
      case 7: {compDisAsmImm32 (h, "XOR  ESI, ", val);
               compCode (h, 0x81); compCode (h, 0xF6); compImm32 (h, val);
               compIncD  (h);
               break;}
      default: errorHalt ("[navmBackendCompXorImm] vD > 7 | vD < 1");}}

void navmBackendCompSHL (pNavmBackend h)
  {compDecD (h); compDecD (h);
   switch (h->vD)
     {case 1: {compDisAsm (h, "SHL EAX, CL");
               compCode (h, 0xD3); compCode (h, 0xE0); compIncD (h);
               break;}
      case 2: {compDisAsm (h, "XCHG ECX, EDX|SHL  EDX, CL|XCHG ECX, EDX");
               compCode (h, 0x87); compCode (h, 0xCA); 
               compCode (h, 0xD3); compCode (h, 0xE2);
               compCode (h, 0x87); compCode (h, 0xCA);
               compIncD (h);
               break;}
      case 3: {compDisAsm (h, "XCHG ECX, EBX|SHL  EDX, CL|XCHG ECX, EBX");
               compCode (h, 0x87); compCode (h, 0xCB);
               compCode (h, 0xD3); compCode (h, 0xE2);
               compCode (h, 0x87); compCode (h, 0xCB);
               compIncD (h);
               break;}
      case 4: {compDisAsm (h, "XCHG ECX, ESP|SHL  EBX, CL|XCHG ECX, ESP");
               compCode (h, 0x87); compCode (h, 0xCC);
               compCode (h, 0xD3); compCode (h, 0xE3);
               compCode (h, 0x87); compCode (h, 0xCC);
               compIncD (h);
               break;}
      case 5: {compDisAsm (h, "XCHG ECX, EBP|SHL  ESP, CL|XCHG ECX, EBP");
               compCode (h, 0x87); compCode (h, 0xCD);
               compCode (h, 0xD3); compCode (h, 0xE4);
               compCode (h, 0x87); compCode (h, 0xCD);
               compIncD (h);
               break;}
      case 6: {compDisAsm (h, "XCHG ECX, ESI|SHL  EBX, CL|XCHG ECX, ESI");
               compCode (h, 0x87); compCode (h, 0xCE);
               compCode (h, 0xD3); compCode (h, 0xE5);
               compCode (h, 0x87); compCode (h, 0xCE);
               compIncD (h);
               break;}
      case 7: {compDisAsm (h, "XCHG ECX, EDI|SHL  ESI, CL|XCHG ECX, EDI");
               compCode (h, 0x87); compCode (h, 0xCF);
               compCode (h, 0xD3); compCode (h, 0xE6);
               compCode (h, 0x87); compCode (h, 0xCF);
               compIncD (h);
               break;}
      default: errorHalt ("[navmBackendCompSHL] vD > 7 | vD < 1");}}

void navmBackendCompShlImm (pNavmBackend h, uint8_t val)
  {compDecD (h);
   switch (h->vD)
     {case 1: {compDisAsmImm8 (h, "SHL  EAX, ", val);
               compCode (h, 0xC1); compCode (h, 0xE0); compCode (h, val);
               compIncD (h);
               break;}
      case 2: {compDisAsmImm8 (h, "SHL  ECX, ", val);
               compCode (h, 0xC1); compCode (h, 0xE1); compCode (h, val);
               compIncD (h);
               break;}
      case 3: {compDisAsmImm8 (h, "SHL  EDX, ", val);
               compCode (h, 0xC1); compCode (h, 0xE2); compCode (h, val);
               compIncD (h);
               break;}
      case 4: {compDisAsmImm8 (h, "SHL  EBX, ", val);
               compCode (h, 0xC1); compCode (h, 0xE3); compCode (h, val);
               compIncD (h);
               break;}
      case 5: {compDisAsmImm8 (h, "SHL  ESP, ", val);
               compCode (h, 0xC1); compCode (h, 0xE4); compCode (h, val);
               compIncD (h);
               break;}
      case 6: {compDisAsmImm8 (h, "SHL  EBP, ", val);
               compCode (h, 0xC1); compCode (h, 0xE5); compCode (h, val);
               compIncD (h);
               break;}
      case 7: {compDisAsmImm8 (h, "SHL  ESI, ", val);
               compCode (h, 0xC1); compCode (h, 0xE6); compCode (h, val);
               compIncD (h);
               break;}
      default: errorHalt ("[navmBackendCompShlImm] vD > 7 | vD < 1");}}

void navmBackendCompSHR (pNavmBackend h)
  {compDecD (h); compDecD (h);
   switch (h->vD)
     {case 1: {compDisAsm (h, "SHR EAX, CL");
               compCode (h, 0xD3); compCode (h, 0xE8); compIncD (h);
               break;}
      case 2: {compDisAsm (h, "XCHG ECX, EDX|SHR  EDX, CL|XCHG ECX, EDX");
               compCode (h, 0x87); compCode (h, 0xCA); 
               compCode (h, 0xD3); compCode (h, 0xEA);
               compCode (h, 0x87); compCode (h, 0xCA);
               compIncD (h);
               break;}
      case 3: {compDisAsm (h, "XCHG ECX, EBX|SHR  EDX, CL|XCHG ECX, EBX");
               compCode (h, 0x87); compCode (h, 0xCB);
               compCode (h, 0xD3); compCode (h, 0xEA);
               compCode (h, 0x87); compCode (h, 0xCB);
               compIncD (h);
               break;}
      case 4: {compDisAsm (h, "XCHG ECX, ESP|SHR  EBX, CL|XCHG ECX, ESP");
               compCode (h, 0x87); compCode (h, 0xCC);
               compCode (h, 0xD3); compCode (h, 0xEB);
               compCode (h, 0x87); compCode (h, 0xCC);
               compIncD (h);
               break;}
      case 5: {compDisAsm (h, "XCHG ECX, EBP|SHR  ESP, CL|XCHG ECX, EBP");
               compCode (h, 0x87); compCode (h, 0xCD);
               compCode (h, 0xD3); compCode (h, 0xEC);
               compCode (h, 0x87); compCode (h, 0xCD);
               compIncD (h);
               break;}
      case 6: {compDisAsm (h, "XCHG ECX, ESI|SHR  EBX, CL|XCHG ECX, ESI");
               compCode (h, 0x87); compCode (h, 0xCE);
               compCode (h, 0xD3); compCode (h, 0xED);
               compCode (h, 0x87); compCode (h, 0xCE);
               compIncD (h);
               break;}
      case 7: {compDisAsm (h, "XCHG ECX, EDI|SHR  ESI, CL|XCHG ECX, EDI");
               compCode (h, 0x87); compCode (h, 0xCF);
               compCode (h, 0xD3); compCode (h, 0xEE);
               compCode (h, 0x87); compCode (h, 0xCF);
               compIncD (h);
               break;}
      default: errorHalt ("[navmBackendCompSHL] vD > 7 | vD < 1");}}

void navmBackendCompShrImm (pNavmBackend h, uint8_t val)
  {compDecD (h);
   switch (h->vD)
     {case 1: {compDisAsmImm8 (h, "SHR  EAX, ", val);
               compCode (h, 0xC1); compCode (h, 0xE8); compCode (h, val);
               compIncD (h);
               break;}
      case 2: {compDisAsmImm8 (h, "SHR  ECX, ", val);
               compCode (h, 0xC1); compCode (h, 0xE9); compCode (h, val);
               compIncD (h);
               break;}
      case 3: {compDisAsmImm8 (h, "SHR  EDX, ", val);
               compCode (h, 0xC1); compCode (h, 0xEA); compCode (h, val);
               compIncD (h);
               break;}
      case 4: {compDisAsmImm8 (h, "SHR  EBX, ", val);
               compCode (h, 0xC1); compCode (h, 0xEB); compCode (h, val);
               compIncD (h);
               break;}
      case 5: {compDisAsmImm8 (h, "SHR  ESP, ", val);
               compCode (h, 0xC1); compCode (h, 0xEC); compCode (h, val);
               compIncD (h);
               break;}
      case 6: {compDisAsmImm8 (h, "SHR  EBP, ", val);
               compCode (h, 0xC1); compCode (h, 0xED); compCode (h, val);
               compIncD (h);
               break;}
      case 7: {compDisAsmImm8 (h, "SHR  ESI, ", val);
               compCode (h, 0xC1); compCode (h, 0xEE); compCode (h, val);
               compIncD (h);
               break;}
      default: errorHalt ("[navmBackendCompShlImm] vD > 7 | vD < 1");}}

void navmBackendCompDUP (pNavmBackend h)
  {switch (h->vD)
     {case 1: {compDisAsm (h, "MOV  ECX, EAX");
               compCode (h, 0x89); compCode (h, 0xC1); compIncD (h);
               break;}
      case 2: {compDisAsm (h, "MOV  EDX, ECX");
               compCode (h, 0x89); compCode (h, 0xCA); compIncD (h);
               break;}
      case 3: {compDisAsm (h, "MOV  EBX, EDX");
               compCode (h, 0x89); compCode (h, 0xD3); compIncD (h);
               break;}
      case 4: {compDisAsm (h, "MOV  ESP, EBX");
               compCode (h, 0x89); compCode (h, 0xDC); compIncD (h);
               break;}
      case 5: {compDisAsm (h, "MOV  EBP, ESP");
               compCode (h, 0x89); compCode (h, 0xE5); compIncD (h);
               break;}
      case 6: {compDisAsm (h, "MOV  ESI, EBP");
               compCode (h, 0x89); compCode (h, 0xF4); compIncD (h);
               break;}
      case 7: {compDisAsm (h, "MOV  EDI, ESI");
               compCode (h, 0x89); compCode (h, 0xFE); compIncD (h);
               break;}
      default: errorHalt ("[navmBackendCompDUP] vD > 7 | vD < 1");}}

void navmBackendCompDRP (pNavmBackend h) {compDecD (h);}

void navmBackendCompSWP (pNavmBackend h)
  {switch (h->vD)
     {case 1: {compDisAsm (h, "XCHG ECX, EAX");
               compCode (h, 0x91);
               break;}
      case 2: {compDisAsm (h, "XCHG EDX, ECX");
               compCode (h, 0x87); compCode (h, 0xD1);
               break;}
      case 3: {compDisAsm (h, "XCHG EBX, EDX");
               compCode (h, 0x87); compCode (h, 0xDA);
               break;}
      case 4: {compDisAsm (h, "XCHG EBX, EDX");
               compCode (h, 0x87); compCode (h, 0xE3);
               break;}
      case 5: {compDisAsm (h, "XCHG ESP, EBX");
               compCode (h, 0x87); compCode (h, 0xEC);
               break;}
      case 6: {compDisAsm (h, "XCHG ESI, ESP");
               compCode (h, 0x87); compCode (h, 0xF5);
               break;}
      case 7: {compDisAsm (h, "XCHG EDI, ESI");
               compCode (h, 0x87); compCode (h, 0xFE);
               break;}
      default: errorHalt ("[navmBackendCompSWP] vD > 7 | vD < 1");}}

void navmBackendCompROT (pNavmBackend h)
  {}
