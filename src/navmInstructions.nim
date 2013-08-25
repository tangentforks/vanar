# -----------------------------------------------------------------------------
# PROJECT:     navm
#
# DESCRIPTION: NAVM is an interpreter for the AVM Level 2 ISA. It is a MISC
#              oriented instruction-set architecture specially designed for
#              efficient interpretation with minimal ressource usage. AVM
#              is an extensible design allowing the compilation of up to
#              120 extended instruction at runtime in a platform-independent
#              way. It also featuring a very fast interpretation engine in
#              combination with a higly effective AOT compiler, generating
#              optimizated machine-code for common CPU architectures.  
#
# AUTHOR:      Matthias Schirm
# COPYRIGHT:   (C) 2103 Matthias Schirm
#
# LICENCE:     BSD STYLE, SEE LICENCE.TXT
#
#              Copyright (c) 2013, Matthias Schirm
#              All rights reserved.
#
#              Redistribution and use in source and binary forms, with or
#              without modification, are permitted provided that the
#              following conditions are met:
#
#              1. Redistributions of source code must retain the above
#                 copyright notice, this list of conditions and the
#                 following disclaimer.
#
#              2. Redistributions in binary form must reproduce the above
#                 copyright notice, this list of conditions and the
#                 following disclaimer in the documentation and/or other
#                 materials provided with the distribution.
#
#              3. Neither the name of the author nor the names of its
#                 contributors may be used to endorse or promote products
#                 derived from this software without specific prior
#                 written permission.
#
#              THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
#              CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#              INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
#              MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#              DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
#              CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
#              SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
#              NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
#              LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
#              HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
#              CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
#              OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#              SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# MODULE:      Routines implementing the AVM Level 2 ISA for AMD64 comp. CPU's
#
# HISTORY:     [2013.8 .8 ] first version
#              [2013.8 .12] bug fixes and testing
#              [2013.8. 25] further versions of Nimrod shall handle space
#                           delimitered parameter lists as tuples, reformatted
#                           the source code as preparation for further compiler
#                           versions
#
# TO-DO:       More error-checking and testing
#              Better documentation
# -----------------------------------------------------------------------------

import navmBackend, strutils

# cpu dependant type definations

type
  tNavmUCell* = uint64
  tNavmSCell* = int64

# Following constant definations declare mendatory CPU descriptions for the
# compiler. For ports to new processor architectures these constants must be
# changed to approbiate values

# The following constants specify characteristics of the CPU architecture:
#  cNavmCpuType:       Can either be set to CISC, RISC, MISC or VLIW
#  cNavmCpuDecoder:    Possible settings are out-of-order,in-order or serial
#  cfNavmCpuBigEndian: Endianess, true for big endian, false for little endian
#  cNavmCpuWordSize:   The word-size for values in byte
#  cNavmCpuAdrSize:    The address word-size in byte
#  cfNavmCompUnified:  True if there exist one unified compare instruction 

const
  cNavmCpuType*       = "CISC"
  cNavmCpuDecoder*    = "out-of-order"
  cfNavmCpuBigEndian* = false
  cNavmCpuWordSize*   = 8
  cNavmCpuAdrSize*    = 8
  cfNavmCompUnified*  = true

# These constants specify which NAVM instructions are avariable as direct
# immediate versions (bypassing stack adressing). Please note, these compiler-
# internal instructions can be compiled instead of there AVM equivalent for
# optimization purposes

const
  cfNavmImmInstLI*  = true
  cfNavmImmInstLX*  = false
  cfNavmImmInstLD*  = false
  cfNavmImmInstST*  = false
  cfNavmImmInstLXI* = false
  cfNavmImmInstSXI* = false
  cfNavmImmInstLXD* = false
  cfNavmImmInstSXD* = false
  cfNavmImmInstADD* = true 
  cfNavmImmInstADC* = true 
  cfNavmImmInstSUB* = true
  cfNavmImmInstSBC* = true
  cfNavmImmInstSLB* = true
  cfNavmImmInstSRB* = true
  cfNavmImmInstANB* = true
  cfNavmImmInstGOR* = true
  cfNavmImmInstXOB* = true
  cfNavmImmInstDUP* = false
  cfNavmImmInstDRP* = false
  cfNavmImmInstSWP* = false
  cfNavmImmInstOVR* = false
  cfNavmImmInstROT* = false
  cfNavmImmInstB*   = false
  cfNavmImmInstBS*  = false
  cfNavmImmInstBR*  = false
  cfNavmImmInstR*   = false
  cfNavmImmInstEQ*  = false
  cfNavmImmInstGR*  = false
  cfNavmImmInstLE*  = false
  cfNavmImmInstZR*  = false

# the word lenght for each immediate representation in byte, zero for all
# instructions which are not avariable in there immediate-direct representation

const
  cfNavmImmInstLenLI  = 8
  cfNavmImmInstLenLX  = 0
  cfNavmImmInstLenLD  = 0
  cfNavmImmInstLenST  = 0
  cfNavmImmInstLenLXI = 0
  cfNavmImmInstLenSXI = 0
  cfNavmImmInstLenLXD = 0
  cfNavmImmInstLenSXD = 0
  cfNavmImmInstLenADD = 4 
  cfNavmImmInstLenADC = 4    
  cfNavmImmInstLenSUB = 4    
  cfNavmImmInstLenSBC = 4   
  cfNavmImmInstLenSLB = 1    
  cfNavmImmInstLenSRB = 1    
  cfNavmImmInstLenANB = 4    
  cfNavmImmInstLenGOR = 4   
  cfNavmImmInstLenXOB = 4    
  cfNavmImmInstLenDUP = 0    
  cfNavmImmInstLenDRP = 0     
  cfNavmImmInstLenSWP = 0    
  cfNavmImmInstLenOVR = 0     
  cfNavmImmInstLenROT = 0   
  cfNavmImmInstLenB   = 0    
  cfNavmImmInstLenBS  = 0    
  cfNavmImmInstLenBR  = 0    
  cfNavmImmInstLenR   = 0     
  cfNavmImmInstLenEQ  = 0     
  cfNavmImmInstLenGR  = 0     
  cfNavmImmInstLenLE  = 0    
  cfNavmImmInstLenZR  = 0     

# some internal help routines

proc errorOFD =
  system.writeLN (stderr,"[navmInstructions] vD > 8")
  quit ()

proc errorUFD =
  system.writeLN (stderr,"[navmInstructions] vD < 1")
  quit ()

proc errorIMM =
  system.writeLN (stderr,"[navmInstructions] fNavmXImm != true")
  quit ()

proc errorREG =
  system.writeLN (stderr,"[navmInstructions] fNavmXImm != false")
  quit ()

proc disAsm (ob: var tNavmBackend,text: string,dis: bool) =
  if dis != false:
    var s: string = toHex (ob.oCodeMem,sizeof (ob.oCodeMem))
    system.write   (stderr,"vD: ")
    system.write   (stderr,ob.vD)
    system.write   (stderr," ")
    system.write   (stderr,s)
    system.write   (stderr,": ")
    system.writeLN (stderr,text)

proc disAsmImm[T] (ob: var tNavmBackend,text: string,val: T,dis: bool) =
  if dis != false:
    var s: string = toHex (ob.oCodeMem,sizeof (ob.oCodeMem))
    system.write (stderr,"vD: ")
    system.write (stderr,ob.vD)
    system.write (stderr," ")
    system.write (stderr,s)
    system.write (stderr,": ")
    system.write (stderr,text)

    var v: BiggestInt = cast[BiggestInt](val)
    s = toHex (v, sizeof (val))
    system.writeLN (stderr, s)

# these are the backend primitives for compiling AVM Level 2 instructions.
# Register mapping:

#   R0 (RAX) = TOS cache,
#   R1 (RBX) = Index register X
#   R2 (RCX) = scratch
#   R3 (RDX) = scratch
#   R7 (RSP) = adress stack pointer R
#   R8-R15   = data-stack D

proc li* (ob: var tNavmBackend,dis: bool) = errorREG ()

proc liImm* (ob: var tNavmBackend,val: int64,dis: bool) =
  var uVal: uint64 = cast[uint64](val)

  case ob.vD
    of 0:
       errorUFD ()
    of 1:
       disAsmImm[int64] (ob,"MOV  R0,  ",val,dis)
       ob.compile2 (0x48B8,cfNavmCpuBigEndian)
       ob.compile8 (uVal,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 2:
       disAsmImm[int64] (ob,"MOV  R8,  ",val,dis)
       ob.compile2 (0x49B8,cfNavmCpuBigEndian)
       ob.compile8 (uVal,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 3:
       disAsmImm[int64] (ob,"MOV  R9,  ",val,dis)
       ob.compile2 (0x49B9,cfNavmCpuBigEndian)
       ob.compile8 (uVal,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 4:
       disAsmImm[int64] (ob,"MOV  R10, ",val,dis)
       ob.compile2 (0x49BA,cfNavmCpuBigEndian)
       ob.compile8 (uVal,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 5:
       disAsmImm[int64] (ob,"MOV  R11, ",val,dis)
       ob.compile2 (0x49BB,cfNavmCpuBigEndian)
       ob.compile8 (uVal,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 6:
       disAsmImm[int64] (ob,"MOV  R12, ",val,dis)
       ob.compile2 (0x49BC,cfNavmCpuBigEndian)
       ob.compile8 (uVal,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 7:
       disAsmImm[int64] (ob,"MOV  R13, ",val,dis)
       ob.compile2 (0x49BD,cfNavmCpuBigEndian)
       ob.compile8 (uVal,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 8:
       disAsmImm[int64] (ob,"MOV  R14, ",val,dis)
       ob.compile2 (0x49BE,cfNavmCpuBigEndian)
       ob.compile8 (uVal,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    else:
       errorOFD ()

proc lx* (ob: var tNavmBackend,dis: bool) =
  ob.vD = ob.vD - 1
  case ob.vD
    of 0:
       errorUFD ()
    of 1:
       disAsm (ob,"MOV  R1,  R0",dis)
       ob.compile2 (0x4889,cfNavmCpuBigEndian)
       ob.compile  (0xC3)
       ob.vD = ob.vD + 1
    of 2:
       disAsm (ob,"MOV  R1,  R8",dis)
       ob.compile2 (0x4C89,cfNavmCpuBigEndian)
       ob.compile  (0xC3)
       ob.vD = ob.vD + 1
    of 3:
       disAsm (ob,"MOV  R1,  R9",dis)
       ob.compile2 (0x4C89,cfNavmCpuBigEndian)
       ob.compile  (0xCB)
       ob.vD = ob.vD + 1
    of 4:
       disAsm (ob,"MOV  R1,  R10",dis)
       ob.compile2 (0x4C89,cfNavmCpuBigEndian)
       ob.compile  (0xD3)
       ob.vD = ob.vD + 1
    of 5:
       disAsm (ob,"MOV  R1,  R11",dis)
       ob.compile2 (0x4C89,cfNavmCpuBigEndian)
       ob.compile  (0xDB)
       ob.vD = ob.vD + 1
    of 6:
       disAsm (ob,"MOV  R1,  R12",dis)
       ob.compile2 (0x4C89,cfNavmCpuBigEndian)
       ob.compile  (0xE3)
       ob.vD = ob.vD + 1
    of 7:
       disAsm (ob,"MOV  R1,  R13",dis)
       ob.compile2 (0x4C89,cfNavmCpuBigEndian)
       ob.compile  (0xEB)
       ob.vD = ob.vD + 1
    of 8:
       disAsm (ob,"MOV  R1,  R14",dis)
       ob.compile2 (0x4C89,cfNavmCpuBigEndian)
       ob.compile  (0xF3)
       ob.vD = ob.vD + 1
    else:
       errorOFD ()

proc lxImm (ob: var tNavmBackend,val: uint64,dis: bool) = errorIMM ()

proc ld* (ob: var tNavmBackend,dis: bool) =
  ob.vD = ob.vD - 1
  case ob.vD
    of 0:
       errorUFD ()
    of 1:
       disAsm (ob,"MOV  R0,  [R0]",dis)
       ob.compile2 (0x488B,cfNavmCpuBigEndian)
       ob.compile  (0x00)
       ob.vD = ob.vD + 1
    of 2:
       disAsm (ob,"MOV  R8,  [R8]",dis)
       ob.compile2 (0x4D8B,cfNavmCpuBigEndian)
       ob.compile  (0x00)
       ob.vD = ob.vD + 1
    of 3:
       disAsm (ob,"MOV  R9,  [R9]",dis)
       ob.compile2 (0x4D8B,cfNavmCpuBigEndian)
       ob.compile  (0x09)
       ob.vD = ob.vD + 1
    of 4:
       disAsm (ob,"MOV  R10, [R10]",dis)
       ob.compile2 (0x4D8B,cfNavmCpuBigEndian)
       ob.compile  (0x12)
       ob.vD = ob.vD + 1
    of 5:
       disAsm (ob,"MOV  R11, [R11]",dis)
       ob.compile2 (0x4D8B,cfNavmCpuBigEndian)
       ob.compile  (0x1B)
       ob.vD = ob.vD + 1
    of 6:
       disAsm (ob,"MOV  R12, [R12]",dis)
       ob.compile4 (0x4D8B2424,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 7:
       disAsm (ob,"MOV  R13, [R13]",dis)
       ob.compile4 (0x4D8B6D00,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 8:
       disAsm (ob,"MOV  R14, [R14]",dis)
       ob.compile2 (0x4D8B,cfNavmCpuBigEndian)
       ob.compile  (0x36)
       ob.vD = ob.vD + 1
    else:
       errorOFD ()

proc ldImm (ob: var tNavmBackend,val: uint64,dis: bool) = errorIMM ()

proc st* (ob: var tNavmBackend,dis: bool) =
  ob.vD = ob.vD - 2
  case ob.vD
    of 0:
       errorUFD ()
    of 1:
       disAsm (ob,"MOV *R8,  R0",dis)
       ob.compile2 (0x4989,cfNavmCpuBigEndian)
       ob.compile  (0x00)
       ob.vD = ob.vD + 1
    of 2:
       disAsm (ob,"MOV *R9,  R8",dis)
       ob.compile2 (0x4D89,cfNavmCpuBigEndian)
       ob.compile  (0x01)
       ob.vD = ob.vD + 1
    of 3:
       disAsm (ob,"MOV *R10, R9",dis)
       ob.compile2 (0x4D89,cfNavmCpuBigEndian)
       ob.compile  (0x0A)
       ob.vD = ob.vD + 1
    of 4:
       disAsm (ob,"MOV *R11, R10",dis)
       ob.compile2 (0x4D89,cfNavmCpuBigEndian)
       ob.compile  (0x13)
       ob.vD = ob.vD + 1
    of 5:
       disAsm (ob,"MOV *R12, R11",dis)
       ob.compile4 (0x4D891C24,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 6:
       disAsm (ob,"MOV *R13, R12",dis)
       ob.compile4 (0x4D896500,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 7:
       disAsm (ob,"MOV *R14, R13",dis)
       ob.compile2 (0x4D89,cfNavmCpuBigEndian)
       ob.compile  (0x2E)
       ob.vD = ob.vD + 1
    else:
       errorOFD ()

proc stImm (ob: var tNavmBackend,val: uint64,dis: bool) = errorIMM ()

proc lxi* (ob: var tNavmBackend,dis: bool) =
  case ob.vD
    of 0:
       errorUFD ()
    of 1:
       disAsm (ob,"MOV  R0, *R1  | INC R1",dis)
       ob.compile4 (0x488B0348,cfNavmCpuBigEndian)
       ob.compile2 (0xFFC3,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 2:
       disAsm (ob,"MOV  R8, *R1  | INC R1",dis)
       ob.compile4 (0x4C8B0348,cfNavmCpuBigEndian)
       ob.compile2 (0xFFC3,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 3:
       disAsm (ob,"MOV  R9, *R1  | INC R1",dis)
       ob.compile4 (0x4C8B0B48,cfNavmCpuBigEndian)
       ob.compile2 (0xFFC3,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 4:
       disAsm (ob,"MOV  R10, *R1 | INC R1",dis)
       ob.compile4 (0x4C8B1348,cfNavmCpuBigEndian)
       ob.compile2 (0xFFC3,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 5:
       disAsm (ob,"MOV  R11, *R1 | INC R1",dis)
       ob.compile4 (0x4C8B1B48,cfNavmCpuBigEndian)
       ob.compile2 (0xFFC3,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 6:
       disAsm (ob,"MOV  R12, *R1 | INC R1",dis)
       ob.compile4 (0x4C8B2348,cfNavmCpuBigEndian)
       ob.compile2 (0xFFC3,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 7:
       disAsm (ob,"MOV  R13, *R1 | INC R1",dis)
       ob.compile4 (0x4C8B2B48,cfNavmCpuBigEndian)
       ob.compile2 (0xFFC3,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 8:
       disAsm (ob,"MOV  R14, *R1 | INC R1",dis)
       ob.compile4 (0x4C8B3348,cfNavmCpuBigEndian)
       ob.compile2 (0xFFC3,cfNavmCpuBigEndian)
    else:
       errorOFD ()

proc lxiImm (ob: var tNavmBackend,val: uint64,dis: bool) = errorIMM ()

proc lxd* (ob: var tNavmBackend,dis: bool) =
  case ob.vD
    of 0:
       errorUFD ()
    of 1:
       disAsm (ob,"MOV  R0,  *R1  | DEC R1",dis)
       ob.compile4 (0x488B0348,cfNavmCpuBigEndian)
       ob.compile2 (0xFFCB,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 2:
       disAsm (ob,"MOV  R8,  *R1  | DEC R1",dis)
       ob.compile4 (0x4C8B0348,cfNavmCpuBigEndian)
       ob.compile2 (0xFFCB,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 3:
       disAsm (ob,"MOV  R9,  *R1  | DEC R1",dis)
       ob.compile4 (0x4C8B0B48,cfNavmCpuBigEndian)
       ob.compile2 (0xFFCB,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 4:
       disAsm (ob,"MOV  R10, *R1  | DEC R1",dis)
       ob.compile4 (0x4C8B1348,cfNavmCpuBigEndian)
       ob.compile2 (0xFFCB,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 5:
       disAsm (ob,"MOV  R11, *R1  | DEC R1",dis)
       ob.compile4 (0x4C8B1B48,cfNavmCpuBigEndian)
       ob.compile2 (0xFFCB,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 6:
       disAsm (ob,"MOV  R12, *R1  | DEC R1",dis)
       ob.compile4 (0x4C8B2348,cfNavmCpuBigEndian)
       ob.compile2 (0xFFCB,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 7:
       disAsm (ob,"MOV  R13, *R1  | DEC R1",dis)
       ob.compile4 (0x4C8B2B48,cfNavmCpuBigEndian)
       ob.compile2 (0xFFCB,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 8:
       disAsm (ob,"MOV  R14, *R1  | DEC R1",dis)
       ob.compile4 (0x4C8B3348,cfNavmCpuBigEndian)
       ob.compile2 (0xFFCB,cfNavmCpuBigEndian)
    else:
       errorOFD ()

proc lxdImm (ob: var tNavmBackend,val: uint64,dis: bool) = errorIMM ()

proc sxi* (ob: var tNavmBackend,dis: bool) =
  case ob.vD
    of 0:
       errorUFD ()
    of 1:
       disAsm (ob,"MOV *R1, R0  | INC R1",dis)
       ob.compile4 (0x48890348,cfNavmCpuBigEndian)
       ob.compile2 (0xFFC3,cfNavmCpuBigEndian)
       ob.vD = ob.vD - 1
    of 2:
       disAsm (ob,"MOV *R1, R8  | INC R1",dis)
       ob.compile4 (0x4C890348,cfNavmCpuBigEndian)
       ob.compile2 (0xFFC3,cfNavmCpuBigEndian)
       ob.vD = ob.vD - 1
    of 3:
       disAsm (ob,"MOV *R1, R9  | INC R1",dis)
       ob.compile4 (0x4C890B48,cfNavmCpuBigEndian)
       ob.compile2 (0xFFC3,cfNavmCpuBigEndian)
       ob.vD = ob.vD - 1
    of 4:
       disAsm (ob,"MOV *R1, R10 | INC R1",dis)
       ob.compile4 (0x4C891348,cfNavmCpuBigEndian)
       ob.compile2 (0xFFC3,cfNavmCpuBigEndian)
       ob.vD = ob.vD - 1
    of 5:
       disAsm (ob,"MOV *R1, R11 | INC R1",dis)
       ob.compile4 (0x4C891B48,cfNavmCpuBigEndian)
       ob.compile2 (0xFFC3,cfNavmCpuBigEndian)
       ob.vD = ob.vD - 1
    of 6:
       disAsm (ob,"MOV *R1, R12 | INC R1",dis)
       ob.compile4 (0x4C892348,cfNavmCpuBigEndian)
       ob.compile2 (0xFFC3,cfNavmCpuBigEndian)
       ob.vD = ob.vD - 1
    of 7:
       disAsm (ob,"MOV *R1, R13 | INC R1",dis)
       ob.compile4 (0x4C892B48,cfNavmCpuBigEndian)
       ob.compile2 (0xFFC3,cfNavmCpuBigEndian)
       ob.vD = ob.vD - 1
    of 8:
       disAsm (ob,"MOV *R1, R14 | INC R1",dis)
       ob.compile4 (0x4C893348,cfNavmCpuBigEndian)
       ob.compile2 (0xFFC3,cfNavmCpuBigEndian)
       ob.vD = ob.vD - 1
    else:
       errorOFD ()

proc sxiImm (ob: var tNavmBackend,val: uint64,dis: bool) = errorIMM ()

proc sxd* (ob: var tNavmBackend,dis: bool) =
  case ob.vD
    of 0:
       errorUFD ()
    of 1:
       disAsm (ob,"MOV *R1, R0  | DEC R1",dis)
       ob.compile4 (0x48890348,cfNavmCpuBigEndian)
       ob.compile2 (0xFFCB,cfNavmCpuBigEndian)
       ob.vD = ob.vD - 1
    of 2:
       disAsm (ob,"MOV *R1, R8  | DEC R1",dis)
       ob.compile4 (0x4C890348,cfNavmCpuBigEndian)
       ob.compile2 (0xFFCB,cfNavmCpuBigEndian)
       ob.vD = ob.vD - 1
    of 3:
       disAsm (ob,"MOV *R1, R9  | DEC R1",dis)
       ob.compile4 (0x4C890B48,cfNavmCpuBigEndian)
       ob.compile2 (0xFFCB,cfNavmCpuBigEndian)
       ob.vD = ob.vD - 1
    of 4:
       disAsm (ob,"MOV *R1, R10 | DEC R1",dis)
       ob.compile4 (0x4C891348,cfNavmCpuBigEndian)
       ob.compile2 (0xFFCB,cfNavmCpuBigEndian)
       ob.vD = ob.vD - 1
    of 5:
       disAsm (ob,"MOV *R1, R11 | DEC R1",dis)
       ob.compile4 (0x4C891B48,cfNavmCpuBigEndian)
       ob.compile2 (0xFFCB,cfNavmCpuBigEndian)
       ob.vD = ob.vD - 1
    of 6:
       disAsm (ob,"MOV *R1, R12 | DEC R1",dis)
       ob.compile4 (0x4C892348,cfNavmCpuBigEndian)
       ob.compile2 (0xFFCB,cfNavmCpuBigEndian)
       ob.vD = ob.vD - 1
    of 7:
       disAsm (ob,"MOV *R1, R13 | DEC R1",dis)
       ob.compile4 (0x4C892B48,cfNavmCpuBigEndian)
       ob.compile2 (0xFFCB,cfNavmCpuBigEndian)
       ob.vD = ob.vD - 1
    of 8:
       disAsm (ob,"MOV *R1, R14 | DEC R1",dis)
       ob.compile4 (0x4C893348,cfNavmCpuBigEndian)
       ob.compile2 (0xFFCB,cfNavmCpuBigEndian)
       ob.vD = ob.vD - 1
    else:
       errorOFD ()

proc sxdImm (ob: var tNavmBackend,val: uint64,dis: bool) = errorIMM ()

proc add* (ob: var tNavmBackend,dis: bool) =
  ob.vD = ob.vD - 2
  case ob.vD
    of 0:
       errorUFD ()
    of 1:
       disAsm (ob,"ADD  R0,  R8",dis)
       ob.compile2 (0x4C01,cfNavmCpuBigEndian)
       ob.compile  (0xC0)
       ob.vD = ob.vD + 1
    of 2:
       disAsm (ob,"ADD  R8,  R9",dis)
       ob.compile2 (0x4D01,cfNavmCpuBigEndian)
       ob.compile  (0xC8)
       ob.vD = ob.vD + 1
    of 3:
       disAsm (ob,"ADD  R9,  R10",dis)
       ob.compile2 (0x4D01,cfNavmCpuBigEndian)
       ob.compile  (0xD1)
       ob.vD = ob.vD + 1
    of 4:
       disAsm (ob,"ADD  R10, R11",dis)
       ob.compile2 (0x4D01,cfNavmCpuBigEndian)
       ob.compile  (0xDA)
       ob.vD = ob.vD + 1
    of 5:
       disAsm (ob,"ADD  R11, R12",dis)
       ob.compile2 (0x4D01,cfNavmCpuBigEndian)
       ob.compile  (0xE3)
       ob.vD = ob.vD + 1
    of 6:
       disAsm (ob,"ADD  R12, R13",dis)
       ob.compile2 (0x4D01,cfNavmCpuBigEndian)
       ob.compile  (0xEC)
       ob.vD = ob.vD + 1
    of 7:
       disAsm (ob,"ADD  R13, R14",dis)
       ob.compile2 (0x4D01,cfNavmCpuBigEndian)
       ob.compile  (0xF5)
       ob.vD = ob.vD + 1
    else:
       errorOFD ()

proc addImm (ob: var tNavmBackend,val: int32,dis: bool) =
  var uVal: uint32 = cast[uint32](val)
  ob.vD = ob.vD - 1

  case ob.vD
    of 0:
       errorUFD ()
    of 1:
       disAsmImm[uint32] (ob,"ADD  R0,  ",uVal,dis)
       ob.compile2 (0x4805,cfNavmCpuBigEndian)
       ob.compile4 (uVal,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 2:
       disAsmImm[uint32] (ob,"ADD  R8,  ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xC0)
       ob.compile4 (uVal,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 3:
       disAsmImm[uint32] (ob,"ADD  R9,  ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xC1)
       ob.compile4 (uVal,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 4:
       disAsmImm[uint32] (ob,"ADD  R10, ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xC2)
       ob.compile4 (uVal,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 5:
       disAsmImm[uint32] (ob,"ADD  R11, ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xC3)
       ob.compile4 (uVal,   cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 6:
       disAsmImm[uint32] (ob,"ADD  R12, ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xC4)
       ob.compile4 (uVal,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 7:
       disAsmImm[uint32] (ob,"ADD  R13, ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xC5)
       ob.compile4 (uVal,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 8:
       disAsmImm[uint32] (ob,"ADD  R14, ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xC6)
       ob.compile4 (uVal,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    else:
       errorOFD ()

proc adc* (ob: var tNavmBackend,dis: bool) =
  ob.vD = ob.Vd - 2
  case ob.vD
    of 0:
       errorUFD ()
    of 1:
       disAsm (ob,"ADC  R0,  R8",dis)
       ob.compile2 (0x4C11,cfNavmCpuBigEndian)
       ob.compile  (0xC0)
       ob.vD = ob.vD + 1
    of 2:
       disAsm (ob,"ADC  R8,  R9",dis)
       ob.compile2 (0x4D11,cfNavmCpuBigEndian)
       ob.compile  (0xC8)
       ob.vD = ob.vD + 1
    of 3:
       disAsm (ob,"ADC  R9,  R10",dis)
       ob.compile2 (0x4D11,cfNavmCpuBigEndian)
       ob.compile  (0xD1)
       ob.vD = ob.vD + 1
    of 4:
       disAsm (ob,"ADC  R10, R11",dis)
       ob.compile2 (0x4D11,cfNavmCpuBigEndian)
       ob.compile  (0xDA)
       ob.vD = ob.vD + 1
    of 5:
       disAsm (ob,"ADC  R11, R12",dis)
       ob.compile2 (0x4D11,cfNavmCpuBigEndian)
       ob.compile  (0xE3)
       ob.vD = ob.vD + 1
    of 6:
       disAsm (ob,"ADC  R12, R13",dis)
       ob.compile2 (0x4D11,cfNavmCpuBigEndian)
       ob.compile  (0xEC)
       ob.vD = ob.vD + 1
    of 7:
       disAsm (ob,"ADC  R13, R14",dis)
       ob.compile2 (0x4D11,cfNavmCpuBigEndian)
       ob.compile  (0xF5)
       ob.vD = ob.vD + 1
    else:
       errorOFD ()

proc adcImm (ob: var tNavmBackend,val: int32,dis: bool) =
  var uVal: uint32 = cast[uint32](val)
  ob.vD = ob.Vd - 1

  case ob.vD
    of 0:
       errorUFD ()
    of 1:
       disAsmImm[uint32] (ob,"ADD  R0,  ",uVal,dis)
       ob.compile2 (0x4815,cfNavmCpuBigEndian)
       ob.compile4 (uVal,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 2:
       disAsmImm[uint32] (ob,"ADD  R8,  ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xD0)
       ob.compile4 (uVal,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 3:
       disAsmImm[uint32] (ob,"ADD  R9,  ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xD1)
       ob.compile4 (uVal,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 4:
       disAsmImm[uint32] (ob,"ADD  R10, ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xD2)
       ob.compile4 (uVal,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 5:
       disAsmImm[uint32] (ob,"ADD  R11, ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xD3)
       ob.compile4 (uVal,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 6:
       disAsmImm[uint32] (ob,"ADD  R12, ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xD4)
       ob.compile4 (uVal,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 7:
       disAsmImm[uint32] (ob,"ADD  R13, ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xD5)
       ob.compile4 (uVal,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 8:
       disAsmImm[uint32] (ob,"ADD  R14, ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xD6)
       ob.compile4 (uVal,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    else:
       errorOFD ()

proc sub* (ob: var tNavmBackend,dis: bool) =
  ob.vD = ob.vD - 2
  case ob.vD
    of 0:
       errorUFD ()
    of 1:
       disAsm (ob,"SUB  R0,  R8",dis)
       ob.compile2 (0x4C29,cfNavmCpuBigEndian)
       ob.compile  (0xC0)
       ob.vD = ob.vD + 1
    of 2:
       disAsm (ob,"SUB  R8,  R9",dis)
       ob.compile2 (0x4D29,cfNavmCpuBigEndian)
       ob.compile  (0xC8)
       ob.vD = ob.vD + 1
    of 3:
       disAsm (ob,"SUB  R9,  R10",dis)
       ob.compile2 (0x4D29,cfNavmCpuBigEndian)
       ob.compile  (0xD1)
       ob.vD = ob.vD + 1
    of 4:
       disAsm (ob,"SUB  R10, R11",dis)
       ob.compile2 (0x4D29,cfNavmCpuBigEndian)
       ob.compile  (0xDA)
       ob.vD = ob.vD + 1
    of 5:
       disAsm (ob,"SUB  R11, R12",dis)
       ob.compile2 (0x4D29,cfNavmCpuBigEndian)
       ob.compile  (0xE3)
       ob.vD = ob.vD + 1
    of 6:
       disAsm (ob,"SUB  R12, R13",dis)
       ob.compile2 (0x4D29,cfNavmCpuBigEndian)
       ob.compile  (0xEC)
       ob.vD = ob.vD + 1
    of 7:
       disAsm (ob,"SUB  R13, R14",dis)
       ob.compile2 (0x4D29,cfNavmCpuBigEndian)
       ob.compile  (0xF5)
       ob.vD = ob.vD + 1
    else:
       errorOFD ()

proc subImm (ob: var tNavmBackend,val: int32,dis: bool) =
  var uVal: uint32 = cast[uint32](val)
  ob.vD = ob.vD - 1

  case ob.vD
    of 0:
       errorUFD ()
    of 1:
       disAsmImm[uint32] (ob,"SUB  R0,  ",uVal,dis)
       ob.compile2 (0x482D,cfNavmCpuBigEndian)
       ob.compile4 (uVal,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 2:
       disAsmImm[uint32] (ob,"SUB  R8,  ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xE8)
       ob.compile4 (uVal,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 3:
       disAsmImm[uint32] (ob,"SUB  R9,  ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xE9)
       ob.compile4 (uVal,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 4:
       disAsmImm[uint32] (ob,"SUB  R10, ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xEA)
       ob.compile4 (uVal,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 5:
       disAsmImm[uint32] (ob,"SUB  R11, ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xEB)
       ob.compile4 (uVal,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 6:
       disAsmImm[uint32] (ob,"SUB  R12, ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xEC)
       ob.compile4 (uVal,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 7:
       disAsmImm[uint32] (ob,"SUB  R13, ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xED)
       ob.compile4 (uVal,   cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 8:
       disAsmImm[uint32] (ob,"SUB  R14, ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xEE)
       ob.compile4 (uVal,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    else:
       errorOFD ()

proc sbc* (ob: var tNavmBackend,dis: bool) =
  ob.vD = ob.vD - 2
  case ob.vD
    of 0:
       errorUFD ()
    of 1:
       disAsm (ob,"SBB  R0,  R8",dis)
       ob.compile2 (0x4C19,cfNavmCpuBigEndian)
       ob.compile  (0xC0)
       ob.vD = ob.vD + 1
    of 2:
       disAsm (ob,"SBB  R8,  R9",dis)
       ob.compile2 (0x4D19,cfNavmCpuBigEndian)
       ob.compile  (0xC8)
       ob.vD = ob.vD + 1
    of 3:
       disAsm (ob,"SBB  R9,  R10",dis)
       ob.compile2 (0x4D19,cfNavmCpuBigEndian)
       ob.compile  (0xD1)
       ob.vD = ob.vD + 1
    of 4:
       disAsm (ob,"SBB  R10, R11",dis)
       ob.compile2 (0x4D19,cfNavmCpuBigEndian)
       ob.compile  (0xDA)
       ob.vD = ob.vD + 1
    of 5:
       disAsm (ob,"SBB  R11, R12",dis)
       ob.compile2 (0x4D19,cfNavmCpuBigEndian)
       ob.compile  (0xE3)
       ob.vD = ob.vD + 1
    of 6:
       disAsm (ob,"SBB  R12, R13",dis)
       ob.compile2 (0x4D19, cfNavmCpuBigEndian)
       ob.compile  (0xEC)
       ob.vD = ob.vD + 1
    of 7:
       disAsm (ob,"SBB  R13, R14",dis)
       ob.compile2 (0x4D19,cfNavmCpuBigEndian)
       ob.compile  (0xF5)
       ob.vD = ob.vD + 1
    else:
       errorOFD ()

proc sbcImm (ob: var tNavmBackend,val: int32,dis: bool) =
  var uVal: uint32 = cast[uint32](val)
  ob.vD = ob.vD - 1

  case ob.vD
    of 0:
       errorUFD ()
    of 1:
       disAsmImm[uint32] (ob,"SBB  R0,  ",uVal,dis)
       ob.compile2 (0x481D,cfNavmCpuBigEndian)
       ob.compile4 (uVal,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 2:
       disAsmImm[uint32] (ob,"SBB  R8,  ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xD8)
       ob.compile4 (uVal,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 3:
       disAsmImm[uint32] (ob,"SBB  R9,  ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xD9)
       ob.compile4 (uVal,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 4:
       disAsmImm[uint32] (ob,"SBB  R10, ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xDA)
       ob.compile4 (uVal,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 5:
       disAsmImm[uint32] (ob,"SBB  R11, ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xDB)
       ob.compile4 (uVal,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 6:
       disAsmImm[uint32] (ob,"SBB  R12, ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xDC)
       ob.compile4 (uVal,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 7:
       disAsmImm[uint32] (ob,"SBB  R13, ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xDD)
       ob.compile4 (uVal,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 8:
       disAsmImm[uint32] (ob,"SBB  R14, ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xDE)
       ob.compile4 (uVal,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    else:
       errorOFD ()

proc slb* (ob: var tNavmBackend,dis: bool) =
  ob.vD = ob.vD - 2
  case ob.vD
    of 0:
       errorUFD ()
    of 1:
       disAsm (ob,"MOV  B2,  B8  | SHL R0,  B2",dis)
       ob.compile4 (0x4488C148,cfNavmCpuBigEndian)
       ob.compile2 (0xD3E0,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 2:
       disAsm (ob,"MOV  B2,  B9  | SHL R8,  B2",dis)
       ob.compile4 (0x4488C949,cfNavmCpuBigEndian)
       ob.compile2 (0xD3E0,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 3:
       disAsm (ob,"MOV  B2,  B10 | SHL R9,  B2",dis)
       ob.compile4 (0x4488D149,cfNavmCpuBigEndian)
       ob.compile2 (0xD3E1,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 4:
       disAsm (ob,"MOV  B2,  B11 | SHL R10, B2",dis)
       ob.compile4 (0x4488D949,cfNavmCpuBigEndian)
       ob.compile2 (0xD3E2,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 5:
       disAsm (ob,"MOV  B2,  B12 | SHL R11, B2",dis)
       ob.compile4 (0x4488E149,cfNavmCpuBigEndian)
       ob.compile2 (0xD3E3,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 6:
       disAsm (ob,"MOV  B2,  B13 | SHL R12, B2",dis)
       ob.compile4 (0x4488E949,cfNavmCpuBigEndian)
       ob.compile2 (0xD3E4,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 7:
       disAsm (ob,"MOV  B2,  B14 | SHL R14, B2",dis)
       ob.compile4 (0x4488F149,cfNavmCpuBigEndian)
       ob.compile2 (0xD3E5,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    else:
       errorOFD ()

proc slbImm (ob: var tNavmBackend,val: int8,dis: bool) =
  var uVal: uint8 = cast[uint8](val)
  ob.vD = ob.vD - 1

  case ob.vD
    of 0:
       errorUFD ()
    of 1:
       disAsmImm[uint8] (ob, "SHL  R0,  ",uVal,dis)
       ob.compile2 (0x48C1,cfNavmCpuBigEndian)
       ob.compile  (0xE0)
       ob.compile  (uVal)
       ob.vD = ob.vD + 1
    of 2:
       disAsmImm[uint8] (ob,"SHL  R8,  ",uVal,dis)
       ob.compile2 (0x49C1,cfNavmCpuBigEndian)
       ob.compile  (0xE0)
       ob.compile  (uVal)
       ob.vD = ob.vD + 1
    of 3:
       disAsmImm[uint8] (ob,"SHL  R9,  ",uVal,dis)
       ob.compile2 (0x49C1,cfNavmCpuBigEndian)
       ob.compile  (0xE1)
       ob.compile  (uVal)
       ob.vD = ob.vD + 1
    of 4:
       disAsmImm[uint8] (ob,"SHL  R10, ",uVal,dis)
       ob.compile2 (0x49C1,cfNavmCpuBigEndian)
       ob.compile  (0xE2)
       ob.compile  (uVal)
       ob.vD = ob.vD + 1
    of 5:
       disAsmImm[uint8] (ob,"SHL  R11, ",uVal,dis)
       ob.compile2 (0x49C1,cfNavmCpuBigEndian)
       ob.compile  (0xE3)
       ob.compile  (uVal)
       ob.vD = ob.vD + 1
    of 6:
       disAsmImm[uint8] (ob,"SHL  R12, ",uVal,dis)
       ob.compile2 (0x49C1,cfNavmCpuBigEndian)
       ob.compile  (0xE4)
       ob.compile  (uVal)
       ob.vD = ob.vD + 1
    of 7:
       disAsmImm[uint8] (ob,"SHL  R13, ",uVal,dis)
       ob.compile2 (0x49C1,cfNavmCpuBigEndian)
       ob.compile  (0xE5)
       ob.compile  (uVal)
       ob.vD = ob.vD + 1
    of 8:
       disAsmImm[uint8] (ob,"SHL  R14, ",uVal,dis)
       ob.compile2 (0x49C1,cfNavmCpuBigEndian)
       ob.compile  (0xE6)
       ob.compile  (uVal)
       ob.vD = ob.vD + 1
    else:
       errorOFD ()

proc srb* (ob: var tNavmBackend,dis: bool) =
  ob.vD = ob.vD - 2
  case ob.vD
    of 0:
       errorUFD ()
    of 1:
       disAsm (ob,"MOV  B2,  B8  | SHR R0,  B2",dis)
       ob.compile4 (0x4488C148,cfNavmCpuBigEndian)
       ob.compile2 (0xD3E8,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 2:
       disAsm (ob,"MOV  B2,  B9  | SHR R8,  B2",dis)
       ob.compile4 (0x4488C949,cfNavmCpuBigEndian)
       ob.compile2 (0xD3E8,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 3:
       disAsm (ob,"MOV  B2,  B10 | SHR R9,  B2",dis)
       ob.compile4 (0x4488D149,cfNavmCpuBigEndian)
       ob.compile2 (0xD3E9,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 4:
       disAsm (ob,"MOV  B2,  B11 | SHR R10, B2",dis)
       ob.compile4 (0x4488D949,cfNavmCpuBigEndian)
       ob.compile2 (0xD3EA,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 5:
       disAsm (ob,"MOV  B2,  B12 | SHR R11, B2",dis)
       ob.compile4 (0x4488E149,cfNavmCpuBigEndian)
       ob.compile2 (0xD3EB,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 6:
       disAsm (ob,"MOV  B2,  B13 | SHR R12, B2",dis)
       ob.compile4 (0x4488E949,cfNavmCpuBigEndian)
       ob.compile2 (0xD3EC,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    of 7:
       disAsm (ob,"MOV  B2,  B14 | SHR R14, B2",dis)
       ob.compile4 (0x4488F149,cfNavmCpuBigEndian)
       ob.compile2 (0xD3ED,cfNavmCpuBigEndian)
       ob.vD = ob.vD + 1
    else:
       errorOFD ()

proc srbImm (ob: var tNavmBackend,val: int8,dis: bool) =
  var uVal: uint8 = cast[uint8](val)
  ob.vD = ob.vD - 1

  case ob.vD
    of 0:
       errorUFD ()
    of 1:
       disAsmImm[uint8] (ob,"SHR  R0,  ",uVal,dis)
       ob.compile2 (0x48C1,cfNavmCpuBigEndian)
       ob.compile  (0xE8)
       ob.compile  (uVal)
       ob.vD = ob.vD + 1
    of 2:
       disAsmImm[uint8] (ob,"SHR  R8,  ",uVal,dis)
       ob.compile2 (0x49C1,cfNavmCpuBigEndian)
       ob.compile  (0xE8)
       ob.compile  (uVal)
       ob.vD = ob.vD + 1
    of 3:
       disAsmImm[uint8] (ob,"SHR  R9,  ",uVal,dis)
       ob.compile2 (0x49C1,cfNavmCpuBigEndian)
       ob.compile  (0xE9)
       ob.compile  (uVal)
       ob.vD = ob.vD + 1
    of 4:
       disAsmImm[uint8] (ob,"SHR  R10, ",uVal,dis)
       ob.compile2 (0x49C1,cfNavmCpuBigEndian)
       ob.compile  (0xEA)
       ob.compile  (uVal)
       ob.vD = ob.vD + 1
    of 5:
       disAsmImm[uint8] (ob,"SHR  R11, ",uVal,dis)
       ob.compile2 (0x49C1,cfNavmCpuBigEndian)
       ob.compile  (0xEB)
       ob.compile  (uVal)
       ob.vD = ob.vD + 1
    of 6:
       disAsmImm[uint8] (ob,"SHR  R12, ",uVal,dis)
       ob.compile2 (0x49C1,cfNavmCpuBigEndian)
       ob.compile  (0xEC)
       ob.compile  (uVal)
       ob.vD = ob.vD + 1
    of 7:
       disAsmImm[uint8] (ob,"SHR  R13, ",uVal,dis)
       ob.compile2 (0x49C1,cfNavmCpuBigEndian)
       ob.compile  (0xED)
       ob.compile  (uVal)
       ob.vD = ob.vD + 1
    of 8:
       disAsmImm[uint8] (ob,"SHR  R14, ",uVal,dis)
       ob.compile2 (0x49C1,cfNavmCpuBigEndian)
       ob.compile  (0xEE)
       ob.compile  (uVal)
       ob.vD = ob.vD + 1
    else:
       errorOFD ()

proc anb* (ob: var tNavmBackend,dis: bool) =
  ob.vD = ob.vD - 2
  case ob.vD
    of 0:
       errorUFD ()
    of 1:
       disAsm (ob,"AND  R0,  R8",dis)
       ob.compile2 (0x4C21,cfNavmCpuBigEndian)
       ob.compile  (0xC0)
       ob.vD = ob.vD + 1
    of 2:
       disAsm (ob,"AND  R8,  R9",dis)
       ob.compile2 (0x4D21,cfNavmCpuBigEndian)
       ob.compile  (0xC8)
       ob.vD = ob.vD + 1
    of 3:
       disAsm (ob,"AND  R9,  R10",dis)
       ob.compile2 (0x4D21,cfNavmCpuBigEndian)
       ob.compile  (0xD1)
       ob.vD = ob.vD + 1
    of 4:
       disAsm (ob,"AND  R10, R11",dis)
       ob.compile2 (0x4D19,cfNavmCpuBigEndian)
       ob.compile  (0xDA)
       ob.vD = ob.vD + 1
    of 5:
       disAsm (ob,"AND  R11, R12",dis)
       ob.compile2 (0x4D21,cfNavmCpuBigEndian)
       ob.compile  (0xE3)
       ob.vD = ob.vD + 1
    of 6:
       disAsm (ob,"AND  R12, R13",dis)
       ob.compile2 (0x4D21,cfNavmCpuBigEndian)
       ob.compile  (0xEC)
       ob.vD = ob.vD + 1
    of 7:
       disAsm (ob,"AND  R13, R14",dis)
       ob.compile2 (0x4D21,cfNavmCpuBigEndian)
       ob.compile  (0xF5)
       ob.vD = ob.vD + 1
    else:
       errorOFD ()

proc anbImm (ob: var tNavmBackend,val: int8,dis: bool) =
  var uVal: uint8 = cast[uint8](val)
  ob.vD = ob.vD - 1

  case ob.vD
    of 0:
       errorUFD ()
    of 1:
       disAsmImm[uint8] (ob,"AND  R0,  ",uVal,dis)
       ob.compile2 (0x4825, cfNavmCpuBigEndian)
       ob.compile  (uVal)
       ob.vD = ob.vD + 1
    of 2:
       disAsmImm[uint8] (ob,"AND  R8,  ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xE0)
       ob.compile  (uVal)
       ob.vD = ob.vD + 1
    of 3:
       disAsmImm[uint8] (ob,"AND  R9,  ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xE1)
       ob.compile  (uVal)
       ob.vD = ob.vD + 1
    of 4:
       disAsmImm[uint8] (ob,"AND  R10, ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xE2)
       ob.compile  (uVal)
       ob.vD = ob.vD + 1
    of 5:
       disAsmImm[uint8] (ob,"AND  R11, ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xE3)
       ob.compile  (uVal)
       ob.vD = ob.vD + 1
    of 6:
       disAsmImm[uint8] (ob,"AND  R12, ",uVal,dis)
       ob.compile2 (0x4981, cfNavmCpuBigEndian)
       ob.compile  (0xE4)
       ob.compile  (uVal)
       ob.vD = ob.vD + 1
    of 7:
       disAsmImm[uint8] (ob,"AND  R13, ",uVal,dis)
       ob.compile2 (0x4981, cfNavmCpuBigEndian)
       ob.compile  (0xE5)
       ob.compile  (uVal)
       ob.vD = ob.vD + 1
    of 8:
       disAsmImm[uint8] (ob,"AND  R14, ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xE6)
       ob.compile  (uVal)
       ob.vD = ob.vD + 1
    else:
       errorOFD ()

proc gor* (ob: var tNavmBackend,dis: bool) =
  ob.vD = ob.vD - 2
  case ob.vD
    of 0:
       errorUFD ()
    of 1:
       disAsm (ob,"GOR  R0,  R8",dis)
       ob.compile2 (0x4C09,cfNavmCpuBigEndian)
       ob.compile  (0xC0)
       ob.vD = ob.vD + 1
    of 2:
       disAsm (ob,"GOR  R8,  R9",dis)
       ob.compile2 (0x4D09,cfNavmCpuBigEndian)
       ob.compile  (0xC8)
       ob.vD = ob.vD + 1
    of 3:
       disAsm (ob,"GOR  R9,  R10",dis)
       ob.compile2 (0x4D09,cfNavmCpuBigEndian)
       ob.compile  (0xD1)
       ob.vD = ob.vD + 1
    of 4:
       disAsm (ob,"GOR  R10, R11",dis)
       ob.compile2 (0x4D09,cfNavmCpuBigEndian)
       ob.compile  (0xDA)
       ob.vD = ob.vD + 1
    of 5:
       disAsm (ob,"GOR  R11, R12",dis)
       ob.compile2 (0x4D09,cfNavmCpuBigEndian)
       ob.compile  (0xE3)
       ob.vD = ob.vD + 1
    of 6:
       disAsm (ob,"GOR  R12, R13",dis)
       ob.compile2 (0x4D09,cfNavmCpuBigEndian)
       ob.compile  (0xEC)
       ob.vD = ob.vD + 1
    of 7:
       disAsm (ob,"GOR  R13, R14",dis)
       ob.compile2 (0x4D09,cfNavmCpuBigEndian)
       ob.compile  (0xF5)
       ob.vD = ob.vD + 1
    else:
       errorOFD ()

proc gorImm (ob: var tNavmBackend,val: int8,dis: bool) =
  var uVal: uint8 = cast[uint8](val)
  ob.vD = ob.vD - 1

  case ob.vD
    of 0:
       errorUFD ()
    of 1:
       disAsmImm[uint8] (ob,"GOR  R0,  ",uVal,dis)
       ob.compile2 (0x480D,cfNavmCpuBigEndian)
       ob.compile  (uVal)
       ob.vD = ob.vD + 1
    of 2:
       disAsmImm[uint8] (ob,"GOR  R8,  ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xC8)
       ob.compile  (uVal)
       ob.vD = ob.vD + 1
    of 3:
       disAsmImm[uint8] (ob,"GOR  R9,  ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xC9)
       ob.compile  (uVal)
       ob.vD = ob.vD + 1
    of 4:
       disAsmImm[uint8] (ob,"GOR  R10, ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xCA)
       ob.compile  (uVal)
       ob.vD = ob.vD + 1
    of 5:
       disAsmImm[uint8] (ob,"GOR  R11, ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xCB)
       ob.compile  (uVal)
       ob.vD = ob.vD + 1
    of 6:
       disAsmImm[uint8] (ob,"GOR  R12, ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xCC)
       ob.compile  (uVal)
       ob.vD = ob.vD + 1
    of 7:
       disAsmImm[uint8] (ob,"GOR  R13, ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xCD)
       ob.compile  (uVal)
       ob.vD = ob.vD + 1
    of 8:
       disAsmImm[uint8] (ob,"GOR  R14, ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xCE)
       ob.compile  (uVal)
       ob.vD = ob.vD + 1
    else:
       errorOFD ()

proc xob* (ob: var tNavmBackend,dis: bool) =
  ob.vD = ob.vD - 2
  case ob.vD
    of 0:
       errorUFD ()
    of 1:
       disAsm (ob,"XOR  R0,  R8",dis)
       ob.compile2 (0x4C31,cfNavmCpuBigEndian)
       ob.compile  (0xC0)
       ob.vD = ob.vD + 1
    of 2:
       disAsm (ob,"XOR  R8,  R9",dis)
       ob.compile2 (0x4D31,cfNavmCpuBigEndian)
       ob.compile  (0xC8)
       ob.vD = ob.vD + 1
    of 3:
       disAsm (ob,"XOR  R9,  R10",dis)
       ob.compile2 (0x4D31,cfNavmCpuBigEndian)
       ob.compile  (0xD1)
       ob.vD = ob.vD + 1
    of 4:
       disAsm (ob,"XOR  R10, R11",dis)
       ob.compile2 (0x4D31,cfNavmCpuBigEndian)
       ob.compile  (0xDA)
       ob.vD = ob.vD + 1
    of 5:
       disAsm (ob,"XOR  R11, R12",dis)
       ob.compile2 (0x4D31,cfNavmCpuBigEndian)
       ob.compile  (0xE3)
       ob.vD = ob.vD + 1
    of 6:
       disAsm (ob,"XOR  R12, R13",dis)
       ob.compile2 (0x4D31,cfNavmCpuBigEndian)
       ob.compile  (0xEC)
       ob.vD = ob.vD + 1
    of 7:
       disAsm (ob,"XOR  R13, R14",dis)
       ob.compile2 (0x4D31,cfNavmCpuBigEndian)
       ob.compile  (0xF5)
       ob.vD = ob.vD + 1
    else:
       errorOFD ()

proc xobImm (ob: var tNavmBackend,val: int8,dis: bool) =
  var uVal: uint8 = cast[uint8](val)
  ob.vD = ob.vD - 2

  case ob.vD
    of 0:
       errorUFD ()
    of 1:
       disAsmImm[uint8] (ob,"XOR  R0,  ",uVal,dis)
       ob.compile2 (0x4835,cfNavmCpuBigEndian)
       ob.compile  (uVal)
       ob.vD = ob.vD + 1
    of 2:
       disAsmImm[uint8] (ob,"XOR  R8,  ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xF0)
       ob.compile  (uVal)
       ob.vD = ob.vD + 1
    of 3:
       disAsmImm[uint8] (ob,"XOR  R9,  ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xF1)
       ob.compile  (uVal)
       ob.vD = ob.vD + 1
    of 4:
       disAsmImm[uint8] (ob,"XOR  R10, ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xF2)
       ob.compile  (uVal)
       ob.vD = ob.vD + 1
    of 5:
       disAsmImm[uint8] (ob,"XOR  R11, ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xF3)
       ob.compile  (uVal)
       ob.vD = ob.vD + 1
    of 6:
       disAsmImm[uint8] (ob,"XOR  R12, ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xF4)
       ob.compile  (uVal)
       ob.vD = ob.vD + 1
    of 7:
       disAsmImm[uint8] (ob,"XOR  R13, ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xF5)
       ob.compile  (uVal)
       ob.vD = ob.vD + 1
    of 8:
       disAsmImm[uint8] (ob,"XOR  R14, ",uVal,dis)
       ob.compile2 (0x4981,cfNavmCpuBigEndian)
       ob.compile  (0xF6)
       ob.compile  (uVal)
       ob.vD = ob.vD + 1
    else:
       errorOFD ()

proc dup* (ob: var tNavmBackend,dis: bool) =
  case ob.vD
    of 0:
       errorUFD ()
    of 1:
       disAsm (ob,"MOV  R8,  R0",dis)
       ob.compile2 (0x4989,cfNavmCpuBigEndian)
       ob.compile  (0xC0)
       ob.vD = ob.vD + 1
    of 2:
       disAsm (ob,"MOV  R9,  R8",dis)
       ob.compile2 (0x4D89,cfNavmCpuBigEndian)
       ob.compile  (0xC1)
       ob.vD = ob.vD + 1
    of 3:
       disAsm (ob,"MOV  R10, R9",dis)
       ob.compile2 (0x4D89,cfNavmCpuBigEndian)
       ob.compile  (0xCA)
       ob.vD = ob.vD + 1
    of 4:
       disAsm (ob,"MOV  R11, R10",dis)
       ob.compile2 (0x4D89,cfNavmCpuBigEndian)
       ob.compile  (0xD3)
       ob.vD = ob.vD + 1
    of 5:
       disAsm (ob,"MOV  R12, R11",dis)
       ob.compile2 (0x4D89,cfNavmCpuBigEndian)
       ob.compile  (0xDC)
       ob.vD = ob.vD + 1
    of 6:
       disAsm (ob,"MOV  R13, R12",dis)
       ob.compile2 (0x4D89,cfNavmCpuBigEndian)
       ob.compile  (0xE5)
       ob.vD = ob.vD + 1
    of 7:
       disAsm (ob,"MOV  R14, R13",dis)
       ob.compile2 (0x4D89,cfNavmCpuBigEndian)
       ob.compile  (0xEE)
       ob.vD = ob.vD + 1
    else:
       errorOFD ()

proc drp* (ob: var tNavmBackend,dis: bool) =
  ob.vD = ob.vD - 1

proc swp* (ob: var tNavmBackend,dis: bool) =
  case ob.vD
    of 0:
       errorUFD ()
    of 1:
       disAsm (ob,"XCHG R8,  R0",dis)
       ob.compile2 (0x4990,cfNavmCpuBigEndian)
    of 2:
       disAsm (ob,"XCHG R9,  R8",dis)
       ob.compile2 (0x4D87,cfNavmCpuBigEndian)
       ob.compile  (0xC8)
    of 3:
       disAsm (ob,"XCHG R10, R9",dis)
       ob.compile2 (0x4D87,cfNavmCpuBigEndian)
       ob.compile  (0xD1)
    of 4:
       disAsm (ob,"XCHG R11, R10",dis)
       ob.compile2 (0x4D87, cfNavmCpuBigEndian)
       ob.compile  (0xDA)
    of 5:
       disAsm (ob,"XCHG R12, R11",dis)
       ob.compile2 (0x4D87, cfNavmCpuBigEndian)
       ob.compile  (0xE3)
    of 6:
       disAsm (ob,"XCHG R13, R12",dis)
       ob.compile2 (0x4D87,cfNavmCpuBigEndian)
       ob.compile  (0xEC)
    of 7:
       disAsm (ob,"XCHG R14, R13",dis)
       ob.compile2 (0x4D87,cfNavmCpuBigEndian)
       ob.compile  (0xE5)
    else:
       errorOFD ()
       
proc ovr* (ob: var tNavmBackend,dis: bool) =
  case ob.vD
    of 0:
       errorUFD ()
    of 1:
       disAsm (ob,"MOV  R9,  R0",dis)
       ob.compile2 (0x4989,cfNavmCpuBigEndian)
       ob.Compile  (0xC1)
       ob.vD = ob.vD + 1
    of 2:
       disAsm (ob,"MOV  R10, R8",dis)
       ob.compile2 (0x4D89,cfNavmCpuBigEndian)
       ob.compile  (0xC2)
       ob.vD = ob.vD + 1
    of 3:
       disAsm (ob,"MOV  R11, R9",dis)
       ob.compile2 (0x4D89,cfNavmCpuBigEndian)
       ob.compile  (0xCB)
       ob.vD = ob.vD + 1
    of 4:
       disAsm (ob,"MOV  R12, R10",dis)
       ob.compile2 (0x4D89,cfNavmCpuBigEndian)
       ob.compile  (0xD4)
       ob.vD = ob.vD + 1
    of 5:
       disAsm (ob,"MOV  R13, R11",dis)
       ob.compile2 (0x4D89,cfNavmCpuBigEndian)
       ob.compile  (0xDD)
       ob.vD = ob.vD + 1
    of 6:
       disAsm (ob,"MOV  R14, R12",dis)
       ob.compile2 (0x4D89,cfNavmCpuBigEndian)
       ob.compile  (0xE6)
       ob.vD = ob.vD + 1
    else:
       errorOFD ()

proc rot* (ob: var tNavmBackend,dis: bool) =
  case ob.vD
    of 0:
       errorUFD ()
    of 1:
       disAsm (ob,"XCHG R9,  R0",dis)
       ob.compile2 (0x4991,cfNavmCpuBigEndian)
    of 2:
       disAsm (ob,"XCHG R10, R8",dis)
       ob.compile2 (0x4D87,cfNavmCpuBigEndian)
       ob.compile  (0xD0)
    of 3:
       disAsm (ob,"XCHG R11, R9",dis)
       ob.compile2 (0x4D87,cfNavmCpuBigEndian)
       ob.compile  (0xD9)
    of 4:
       disAsm (ob,"XCHG R12, R10",dis)
       ob.compile2 (0x4D87,cfNavmCpuBigEndian)
       ob.compile  (0xE2)
    of 5:
       disAsm (ob,"XCHG R13, R11",dis)
       ob.compile2 (0x4D87,cfNavmCpuBigEndian)
       ob.compile  (0xEB)
    of 6:
       disAsm (ob,"XCHG R14, R12",dis)
       ob.compile2 (0x4D87,cfNavmCpuBigEndian)
       ob.compile  (0xF4)
    else:
       errorOFD ()

proc b* (ob: var tNavmBackend,dis: bool) =
  case ob.vD
    of 0:
       errorUFD ()
    of 1:
       disAsm (ob,"JMP  R0",dis)
       ob.compile2 (0xFFE0,cfNavmCpuBigEndian)
    of 2:
       disAsm (ob,"JMP  R8",dis)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xE0)
    of 3:
       disAsm (ob,"JMP  R9",dis)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xE1)
    of 4:
       disAsm (ob,"JMP  R10",dis)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xE2)
    of 5:
       disAsm (ob,"JMP  R11",dis)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xE3)
    of 6:
       disAsm (ob,"JMP  R12",dis)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xE4)
    of 7:
       disAsm (ob,"JMP  R13",dis)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xE5)
    of 8:
       disAsm (ob,"JMP  R14",dis)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xE6)
    else:
       errorOFD ()

proc beq* (ob: var tNavmBackend,dis: bool) =
  case ob.vD
    of 0:
       errorUFD ()
    of 1:
       disAsm (ob,"JNE  +2  | JMP  R0",dis)
       ob.compile2 (0x7502,cfNavmCpuBigEndian)
       ob.compile2 (0xFFE0,cfNavmCpuBigEndian)
    of 2:
       disAsm (ob,"JNE  +3  | JMP  R8",dis)
       ob.compile2 (0x7503,cfNavmCpuBigEndian)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xE0)
       ob.vD = 3
    of 3:
       disAsm (ob,"JNE  +3  | JMP  R9",dis)
       ob.compile2 (0x7503,cfNavmCpuBigEndian)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xE1)
    of 4:
       disAsm (ob,"JNE  +3  | JMP  R10",dis)
       ob.compile2 (0x7503,cfNavmCpuBigEndian)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xE2)
    of 5:
       disAsm (ob,"JNE  +3  | JMP  R11",dis)
       ob.compile2 (0x7503,cfNavmCpuBigEndian)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xE3)
    of 6:
       disAsm (ob,"JNE  +3  | JMP  R12",dis)
       ob.compile2 (0x7503,cfNavmCpuBigEndian)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xE4)
    of 7:
       disAsm (ob,"JNE  +3  | JMP  R13",dis)
       ob.compile2 (0x7503,cfNavmCpuBigEndian)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xE5)
    of 8:
       disAsm (ob,"JNE  +3  | JMP  R14",dis)
       ob.compile2 (0x7503,cfNavmCpuBigEndian)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xE6)
    else:
       errorOFD ()

proc bgr* (ob: var tNavmBackend,dis: bool) =
  case ob.vD
    of 0:
       errorUFD ()
    of 1:
       disAsm (ob,"JNG  +2  | JMP  R0",dis)
       ob.compile2 (0x7E02,cfNavmCpuBigEndian)
       ob.compile2 (0xFFE0,cfNavmCpuBigEndian)
    of 2:
       disAsm (ob,"JNG  +3  | JMP  R8",dis)
       ob.compile2 (0x7E03,cfNavmCpuBigEndian)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xE0)
    of 3:
       disAsm (ob,"JNG  +3  | JMP  R9",dis)
       ob.compile2 (0x7E03,cfNavmCpuBigEndian)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xE1)
    of 4:
       disAsm (ob,"JNG  +3  | JMP  R10",dis)
       ob.compile2 (0x7E03,cfNavmCpuBigEndian)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xE2)
    of 5:
       disAsm (ob,"JNG  +3  | JMP  R11",dis)
       ob.compile2 (0x7E03,cfNavmCpuBigEndian)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xE3)
    of 6:
       disAsm (ob,"JNG  +3  | JMP  R12",dis)
       ob.compile2 (0x7E03,cfNavmCpuBigEndian)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xE4)
    of 7:
       disAsm (ob,"JNG  +3  | JMP  R13",dis)
       ob.compile2 (0x7E03,cfNavmCpuBigEndian)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xE5)
    of 8:
       disAsm (ob,"JNG  +3  | JMP  R14",dis)
       ob.compile2 (0x7E03,cfNavmCpuBigEndian)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xE6)
    else:
       errorOFD ()

proc ble* (ob: var tNavmBackend,dis: bool) =
  case ob.vD
    of 0:
       errorUFD ()
    of 1:
       disAsm (ob,"JNL  +2  | JMP  R0",dis)
       ob.compile2 (0x7D02,cfNavmCpuBigEndian)
       ob.compile2 (0xFFE0,cfNavmCpuBigEndian)
    of 2:
       disAsm (ob,"JNL  +3  | JMP  R8",dis)
       ob.compile2 (0x7D03,cfNavmCpuBigEndian)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xE0)
    of 3:
       disAsm (ob,"JNL  +3  | JMP  R9",dis)
       ob.compile2 (0x7D03,cfNavmCpuBigEndian)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xE1)
    of 4:
       disAsm (ob,"JNL  +3  | JMP  R10",dis)
       ob.compile2 (0x7D03,cfNavmCpuBigEndian)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xE2)
    of 5:
       disAsm (ob,"JNL  +3  | JMP  R11",dis)
       ob.compile2 (0x7D03,cfNavmCpuBigEndian)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xE3)
    of 6:
       disAsm (ob,"JNL  +3  | JMP  R12",dis)
       ob.compile2 (0x7D03,cfNavmCpuBigEndian)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xE4)
    of 7:
       disAsm (ob,"JNL  +3  | JMP  R13",dis)
       ob.compile2 (0x7D03,cfNavmCpuBigEndian)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xE5)
    of 8:
       disAsm (ob,"JNL  +3  | JMP  R14",dis)
       ob.compile2 (0x7D03,cfNavmCpuBigEndian)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xE6)
    else:
       errorOFD ()

proc bzr* (ob: var tNavmBackend,dis: bool) = beq (ob,dis)

proc bs* (ob: var tNavmBackend,dis: bool) =
  case ob.vD
    of 0:
       errorUFD ()
    of 1:
       disAsm (ob,"CALL R0",dis)
       ob.compile2 (0xFFD0,cfNavmCpuBigEndian)
    of 2:
       disAsm (ob,"CALL R8",dis)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xD0)
    of 3:
       disAsm (ob,"CALL R9",dis)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xD1)
    of 4:
       disAsm (ob,"CALL R10",dis)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xD2)
    of 5:
       disAsm (ob,"CALL R11",dis)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xD3)
    of 6:
       disAsm (ob,"CALL R12",dis)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xD4)
    of 7:
       disAsm (ob,"CALL R13",dis)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xD5)
    of 8:
       disAsm (ob,"CALL R14",dis)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xD6)
    else:
       errorOFD ()

proc bseq* (ob: var tNavmBackend,dis: bool) =
  case ob.vD
    of 0:
       errorUFD ()
    of 1:
       disAsm (ob,"JNE  +2  | CALL R0",dis)
       ob.compile2 (0x7502,cfNavmCpuBigEndian)
       ob.compile2 (0xFFD0,cfNavmCpuBigEndian)
    of 2:
       disAsm (ob,"JNE  +3  | CALL R8",dis)
       ob.compile2 (0x7503,cfNavmCpuBigEndian)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xD0)
    of 3:
       disAsm (ob,"JNE  +3  | CALL R9",dis)
       ob.compile2 (0x7503,cfNavmCpuBigEndian)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xD1)
    of 4:
       disAsm (ob,"JNE  +3  | CALL R10",dis)
       ob.compile2 (0x7503,cfNavmCpuBigEndian)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xD2)
    of 5:
       disAsm (ob,"JNE  +3  | CALL R11",dis)
       ob.compile2 (0x7503,cfNavmCpuBigEndian)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xD3)
    of 6:
       disAsm (ob,"JNE  +3  | CALL R12",dis)
       ob.compile2 (0x7503,cfNavmCpuBigEndian)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xD4)
    of 7:
       disAsm (ob,"JNE  +3  | CALL R13",dis)
       ob.compile2 (0x7503,cfNavmCpuBigEndian)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xD5)
    of 8:
       disAsm (ob,"JNE  +3  | CALL R14",dis)
       ob.compile2 (0x7503,cfNavmCpuBigEndian)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xD6)
    else:
       errorOFD ()

proc bsgr* (ob: var tNavmBackend,dis: bool) =
  case ob.vD
    of 0:
       errorUFD ()
    of 1:
       disAsm (ob,"JNG  +2  | CALL R0",dis)
       ob.compile2 (0x7E02,cfNavmCpuBigEndian)
       ob.compile2 (0xFFD0,cfNavmCpuBigEndian)
    of 2:
       disAsm (ob,"JNG  +3  | CALL R8",dis)
       ob.compile2 (0x7E03,cfNavmCpuBigEndian)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xD0)
    of 3:
       disAsm (ob,"JNG  +3  | CALL R9",dis)
       ob.compile2 (0x7E03,cfNavmCpuBigEndian)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xD1)
    of 4:
       disAsm (ob,"JNG  +3  | CALL R10",dis)
       ob.compile2 (0x7E03,cfNavmCpuBigEndian)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xD2)
    of 5:
       disAsm (ob,"JNG  +3  | CALL R11",dis)
       ob.compile2 (0x7E03,cfNavmCpuBigEndian)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xD3)
    of 6:
       disAsm (ob,"JNG  +3  | CALL R12",dis)
       ob.compile2 (0x7E03,cfNavmCpuBigEndian)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xD4)
    of 7:
       disAsm (ob,"JNG  +3  | CALL R13",dis)
       ob.compile2 (0x7E03,cfNavmCpuBigEndian)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xD5)
    of 8:
       disAsm (ob,"JNG  +3  | CALL R14",dis)
       ob.compile2 (0x7E03,cfNavmCpuBigEndian)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xD6)
    else:
       errorOFD ()

proc bsle* (ob: var tNavmBackend,dis: bool) =
  case ob.vD
    of 0:
       errorUFD ()
    of 1:
       disAsm (ob,"JNG  +2  | CALL R0",dis)
       ob.compile2 (0x7D02,cfNavmCpuBigEndian)
       ob.compile2 (0xFFD0,cfNavmCpuBigEndian)
    of 2:
       disAsm (ob,"JNG  +3  | CALL R8",dis)
       ob.compile2 (0x7D03,cfNavmCpuBigEndian)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xD0)
    of 3:
       disAsm (ob,"JNG  +3  | CALL R9",dis)
       ob.compile2 (0x7D03,cfNavmCpuBigEndian)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xD1)
    of 4:
       disAsm (ob,"JNG  +3  | CALL R10",dis)
       ob.compile2 (0x7D03,cfNavmCpuBigEndian)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xD2)
    of 5:
       disAsm (ob,"JNG  +3  | CALL R11",dis)
       ob.compile2 (0x7D03,cfNavmCpuBigEndian)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xD3)
    of 6:
       disAsm (ob,"JNG  +3  | CALL R12",dis)
       ob.compile2 (0x7D03,cfNavmCpuBigEndian)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xD4)
    of 7:
       disAsm (ob,"JNG  +3  | CALL R13",dis)
       ob.compile2 (0x7D03,cfNavmCpuBigEndian)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xD5)
    of 8:
       disAsm (ob,"JNG  +3  | CALL R14",dis)
       ob.compile2 (0x7D03,cfNavmCpuBigEndian)
       ob.compile2 (0x41FF,cfNavmCpuBigEndian)
       ob.compile  (0xD6)
    else:
       errorOFD ()

proc bszr* (ob: var tNavmBackend,dis: bool) = bseq (ob,dis)

proc br* (ob: var tNavmBackend,dis: bool) =
  disAsm (ob,"RET",dis)
  ob.compile (0xC3)

proc breq* (ob: var tNavmBackend,dis: bool) =
  disAsm (ob,"JNE  +1  | RET",dis)
  ob.compile2 (0x7501,cfNavmCpuBigEndian)
  ob.compile (0xC3)

proc brgr* (ob: var tNavmBackend,dis: bool) =
  disAsm (ob,"JNG  +1  | RET",dis)
  ob.compile2 (0x7E01,cfNavmCpuBigEndian)
  ob.compile (0xC3)

proc brle* (ob: var tNavmBackend,dis: bool) =
  disAsm (ob,"JNL  +1  | RET",dis)
  ob.compile2 (0x7D01,cfNavmCpuBigEndian)
  ob.compile (0xC3)

proc brzr* (ob: var tNavmBackend,dis: bool) = breq (ob,dis)

proc r* (ob: var tNavmBackend,dis: bool) =
  system.writeLN (stderr,"[R] unimplemented")
  quit ()

proc req* (ob: var tNavmBackend,dis: bool) =
  system.writeLN (stderr,"[REQ] unimplemented")
  quit ()

proc rgr* (ob: var tNavmBackend,dis: bool) =
  system.writeLN (stderr,"[RGR] unimplemented")
  quit ()

proc rle* (ob: var tNavmBackend,dis: bool) =
  system.writeLN (stderr,"[RLE] unimplemented")
  quit ()

proc rzr* (ob: var tNavmBackend,dis: bool) =
  system.writeLN (stderr,"[RZR] unimplemented")
  quit ()

proc eq* (ob: var tNavmBackend,dis: bool) =
  case ob.vD
    of 0:
       errorUFD ()
    of 1:
       disAsm (ob,"CMP  R0,  R8",dis)
       ob.compile2 (0x4C39,cfNavmCpuBigEndian)
       ob.compile  (0xC0)
    of 2:
       disAsm (ob,"CMP  R8,  R9",dis)
       ob.compile2 (0x4D39,cfNavmCpuBigEndian)
       ob.compile  (0xC8)
    of 3:
       disAsm (ob,"CMP  R9,  R10",dis)
       ob.compile2 (0x4D39,cfNavmCpuBigEndian)
       ob.compile  (0xD1)
    of 4:
       disAsm (ob,"CMP  R10, R11",dis)
       ob.compile2 (0x4D39,cfNavmCpuBigEndian)
       ob.compile  (0xDA)
    of 5:
       disAsm (ob,"CMP  R11, R12",dis)
       ob.compile2 (0x4D39,cfNavmCpuBigEndian)
       ob.compile  (0xE3)
    of 6:
       disAsm (ob,"CMP  R12, R13",dis)
       ob.compile2 (0x4D39,cfNavmCpuBigEndian)
       ob.compile  (0xEC)
    of 7:
       disAsm (ob,"CMP  R13, R14",dis)
       ob.compile2 (0x4D39,cfNavmCpuBigEndian)
       ob.compile  (0xF5)
    else:
       errorOFD ()

proc gr* (ob: var tNavmBackend,dis: bool) = eq (ob,dis)
proc le* (ob: var tNavmBackend,dis: bool) = eq (ob,dis)
proc zr* (ob: var tNavmBackend,dis: bool) = eq (ob,dis)





when isMainModule:
  var test: tNavmBackend
  test.init (0xFFFF)
  
  test.liImm (0x101,true)
  echo ("oCodeMem: ",test.oCodeMem)

  test.release ()

