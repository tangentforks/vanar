# -----------------------------------------------------------------------------
# PROJECT:     navm
# DESCRIPTION: NAVM is an interpreter for the AVM Level 2 ISA, a MISC inspired
#              instruction-set architecture specially designed for efficient
#              interpretation with minimal ressource usage. AVM is extensible,
#              allowing the (re)compilation of up to 120 extended instructions
#              at runtime in a platform-independent way. It also featuring a
#              very fast interpretation engine in combination with a higly
#              effective AOT compiler, generating optimizated machine-code
#              for common CPU architectures.  
# AUTHOR:      Matthias Schirm
# COPYRIGHT:   (C) 2103 Matthias Schirm
# LICENCE:     BSD, see licence.txt
# MODULE:      This module encapsulate the handling of shared-memory buffers
# -----------------------------------------------------------------------------

import posix

type tShmBuffer*[T] = object of TObject
                        pBuffer: ptr T
                        cBuffer: int
                        fInit:   bool

# for accessing shared-memory buffers, the cast operator converting the
# pBuffer pointer to type: array[0..x, uint8] and this is the only way for
# Nimrod accessing external memory. The problem is then, x must be a constant
# value! Instead, in this case, x is of size cBuffer:int and so not constant.
# For this purposes Nimrod allows the specification of a maximal upper
# range, still supporting basic range checking. This upper-bound limit
# is at default 0xFFFFF elements and can be changed at demand

const shmBufferMaxRange = 0xFFFFF 

# following are constructors for the object record tShmBuffer. There exist
# two variants, one for allocating a SHM Buffer with executive, read and
# write access-rights and another allowing only read and write access.
# For hosted usages, allocated memory for compiled machine-code must be
# registered with read, write and execution rights because otherwise a isNIX
# conform operating system will interrupt code-execution because of adress
# violation. For POSIX compatible operating-systems the mmap function can be
# used for this. However, its interface was originally designed for memory-
# mapping of files so these, otherwise standard conform, procedere depends on
# a non standard flag setting. Because the POSIX library of Nimrod do not
# import these OS dependent flag, beside different CPU backends the MAP_ANON
# constant must be changed for porting to other operating-systems as Linux!

const MAP_ANON = 0x20

proc shmInitExec*[T](ob: var tShmBuffer[T], memSize: int) =
  proc valloc (memSize: int): pointer =
    var size: int = memSize
    if size > shmBufferMaxRange: size = shmBufferMaxRange
    result = mmap (nil, size,
                   PROT_EXEC or PROT_READ  or PROT_WRITE,
                   MAP_ANON  or MAP_SHARED,
                   -1, 0)
    if result == nil:
       system.writeLN (stderr, "[initExec.valloc] mmap = MAP_FAILED")
       quit ()

  ob.pBuffer = cast[ptr T](valloc (memSize))
  ob.cBuffer = memSize
  ob.fInit   = true

proc shmInit*[T] (ob: var tShmBuffer[T], memSize: int) =
  proc valloc (size: int): pointer =
    result = mmap (nil, size,
                   PROT_READ or PROT_WRITE,
                   MAP_ANON  or MAP_SHARED, 
                   -1, 0)
    if result == nil:
       system.writeLN (stderr, "[init.valloc] mmap = MAP_FAILED")
       quit ()

  ob.pBuffer = cast[ptr T](valloc (memSize))
  ob.cBuffer = memSize
  ob.fInit   = true

# unsed buffers need to be explicit deallocated because of the external
# memory reservation. Please note that this, dependent of the OS used,
# external-reserved memory can still left allocated at program termination,
# so calling these destructor is mendatory, specially in memory restricted
# environments! (do not forget to release initiated SHM buffers)

proc shmRelease*[T] (ob: var tShmBuffer[T]) =
  assert (ob.fInit != false)
  var erg: cint = munmap (cast[pointer](ob.pBuffer), ob.cBuffer)
  if erg < 0:
     system.writeLN (stderr, "[tShmBuffer.release] munmap < 0")
     quit ()

# these are the routines for reading and writing SHM buffers. Please note that
# 'ofs' is a type-dependent offset for all variants! By initiating a buffer of
# uint8 for example, the 'ofs' parameter hold byte offsets where by initiating
# a buffer of uint16 'ofs' hold halfword offsets

proc shmBufferRangeCheck[T] (ob: var tShmBuffer[T], ofs: int) =
  assert (ob.fInit != false)
  if ofs > ob.cBuffer:
     system.writeLN (stderr, "[shmBufferRangeCheck] ofs > tShmBuffer.cBuffer")
     quit ()

proc shmPeek*[T] (ob: var tShmBuffer[T], ofs: int): T =
  shmBufferRangeCheck[T] (ob, ofs)
  result = cast[ptr array[0..shmBufferMaxRange, T]](ob.pBuffer)[ofs]
    
proc shmPoke*[T] (ob: var tShmBuffer[T], ofs: int, val: T) =
  shmBufferRangeCheck[T] (ob, ofs)
  cast[ptr array[0..shmBufferMaxRange, T]](ob.pBuffer)[ofs] = val

proc shmSize*[T] (ob: var tShmBuffer[T]): int =
  assert (ob.fInit != false)
  result = ob.cBuffer

# -----------------------------------------------------------------------------

when isMainModule:
  var test: tShmBuffer[uint8]
  var erg:  uint8

  var test1: tShmBuffer[uInt16]
  var erg1:  uint16

  shmInitExec[uint8] (test, 0xFFFF)
  shmPoke[uint8] (test, 0, 24)
  erg = shmPeek[uint8] (test, 0)
  system.writeLN (stdout, "8 bit: ", erg)

  shmInit[uint16] (test1, 0xFF)
  shmPoke[uint16] (test1, 2, 1000)
  erg1 = shmPeek[uint16] (test1, 2)
  system.writeLN (stdout, "16 bit: ", erg1)

  shmRelease[uint8]  (test)
  shmRelease[uint16] (test1)

