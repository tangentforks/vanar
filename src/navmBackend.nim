# -----------------------------------------------------------------------------
# PROJECT:     navm
# DESCRIPTION: NAVM is an interpreter for the AVM Level 2 ISA. It is a MISC
#              oriented instruction-set architecture specially designed for
#              efficient interpretation with minimal ressource usage. AVM
#              is an extensible design allowing the compilation of up to
#              120 extended instruction at runtime in a platform-independent
#              way. It also featuring a very fast interpretation engine in
#              combination with a higly effective AOT compiler, generating
#              optimizated machine-code for common CPU architectures.  
# AUTHOR:      Matthias Schirm
# COPYRIGHT:   (C) 2103 Matthias Schirm
# LICENCE:     BSD, see licence.txt
# -----------------------------------------------------------------------------

import posix, shmBuffer, unsigned

# This is the backend object-defination:
# Variables:   vD - virtual, dynamic data-stack state at compilation. The 
#                   compiler frontend uses its value for mapping the actual
#                   stack reference to a set of registers. NAVM define a
#                   small data-stack depth of 8 elements, so vA can hold
#                   values from 1-8. Value 0 signaled stack underflow,
#                   vD > 8 stack overflow.
#              vR - Like vD, these state variable is used for mapping the
#                   64 entry return-stack. Because all return adresses are
#                   known at compile-time the frontend compiler is able to
#                   convert subroutine calls into direct branches if useful
#        tCodeMem - static SHM buffer for compiled machine code (static
#                   allocated memory is important for embedded and bare-metal
#                   usages where often no operating system with dynamic memory
#                   management exist)
#        oCodeMem - offset into pCodeMem, referencing the first free element
#                   in buffer
#        fInit    - true if object-instance was initiated by init, used
#                   for catching access to unitialized objects

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

# Destructor for releasing the SHM buffer. Beware some operating systems
# are otherwise unable to free this priour allocated memory area (or at least
# mark it as free) !

proc release* (ob: var tNavmBackend) =
  assert (ob.fInit != false)
  shmRelease[uint8] (ob.tCodeMem)
  ob.fInit = false

# these are helpful routines for compiling short machine-code sequences

proc compile* (ob: var tNavmBackend, val: uint8) =
  assert (ob.fInit != false)
  shmPoke[uint8] (ob.tCodeMem, ob.oCodeMem, val)
  ob.oCodeMem = ob.oCodeMem + 1

proc compile2* (ob: var tNavmBackend, val: uint16, bigEndian: bool) =
  assert (ob.fInit != false)
  if bigEndian != false:
     let b1: uint8 = cast[uint8]((val shr 8) and 0xFF)
     let b2: uint8 = cast[uint8](val  and 0xFF)
     shmPoke[uint8] (ob.tCodeMem, ob.oCodeMem,   b2)
     shmPoke[uint8] (ob.tCodeMem, ob.oCodeMem+1, b1)
     ob.oCodeMem = ob.oCodeMem + 2
  else:
     let b1: uint8 = cast[uint8](val  and 0xFF)
     let b2: uint8 = cast[uint8]((val shr 8) and 0xFF)
     shmPoke[uint8] (ob.tCodeMem, ob.oCodeMem,   b2)
     shmPoke[uint8] (ob.tCodeMem, ob.oCodeMem+1, b1)
     ob.oCodeMem = ob.oCodeMem + 2
     
proc compile4* (ob: var tNavmBackend, val: uint32, bigEndian: bool) =
  assert (ob.fInit != false)
  if bigEndian != false:
     let b1: uint8 = cast[uint8]((val shr 24) and 0xFF)
     let b2: uint8 = cast[uint8]((val shr 16) and 0xFF)
     let b3: uint8 = cast[uint8]((val shr 8)  and 0xFF)
     let b4: uint8 = cast[uint8](val  and 0xFF)
     shmPoke[uint8] (ob.tCodeMem, ob.oCodeMem,   b4)
     shmPoke[uint8] (ob.tCodeMem, ob.oCodeMem+1, b3)
     shmPoke[uint8] (ob.tCodeMem, ob.oCodeMem+2, b2)
     shmPoke[uint8] (ob.tCodeMem, ob.oCodeMem+3, b1)
     ob.oCodeMem = ob.oCodeMem + 4
  else:
     let b1: uint8 = cast[uint8](val  and 0xFF)
     let b2: uint8 = cast[uint8]((val shr 8)  and 0xFF)
     let b3: uint8 = cast[uint8]((val shr 16) and 0xFF)
     let b4: uint8 = cast[uint8]((val shr 24) and 0xFF)
     shmPoke[uint8] (ob.tCodeMem, ob.oCodeMem,   b4)
     shmPoke[uint8] (ob.tCodeMem, ob.oCodeMem+1, b3)
     shmPoke[uint8] (ob.tCodeMem, ob.oCodeMem+2, b2)
     shmPoke[uint8] (ob.tCodeMem, ob.oCodeMem+3, b1)
     ob.oCodeMem = ob.oCodeMem + 4

proc compile8* (ob: var tNavmBackend, val: uint64, bigEndian: bool) =
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
     shmPoke[uint8] (ob.tCodeMem, ob.oCodeMem,   b8)
     shmPoke[uint8] (ob.tCodeMem, ob.oCodeMem+1, b7)
     shmPoke[uint8] (ob.tCodeMem, ob.oCodeMem+2, b6)
     shmPoke[uint8] (ob.tCodeMem, ob.oCodeMem+3, b5)
     shmPoke[uint8] (ob.tCodeMem, ob.oCodeMem+4, b4)
     shmPoke[uint8] (ob.tCodeMem, ob.oCodeMem+5, b3)
     shmPoke[uint8] (ob.tCodeMem, ob.oCodeMem+6, b2)
     shmPoke[uint8] (ob.tCodeMem, ob.oCodeMem+7, b1)
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
     shmPoke[uint8] (ob.tCodeMem, ob.oCodeMem,   b8)
     shmPoke[uint8] (ob.tCodeMem, ob.oCodeMem+1, b7)
     shmPoke[uint8] (ob.tCodeMem, ob.oCodeMem+2, b6)
     shmPoke[uint8] (ob.tCodeMem, ob.oCodeMem+3, b5)
     shmPoke[uint8] (ob.tCodeMem, ob.oCodeMem+4, b4)
     shmPoke[uint8] (ob.tCodeMem, ob.oCodeMem+5, b3)
     shmPoke[uint8] (ob.tCodeMem, ob.oCodeMem+6, b2)
     shmPoke[uint8] (ob.tCodeMem, ob.oCodeMem+7, b1)
     ob.oCodeMem = ob.oCodeMem + 8


when isMainModule:
  var test: tNavmBackend

  test.init (0xFFFF)

  echo ("Endian test (value F8F9FAFBFCFDFEFFh):")

  echo ("  ---- big endian ----")
  test.compile8 (0xF8F9FAFBFCFDFEFF'u64, true)
  echo ("  byte 1: ", shmPeek[uint8] (test.tCodeMem, 0))
  echo ("  byte 2: ", shmPeek[uint8] (test.tCodeMem, 1))
  echo ("  byte 3: ", shmPeek[uint8] (test.tCodeMem, 2))
  echo ("  byte 4: ", shmPeek[uint8] (test.tCodeMem, 3))
  echo ("  byte 5: ", shmPeek[uint8] (test.tCodeMem, 4))
  echo ("  byte 6: ", shmPeek[uint8] (test.tCodeMem, 5))
  echo ("  byte 7: ", shmPeek[uint8] (test.tCodeMem, 6))
  echo ("  byte 8: ", shmPeek[uint8] (test.tCodeMem, 7))

  echo ("  ---- little endian ----")
  test.compile8 (0xF8F9FAFBFCFDFEFF'u64, false)
  echo ("  byte 1: ", shmPeek[uint8] (test.tCodeMem, 8))
  echo ("  byte 2: ", shmPeek[uint8] (test.tCodeMem, 9))
  echo ("  byte 3: ", shmPeek[uint8] (test.tCodeMem, 10))
  echo ("  byte 4: ", shmPeek[uint8] (test.tCodeMem, 11))
  echo ("  byte 1: ", shmPeek[uint8] (test.tCodeMem, 12))
  echo ("  byte 2: ", shmPeek[uint8] (test.tCodeMem, 13))
  echo ("  byte 3: ", shmPeek[uint8] (test.tCodeMem, 14))
  echo ("  byte 4: ", shmPeek[uint8] (test.tCodeMem, 15))

  test.release ()
