/*****************************************************************************
  PROJECT:     NAVM AOT COMPILER
  DESCRIPTION: COMPILER FOR THE NAVM LEVEL 1 ISA AS IL REPRESENTATION
  INFO:        ABSTRACT BACKEND INTERFACE OF THE COMPILER
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
   INFO: This is the handler structure for each specific backend. It holds
         the whole state for the single-pass backend compiler. Please note
         that all optimizations are handled by the frontend interface.
         
   STATE VARIABLES: pMem     - pointer into machine-code array
                    oMem     - offset into first free address of mcode array
                    cMemSize - size of machine-code array [byte]
                    vD       - virtual data-stack depth
                    vR       - virtual adress-stack depth
                    fDisAsm  - true for disassembly by compilation
                    vLI      - number of following LI instructions
                               (needed for code-optimization)
   ------------------------------------------------------------------------- */
                    
typedef struct
  {pWord pMem; uWord oMem; uWord cMemSize; uWord vD; uWord vR; bool fDisAsm;
   uByte vLI;} hNavmBackend;

typedef hNavmBackend *pNavmBackend;
  
/* -------------------------------------------------------------------------
   INFO: Function for initialising a new hNavmBackend handler
   
   PARAMETERS: codeArraySize - size of the machine-code array to allocate
   RETURN:     pNavmBackend  - pointer to new initialisated handler
   
   INTERNAL VARIABLES: retSize - size of the handler structure to be created
                       ret     - pointer to newly created handler structure
   ------------------------------------------------------------------------- */

void navmInitBackendHandlerExit (void)
  {fprintf (stderr, "[navmInitBackendHandler] pMem = NIL!\n");
   exit (0);}
   
pNavmBackend navmInitBackendHandler (uWord codeArraySize)
  {uWord retSize = sizeof (hNavmBackend);
   pNavmBackend ret = (pNavmBackend) malloc (retSize);
     ret->pMem     = (pWord) malloc (codeArraySize);
     ret->oMem     = 0;
     ret->cMemSize = codeArraySize;
     ret->vD       = 1;
     ret->vR       = 1;
     mprotect (ret, codeArraySize, PROT_READ|PROT_WRITE|PROT_EXEC);
   if (ret->pMem == NULL) navmInitBackendHandlerExit ();
   return ret;}

/* -------------------------------------------------------------------------
   INFO: Free allocated memory of a given pointer handle
   ------------------------------------------------------------------------- */

void navmFreeBackendHandler (pNavmBackend handler)
  {free (handler->pMem); free (handler);}
