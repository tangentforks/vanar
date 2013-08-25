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
# MODULE:      Platform independent low-level interface for compiling
#              machine-code. The processor dependent routines extending these
#              interface (navmInstructions.nim)
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

import posix, shmBuffer, unsigned

# ----------------------------------------------------------------tNavmBackend-
# DESCRIPTION: To ease porting, the compiler is seperated into different
#              layers. There exist only a small, platform-dependent interface
#              (navmInstructions.nim) which implements the AVM Level 2
#              instruction-set architecture as *extension* of a platftorm-
#              independent backend object (these file). From this, a frontend
#              interface encapsulate different compiler for supported
#              optimization strategies. Please note, the backend object hold
#              the whole,internal dynamic state for a specific compiler
#              process. Because there exist no side-effects each backend
#              object can be handled independent and processed parallel.
#
# BUGS:        [2013.8 .23] none found
#
# TO-DO:       Implement better exception handling
# -----------------------------------------------------------------------------
# VARIABLES:
#
#           vD - virtual, dynamic data-stack state at compilation. The 
#                compiler frontend uses this value for mapping the actual
#                stack reference to a set of registers. NAVM define a
#                small data-stack depth of 8 elements, so vD can hold
#                values from 1-8. Value 0 signaled stack underflow,
#                vD > 8 stack overflow.
#           vR - Like vD, these state variable is used for mapping the
#                64 entry return-stack. Because all return adresses are
#                known at compile-time the frontend compiler is able to
#                convert subroutine calls into direct branches if useful
#     tCodeMem - static SHM buffer for compiled machine code (static
#                allocated memory is important for embedded and bare-metal
#                usages where often no operating system with dynamic memory
#                management exist)
#     oCodeMem - offset into pCodeMem, referencing the first free element
#                in buffer
#        fInit - true if the object-instance was initiated by proc init, used
#                for catching access to unitialized objects
#
# INTERFACE:
#
#   proc init
#     (ob: var tNavmBackend, memSize: int)
#       Constructor for initiating a shared memory-area as buffer for generated
#       machine code and inital state setting
#   proc release
#     (ob: var tNavmBackend)
#       Destructor for releasing the SHM buffer. Beware some operating systems
#       are otherwise unable to free the priour allocated memory area (or at
#       least mark it as free) !
#   proc compile
#     (ob: var tNavmBackend, val: uint8)
#       Write a single byte into machine-code buffer at current offset
#   proc compile2
#     (ob: var tNavmBackend, val: uint16, bigEndian: bool)
#       Write two bytes into machine-code buffer at current offset
#   proc compile4
#     (ob: var tNavmBackend, val: uint32, bigEndian: bool)
#       Write four bytes into buffer...
#   proc compile8
#    (ob: var tNavmBackend, val: uint64, bigEndian: bool)
#       Write eight bytes into buffer...
# -----------------------------------------------------------------------------

type tNavmBackend* = object of TObject
                       vD*: int8
                       vR*: int8
                       tCodeMem:  tShmBuffer[uint8]
                       oCodeMem*: int
                       fInit*:    bool

proc init* (ob: var tNavmBackend, memSize: int) =
  assert (ob.fInit != true)
  shmInitExec[uint8] (ob.tCodeMem, memSize)

  ob.oCodeMem = 0
  ob.fInit    = true
  ob.vD = 1
  ob.vR = 1

proc release* (ob: var tNavmBackend) =
  assert (ob.fInit != false)
  shmRelease[uint8] (ob.tCodeMem)
  ob.fInit = false

proc compile* (ob: var tNavmBackend,val: uint8) =
  assert (ob.fInit != false)
  shmPoke[uint8] (ob.tCodeMem,ob.oCodeMem,val)
  ob.oCodeMem = ob.oCodeMem + 1

proc compile2* (ob: var tNavmBackend,val: uint16,bigEndian: bool) =
  assert (ob.fInit != false)
  if bigEndian != false:
     let b1: uint8 = cast[uint8]((val shr 8) and 0xFF)
     let b2: uint8 = cast[uint8](val  and 0xFF)
     shmPoke[uint8] (ob.tCodeMem,ob.oCodeMem,b2)
     shmPoke[uint8] (ob.tCodeMem,ob.oCodeMem+1,b1)
     ob.oCodeMem = ob.oCodeMem + 2
  else:
     let b1: uint8 = cast[uint8](val  and 0xFF)
     let b2: uint8 = cast[uint8]((val shr 8) and 0xFF)
     shmPoke[uint8] (ob.tCodeMem,ob.oCodeMem,b2)
     shmPoke[uint8] (ob.tCodeMem,ob.oCodeMem+1,b1)
     ob.oCodeMem = ob.oCodeMem + 2
     
proc compile4* (ob: var tNavmBackend,val: uint32,bigEndian: bool) =
  assert (ob.fInit != false)
  if bigEndian != false:
     let b1: uint8 = cast[uint8]((val shr 24) and 0xFF)
     let b2: uint8 = cast[uint8]((val shr 16) and 0xFF)
     let b3: uint8 = cast[uint8]((val shr 8)  and 0xFF)
     let b4: uint8 = cast[uint8](val  and 0xFF)
     shmPoke[uint8] (ob.tCodeMem,ob.oCodeMem,b4)
     shmPoke[uint8] (ob.tCodeMem,ob.oCodeMem+1,b3)
     shmPoke[uint8] (ob.tCodeMem,ob.oCodeMem+2,b2)
     shmPoke[uint8] (ob.tCodeMem,ob.oCodeMem+3,b1)
     ob.oCodeMem = ob.oCodeMem + 4
  else:
     let b1: uint8 = cast[uint8](val  and 0xFF)
     let b2: uint8 = cast[uint8]((val shr 8)  and 0xFF)
     let b3: uint8 = cast[uint8]((val shr 16) and 0xFF)
     let b4: uint8 = cast[uint8]((val shr 24) and 0xFF)
     shmPoke[uint8] (ob.tCodeMem,ob.oCodeMem,b4)
     shmPoke[uint8] (ob.tCodeMem,ob.oCodeMem+1,b3)
     shmPoke[uint8] (ob.tCodeMem,ob.oCodeMem+2,b2)
     shmPoke[uint8] (ob.tCodeMem,ob.oCodeMem+3,b1)
     ob.oCodeMem = ob.oCodeMem + 4

proc compile8* (ob: var tNavmBackend,val: uint64,bigEndian: bool) =
  assert (ob.fInit != false)
  if bigEndian != false:
     let b1: uint8 = cast[uint8]((val shr 56) and 0xFF)
     let b2: uint8 = cast[uint8]((val shr 48) and 0xFF)
     let b3: uint8 = cast[uint8]((val shr 40) and 0xFF)
     let b4: uint8 = cast[uint8]((val shr 32) and 0xFF)
     let b5: uint8 = cast[uint8]((val shr 24) and 0xFF)
     let b6: uint8 = cast[uint8]((val shr 16) and 0xFF)
     let b7: uint8 = cast[uint8]((val shr 8)  and 0xFF)
     let b8: uint8 = cast[uint8](val  and 0xFF)
     shmPoke[uint8] (ob.tCodeMem,ob.oCodeMem,b8)
     shmPoke[uint8] (ob.tCodeMem,ob.oCodeMem+1,b7)
     shmPoke[uint8] (ob.tCodeMem,ob.oCodeMem+2,b6)
     shmPoke[uint8] (ob.tCodeMem,ob.oCodeMem+3,b5)
     shmPoke[uint8] (ob.tCodeMem,ob.oCodeMem+4,b4)
     shmPoke[uint8] (ob.tCodeMem,ob.oCodeMem+5,b3)
     shmPoke[uint8] (ob.tCodeMem,ob.oCodeMem+6,b2)
     shmPoke[uint8] (ob.tCodeMem,ob.oCodeMem+7,b1)
     ob.oCodeMem = ob.oCodeMem + 8
  else:
     let b1: uint8 = cast[uint8](val  and 0xFF)
     let b2: uint8 = cast[uint8]((val shr 8)  and 0xFF)
     let b3: uint8 = cast[uint8]((val shr 16) and 0xFF)
     let b4: uint8 = cast[uint8]((val shr 24) and 0xFF)
     let b5: uint8 = cast[uint8]((val shr 32) and 0xFF)
     let b6: uint8 = cast[uint8]((val shr 40) and 0xFF)
     let b7: uint8 = cast[uint8]((val shr 48) and 0xFF)
     let b8: uint8 = cast[uint8]((val shr 56) and 0xFF)
     shmPoke[uint8] (ob.tCodeMem,ob.oCodeMem,b8)
     shmPoke[uint8] (ob.tCodeMem,ob.oCodeMem+1,b7)
     shmPoke[uint8] (ob.tCodeMem,ob.oCodeMem+2,b6)
     shmPoke[uint8] (ob.tCodeMem,ob.oCodeMem+3,b5)
     shmPoke[uint8] (ob.tCodeMem,ob.oCodeMem+4,b4)
     shmPoke[uint8] (ob.tCodeMem,ob.oCodeMem+5,b3)
     shmPoke[uint8] (ob.tCodeMem,ob.oCodeMem+6,b2)
     shmPoke[uint8] (ob.tCodeMem,ob.oCodeMem+7,b1)
     ob.oCodeMem = ob.oCodeMem + 8


when isMainModule:
  var test: tNavmBackend

  test.init (0xFFFF)

  echo ("Endian test (value F8F9FAFBFCFDFEFFh):")

  echo ("  ---- big endian ----")
  test.compile8 (0xF8F9FAFBFCFDFEFF'u64, true)
  echo ("  byte 1: ",shmPeek[uint8] (test.tCodeMem,0))
  echo ("  byte 2: ",shmPeek[uint8] (test.tCodeMem,1))
  echo ("  byte 3: ",shmPeek[uint8] (test.tCodeMem,2))
  echo ("  byte 4: ",shmPeek[uint8] (test.tCodeMem,3))
  echo ("  byte 5: ",shmPeek[uint8] (test.tCodeMem,4))
  echo ("  byte 6: ",shmPeek[uint8] (test.tCodeMem,5))
  echo ("  byte 7: ",shmPeek[uint8] (test.tCodeMem,6))
  echo ("  byte 8: ",shmPeek[uint8] (test.tCodeMem,7))

  echo ("  ---- little endian ----")
  test.compile8 (0xF8F9FAFBFCFDFEFF'u64, false)
  echo ("  byte 1: ",shmPeek[uint8] (test.tCodeMem,8))
  echo ("  byte 2: ",shmPeek[uint8] (test.tCodeMem,9))
  echo ("  byte 3: ",shmPeek[uint8] (test.tCodeMem,10))
  echo ("  byte 4: ",shmPeek[uint8] (test.tCodeMem,11))
  echo ("  byte 1: ",shmPeek[uint8] (test.tCodeMem,12))
  echo ("  byte 2: ",shmPeek[uint8] (test.tCodeMem,13))
  echo ("  byte 3: ",shmPeek[uint8] (test.tCodeMem,14))
  echo ("  byte 4: ",shmPeek[uint8] (test.tCodeMem,15))

  test.release ()
