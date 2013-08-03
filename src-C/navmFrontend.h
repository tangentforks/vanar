/*****************************************************************************
  PROJECT:     NAVM AOT COMPILER
  DESCRIPTION: COMPILER FOR THE NAVM LEVEL 1 ISA AS IL REPRESENTATION
  INFO:        ABSTRACT FRONTEND INTERFACE OF THE COMPILER
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
  STANDARD:    ANSI C 1989 + POSIX 1
  HISTORY:     7-15-2013: FIRST VERSION
  TO-DO:       NONE AT CURRENT
  ****************************************************************************/
  
/* -------------------------------------------------------------------------
   INFO: Help routines, macros and declarations.
   ------------------------------------------------------------------------- */

#define notMacroOpcode                                                        \
(ins.aOpcodes[i] != 76)  && (ins.aOpcodes[i] != 102) &&                       \
(ins.aOpcodes[i] != 119) && (ins.aOpcodes[i] != 136) &&                       \
(ins.aOpcodes[i] != 140) && (ins.aOpcodes[i] != 153) &&                       \
(ins.aOpcodes[i] != 156) && (ins.aOpcodes[i] != 170) &&                       \
(ins.aOpcodes[i] != 172) && (ins.aOpcodes[i] != 187) &&                       \
(ins.aOpcodes[i] != 208) && (ins.aOpcodes[i] != 209) &&                       \
(ins.aOpcodes[i] != 211) && (ins.aOpcodes[i] != 212) &&                       \
(ins.aOpcodes[i] != 213) && (ins.aOpcodes[i] != 214) &&                       \
(ins.aOpcodes[i] != 215) && (ins.aOpcodes[i] != 216) &&                       \
(ins.aOpcodes[i] != 217) && (ins.aOpcodes[i] != 218) &&                       \
(ins.aOpcodes[i] != 220) && (ins.aOpcodes[i] != 223) &&                       \
(ins.aOpcodes[i] != 236) && (ins.aOpcodes[i] != 238) &&                       \
(ins.aOpcodes[i] != 255)

enum {insLI,  insLD,  insST, insADD, insDIV, insMUL, insSHL, insSHR, insAND,
      insGOR, insXOR, insCP, insDUP, insDRP, insSWP, insROT};

enum {opcLI  = 76,  opcST  = 102, opcDUP = 119, opcDRP = 136, opcB   = 140,
      opcSWP = 153, opcBC  = 156, opcROT = 170, opcBS  = 172, opcLD  = 187,
      opcBCS = 208, opcBR  = 209, opcBCR = 211,	opcR   = 212, opcRC  = 213,
      opcADD = 214, opcDIV = 215, opcMUL = 216,	opcAND = 217, opcGOR = 218,	
      opcCP  = 220, opcXOR = 223, opcSHL = 236, opcSYS = 238, opcSHR = 255,
      
      opcLiLI  = 0,   opcLiLD  = 16,  opcLiST  = 32,  opcLiADD = 48,
      opcLiDIV = 64,  opcLiMUL = 80,  opcLiSHL = 96,  opcLiSHR = 112,
      opcLiAND = 128, opcLiGOR = 144, opcLiXOR = 160, opcLiCP  = 176,
      opcLiDUP = 192, opcLiSWP = 224, opcLiROT = 240};

bool immRange (sWord pImm, uByte bitSize)
  {sWord test = pImm >> (bitSize-1);
   if (test != 0) return false; else return true;}

/* -------------------------------------------------------------------------
   INFO: Compiler functions for single instructions, extending there
   backend equivalents (basical adding state-dependent functionality most
   to all frontend compilation-strategies)
   ------------------------------------------------------------------------- */

pWord pImm;

void navmFrontendCompLI  (pNavmBackend p) {navmBackendCompLI  (p, *pImm++);}
void navmFrontendCompLD  (pNavmBackend p) {navmBackendCompLD  (p);}
void navmFrontendCompST  (pNavmBackend p) {navmBackendCompST  (p);}
void navmFrontendCompDUP (pNavmBackend p) {navmBackendCompDUP (p);}
void navmFrontendCompDRP (pNavmBackend p) {navmBackendCompDRP (p);}
void navmFrontendCompSWP (pNavmBackend p) {navmBackendCompSWP (p);}
void navmFrontendCompROT (pNavmBackend p) {navmBackendCompROT (p);}

void navmFrontendCompADD (pNavmBackend p)
  {if (immRange (*pImm, cfNavmBackendImmAddLen) != true)
       navmBackendCompADD (p); else navmBackendCompAddImm (p, *pImm++);}

void navmFrontendCompDIV (pNavmBackend p)
  {if (immRange (*pImm, cfNavmBackendImmDivLen) != true)
       navmBackendCompDIV (p); else navmBackendCompDivImm (p, *pImm++);}

void navmFrontendCompMUL (pNavmBackend p)
  {if (immRange (*pImm, cfNavmBackendImmMulLen) != true)
       navmBackendCompMUL (p); else navmBackendCompMulImm (p, *pImm++);}

void navmFrontendCompAND (pNavmBackend p)
  {if (immRange (*pImm, cfNavmBackendImmAndLen) != true)
       navmBackendCompAND (p); else navmBackendCompAndImm (p, *pImm++);}

void navmFrontendCompGOR (pNavmBackend p)
  {if (immRange (*pImm, cfNavmBackendImmGorLen) != true)
       navmBackendCompGOR (p); else navmBackendCompGorImm (p, *pImm++);}

void navmFrontendCompXOR (pNavmBackend p)
  {if (immRange (*pImm, cfNavmBackendImmXorLen) != true)
       navmBackendCompXOR (p); else navmBackendCompXorImm (p, *pImm++);}

void navmFrontendCompSHL (pNavmBackend p)
  {if (immRange (*pImm, cfNavmBackendImmShlLen) != true)
       navmBackendCompSHL (p); else navmBackendCompShlImm (p, *pImm++);}

void navmFrontendCompSHR (pNavmBackend p)
  {if (immRange (*pImm, cfNavmBackendImmShrLen) != true)
       navmBackendCompSHR (p); else navmBackendCompShrImm (p, *pImm++);}

void navmFrontendCompCP  (pNavmBackend p)
  {if (immRange (*pImm, cfNavmBackendImmCpLen) != true)
       navmBackendCompCP (p); else navmBackendCompCpImm (p, *pImm++);}

/* -------------------------------------------------------------------------
   INFO: The following compiler strategy fits for superscalar out-of-order
   designs without dynamic register sheduling
   ------------------------------------------------------------------------- */

bool navmFrontendDynScal (pNavmBackend h, uOpcode ins)
  {uByte i1, i2, i; bool ret = false;
   for (i = 0; i < 8; i++)
     {if (notMacroOpcode) 
         {i1 = ins.aOpcodes[i] & 0x0F; i2 = (ins.aOpcodes[i] >> 4) & 0x0F;
          switch (i1)
            {case insLI:  {navmBackendCompLI  (h, *pImm++); break;}
             case insLD:  {navmBackendCompLD  (h); break;}
             case insST:  {navmBackendCompST  (h); break;}
             case insADD: {navmBackendCompADD (h); break;}
             case insDIV: {navmBackendCompDIV (h); break;}
             case insMUL: {navmBackendCompMUL (h); break;}
             case insAND: {navmBackendCompAND (h); break;}
             case insGOR: {navmBackendCompGOR (h); break;}
             case insXOR: {navmBackendCompXOR (h); break;}
             case insSHL: {navmBackendCompSHL (h); break;}
             case insSHR: {navmBackendCompSHR (h); break;}
             case insCP:  {navmBackendCompCP  (h); break;}
             case insDUP: {navmBackendCompDUP (h); break;}
             case insDRP: {navmBackendCompDRP (h); break;}
             case insSWP: {navmBackendCompSWP (h); break;}
             case insROT: {navmBackendCompROT (h); break;}}
          switch (i2)
            {case insLI:  {navmBackendCompLI  (h, *pImm++); break;}
             case insLD:  {navmBackendCompLD  (h); break;}
             case insST:  {navmBackendCompST  (h); break;}
             case insADD: {navmBackendCompADD (h); break;}
             case insDIV: {navmBackendCompDIV (h); break;}
             case insMUL: {navmBackendCompMUL (h); break;}
             case insAND: {navmBackendCompAND (h); break;}
             case insGOR: {navmBackendCompGOR (h); break;}
             case insXOR: {navmBackendCompXOR (h); break;}
             case insSHL: {navmBackendCompSHL (h); break;}
             case insSHR: {navmBackendCompSHR (h); break;}
             case insCP:  {navmBackendCompCP  (h); break;}
             case insDUP: {navmBackendCompDUP (h); break;}
             case insDRP: {navmBackendCompDRP (h); break;}
             case insSWP: {navmBackendCompSWP (h); break;}
             case insROT: {navmBackendCompROT (h); break;}}}
      else {switch (ins.aOpcodes[i])
              {case opcLI:  {navmBackendCompLI  (h, *pImm++); break;}
               case opcLD:  {navmBackendCompLD  (h); break;}
               case opcST:  {navmBackendCompST  (h); break;}
               case opcADD: {navmBackendCompADD (h); break;}
               case opcDIV: {navmBackendCompDIV (h); break;}
               case opcMUL: {navmBackendCompMUL (h); break;}
               case opcAND: {navmBackendCompAND (h); break;}
               case opcGOR: {navmBackendCompGOR (h); break;}
               case opcXOR: {navmBackendCompXOR (h); break;}
               case opcSHL: {navmBackendCompSHL (h); break;}
               case opcSHR: {navmBackendCompSHR (h); break;}
               case opcCP:  {navmBackendCompCP  (h); break;}
               case opcDUP: {navmBackendCompDUP (h); break;}
               case opcDRP: {navmBackendCompDRP (h); break;}
               case opcSWP: {navmBackendCompSWP (h); break;}
               case opcROT: {navmBackendCompROT (h); break;}
               case opcBR:  {ret = true; goto endFunction;}
               case opcBCR: {ret = true; goto endFunction;}
               default:     errorHalt ("[navmFrontendDynScal]");}}}
      endFunction: return ret;}

/* -------------------------------------------------------------------------
   INFO: Compiler strategy for out-of-order architectures with dynamic
   register sheduling. For such designs an accumulator-store approach fits
   quite well.
   ------------------------------------------------------------------------- */

bool navmFrontendDynScalRegShed (pNavmBackend h, uOpcode ins)
  {uByte i1, i2, i; bool ret = false;
   for (i = 0; i < 8; i++)
     {switch (ins.aOpcodes[i])
        {case opcLiADD: {navmFrontendCompADD (h); break;}
         case opcLiDIV: {navmFrontendCompDIV (h); break;}
         case opcLiMUL: {navmFrontendCompMUL (h); break;}
         case opcLiAND: {navmFrontendCompAND (h); break;}
         case opcLiGOR: {navmFrontendCompGOR (h); break;}
         case opcLiXOR: {navmFrontendCompXOR (h); break;}
         case opcLiSHL: {navmFrontendCompSHL (h); break;}
         case opcLiSHR: {navmFrontendCompSHR (h); break;}
         case opcLiCP:  {navmFrontendCompCP  (h); break;}
         case opcLI:    {navmBackendCompLI   (h, *pImm++); break;}
         case opcLD:    {navmBackendCompLD   (h); break;}
         case opcST:    {navmBackendCompST   (h); break;}
         case opcADD:   {navmBackendCompADD  (h); break;}
         case opcDIV:   {navmBackendCompDIV  (h); break;}
         case opcMUL:   {navmBackendCompMUL  (h); break;}
         case opcAND:   {navmBackendCompAND  (h); break;}
         case opcGOR:   {navmBackendCompGOR  (h); break;}
         case opcXOR:   {navmBackendCompXOR  (h); break;}
         case opcSHL:   {navmBackendCompSHL  (h); break;}
         case opcSHR:   {navmBackendCompSHR  (h); break;}
         case opcCP:    {navmBackendCompCP   (h); break;}
         case opcDUP:   {navmBackendCompDUP  (h); break;}
         case opcDRP:   {navmBackendCompDRP  (h); break;}
         case opcSWP:   {navmBackendCompSWP  (h); break;}
         case opcROT:   {navmBackendCompROT  (h); break;}
         case opcBR:    {ret = true; goto endFunction;}
         case opcBCR:   {ret = true; goto endFunction;}
         
         default: {i1 = ins.aOpcodes[i] & 0x0F;
                   i2 = (ins.aOpcodes[i] >> 4) & 0x0F;
                   switch (i1)
                     {case insLI:  {navmBackendCompLI  (h, *pImm++); break;}
                      case insLD:  {navmBackendCompLD  (h); break;}
                      case insST:  {navmBackendCompST  (h); break;}
                      case insADD: {navmBackendCompADD (h); break;}
                      case insDIV: {navmBackendCompDIV (h); break;}
                      case insMUL: {navmBackendCompMUL (h); break;}
                      case insAND: {navmBackendCompAND (h); break;}
                      case insGOR: {navmBackendCompGOR (h); break;}
                      case insXOR: {navmBackendCompXOR (h); break;}
                      case insSHL: {navmBackendCompSHL (h); break;}
                      case insSHR: {navmBackendCompSHR (h); break;}
                      case insCP:  {navmBackendCompCP  (h); break;}
                      case insDUP: {navmBackendCompDUP (h); break;}
                      case insDRP: {navmBackendCompDRP (h); break;}
                      case insSWP: {navmBackendCompSWP (h); break;}
                      case insROT: {navmBackendCompROT (h); break;}}
                    switch (i2)
                      {case insLI:  {navmBackendCompLI  (h, *pImm++); break;}
                       case insLD:  {navmBackendCompLD  (h); break;}
                       case insST:  {navmBackendCompST  (h); break;}
                       case insADD: {navmBackendCompADD (h); break;}
                       case insDIV: {navmBackendCompDIV (h); break;}
                       case insMUL: {navmBackendCompMUL (h); break;}
                       case insAND: {navmBackendCompAND (h); break;}
                       case insGOR: {navmBackendCompGOR (h); break;}
                       case insXOR: {navmBackendCompXOR (h); break;}
                       case insSHL: {navmBackendCompSHL (h); break;}
                       case insSHR: {navmBackendCompSHR (h); break;}
                       case insCP:  {navmBackendCompCP  (h); break;}
                       case insDUP: {navmBackendCompDUP (h); break;}
                       case insDRP: {navmBackendCompDRP (h); break;}
                       case insSWP: {navmBackendCompSWP (h); break;}
                       case insROT: {navmBackendCompROT (h); break;}
                       default:  errorHalt ("[navmFrontendDynScalRegShed]");}}}}
                       
      endFunction: return ret;}

/* -------------------------------------------------------------------------
   INFO: This is the compiler frontend for the NAVM level 1 instruction set
   ------------------------------------------------------------------------- */

void navmCompile (pNavmBackend h, pWord adr)
  {uOpcode ins; bool finish = false;
   while (finish != true)
     {ins.bundle = *adr++; pImm = adr;
      if (cNavmBackendArchType == navmArchOutOfOrderRegShed)
           finish = navmFrontendDynScalRegShed (h, ins);
      else if (cNavmBackendArchType == navmArchOutOfOrder)
                finish = navmFrontendDynScal (h, ins);
           else errorHalt ("[navmCompile] cNavmBackendArchType!");}}
