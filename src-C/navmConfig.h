/*****************************************************************************
  PROJECT:     NAVM AOT COMPILER
  DESCRIPTION: COMPILER FOR THE NAVM LEVEL 1 ISA AS IL REPRESENTATION
  INFO:        CENTRAL INCLUDE FILE FOR CONFIGURATION OPTIONS
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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/mman.h>

/* -------------------------------------------------------------------------
   select the right basic type-definations for your CPU:

     navmTypes16.h - for CPU's with 16-bit word-size
     navmTypes32.h - for CPU's with 32-bit word-size
     navmTypes64.h - for CPU's with 64-bit word-size
   
   For CPU architectures with different word-sizes, a new type description
   must be created. Plase name the file in the navmTypes<size> convention
   and do not forget to include it! As the minimal word-size of the NAVM
   Level 1 ISA is 16 bit, for consequent 8 bit architecures like the
   mos6502 and rca1802 16-bit wide access need to be emulated. It is
   convient to define some macros for this!
   ------------------------------------------------------------------------- */

#include "navmTypes64.h"

/* -------------------------------------------------------------------------
   The backend handler. This is the abstract interface of the compiler
   backend. Please do not remove the following line !
   ------------------------------------------------------------------------- */

#include "navmBackend.h"
#include "./arch/AMD64/navmInstructions.h"
#include "navmFrontend.h"
