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
# MODULE:      Platform independent interface of the compiler and its
#              optimization routines
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

import navmBackend, navmInstructions, unsigned, strUtils

# ---------------------------------------------------------------tNavmFrontend-
# DESCRIPTION: The frontend object encapsulate all state variables for the
#              code optimization routines. Please note that these compiler is
#              part of an interpretive runtime envirionment for the AVM Level
#              2 ISA and integrated into it though the CI and I instructions
#              which handle instruction (re)compilation. One reason therefor
#              is, that this way the dynamic state of the interpretive
#              environment is reusable for code-generation and optimization.
#              Other reasons are FFI seperation which minimize code complexity
#              and better ressource usage
#
# BUGS:        [2013.8 .23] none found
#
# TO-DO:       Implement better exception handling
#              implement alternative to addIns which uses a variadic record
#              as higher-level abstraction of the encoding scheme
# -----------------------------------------------------------------------------
# VARIABLES:
#
#      fDisAsm - This flag is setted true if code-generation should output
#                a disassembly beside compilation to stdout
#           vA - A variable for storing the actual top-of-stack value at
#                compilation. Because AVM code grant constant behaviour for
#                most values, the compiler can benefit from dynamic-folding
#                as optimization to large extend. For this, the actual top-
#                of-stack value is simply calculated at compile-time so
#                instead of compiling code-sequences to calculate constant
#                values these are replaced with there precalculated result
#       sVmMem - AVM code ist stored as sequence of word-sized integers
#                (tNavmSCell) to avoid manual memory managment
#       oVmMem - Offset into sVmCode as marker for the actually compiled
#                opcode-bundle
#
# INTERFACE:
#
#   eNavmIn* {.pure.} = enum
#     enumeration of all 32 basic AVM instructions for easier assemblation
#   eNavmOp* {.pure.} = enum
#     enumeration of all 1024 opcodes (2x instruction bundling). Please note
#     these opcodes are unit of execution, not single instructions! The
#     interpreter execute up to 12 opcodes, each consisting of two fused
#     instructions (software pipeline, max. 24 instructions per dispatch).
#     From these all valid instruction combinations (which are not special
#     macro instructions using two slots) can be compiled with exception of
#     PCK and B
#   proc setup
#     (ob: var tNavmFrontend, disAsm: bool, codeSize: int)
#       object constructor. A destructor is not needed because of GC and
#       tNavmBackend.release is sufficient
#   proc setTraceOfs
#     (ob: var tNavmFrontend, ofs: int)
#       set the start offset into sVmMem for compilation. This way
#       code modification and recompilation is possible
#   proc getTraceOfs
#     (ob: var tNavmFrontend): int
#       get the actual trace offset (is used for compiling the R instructions)
#   proc setDisAsmFlag
#     (ob: var tNavmFrontend, flag: bool)
#       set disassembly flag (fDisAsm)
#   proc getDisAsmFlag
#     (ob: var tNavmFrontend): bool
#       get furrent disassembly flag setting
#   proc addIns
#     (ob: var tNavmFrontend, bundle: var tNavmUCell, ins: eNavmIn)
#       assemble single instruction to opcode-bundle 'bundle' by right shifting
#       its slots. The functionality of these procedure is equivalent to a
#       bit-slicing register and as thus, addition of instructions up the max.
#       slot number resulting in following instruction loss of priour assembled
#       instructions (it's not a bug, it's a feature) !
#  proc pokeBundle
#    (ob: var tNavmFrontend, bundle: tNavmUCell)
#       compile opcode-bundle at actual offset into machine-code array
#  proc peekBundle
#    (ob: tNavmFrontend, ofs: int): tNavmUCell
#       get opcode-bundle at ofs from machine-code array
#  proc pokeImm
#    (ob: var tNavmFrontend, imm: tNavmSCell)
#       compile immediate value at actual offset into machine-code array
#  proc peekImm
#    (ob: tNavmFrontend, ofs:int): tNavmSCell =
#       get immediate value at ofs out of machine-code array
#
#  the following routines compile a single AVM opcode-bundle, each for a
#  specific optimization. These are: Direct-register mapping (DRM), immediate-
#  transformation (IT) and dynamic folding (DF). These three optimizations are
#  sufficient for generating highly optimizated machine-code because the AVM
#  ISA is specially designed for efficient compilation (and interpretation),
#  featuring an encoding which can be seen as a uncommon CPS form. This form
#  already solve most SSA based optimizations implicitly and as result, the
#  compiler can be implemented with minimal complexity, moreover generating
#  efficient machine-code with minimal ressource usage. That's the trick
#  (beside the possibility to generate new or recompile existant instructions
#  at runtime for efficient interpretation)!
#
#  proc compBundleDRM
#    (ob: var tNavmFrontend)
#      compiles a single opcode-bundle with DRM optimization. The result is
#      efficient native-code for most out-of-order architectures except in-
#      order designs (such as Intel ATOM). These CPU class will need a simple
#      additional optimization: Static exchanging of instructions slots
#      (alias instruction-sheduling) which an application can apply without
#      problems at demand so there is no need for a special interface routine
#      (please note, that optimization is only easy applicable because the
#      AVM ISA specify a process-effect free, bundled encoding. You will get
#      in trouble with this for all JIT compiler I know of because there IL
#      definations either do not serialize state assignments [all register
#      based designs] or uses folded SSA forms which both are more or less
#      insufficient for instruction shuffling) 
# -----------------------------------------------------------------------------

type
  tNavmFrontend* = object of tNavmBackend
                     fDisAsm: bool
                     vA:      tNavmSCell
                     sVmMem:  seq[tNavmSCell]
                     oVmMem:  int

  tNavmIns = range [0..31]

  eNavmIn* {.pure.} = enum
       sys, li,  lx,  ld,  st,  lxi, sxi, lxd, sxd, add, adc, sub, sbc, slb,
       srb, anb, gor, xob, dup, drp, swp, ovr, pck, rot, b,   bs,  br,  r,
       eq,  gr,  le,  zr

  eNavmOp* {.pure.} = enum
       FETCH,liSYS,lixSYS,ldSYS,stSYS,lxiSYS,sxiSYS,lxdSYS,sxdSYS,addSYS,
       adcSYS,subSYS,sbcSYS,slbSYS,srbSYS,anbSYS,gorSYS,xobSYS,dupSYS,drpSYS,
       swpSYS,ovrSYS,pckSYS,rotSYS,B,BS,BR,R,eqSYS,eqGR,eqLE,egZR,sysLI,liLI,
       lixLI,ldLI,stLI,lxiLI,sxiLI,lxdLI,sxdLI,addLI,adcLI,subLI,sbcLI,slbLI,
       srbLI,anbLI,gorLI,xobLI,dupLI,drpLI,swpLI,ovrLI,pckLI,rotLI,I1,I2,I3,
       I4,eqLI,grLI,leLI,zrLI,sysLX,liLX,lixLX,ldLX,stLX,lxiLX,sxiLX,lxdLX,
       sxdLX,addLX,adcLX,subLX,sbcLX,slbLX,srbLX,anbLX,gorLX,xobLX,dupLX,drpLX,
       swpLX,ovrLX,pckLX,rotLX,I5,I6,I7,I8,eqLX,grLX,leLX,zrLX,sysLD,liLD,
       lixLD,ldLD,stLD,lxiLD,sxiLD,lxdLD,sxdLD,addLD,adcLD,subLD,sbcLD,slbLD,
       srbLD,anbLD,gorLD,xobLD,dupLD,drpLD,swpLD,ovrLD,pckLD,rotLD,I9,I10,I11,
       I12,eqLD,grLD,leLD,zrLD,sysST,liST,lixST,ldST,stST,lxiST,sxiST,lxdST,
       sxdST,addST,adcST,subST,sbcST,slbST,srbST,anbST,gorST,xobST,dupST,drpST,
       swpST,ovrST,pckST,rotST,I13,I14,I15,I16,eqST,grST,leST,zrST,sysLXI,
       liLXI,lixLXI,ldLXI,stLXI,lxiLXI,sxiLXI,lxdLXI,sxdLXI,addLXI,adcLXI,
       subLXI,sbcLXI,slbLXI,srbLXI,anbLXI,gorLXI,xobLXI,dupLXI,drpLXI,swpLXI,
       ovrLXI,pckLXI,rotLXI,I17,I18,I19,I20,eqLXI,grLXI,leLXI,zrLXI,sysSXI,
       liSXI,lixSXI,ldSXI,stSXI,lxiSXI,sxiSXI,lxdSXI,sxdSXI,addSXI,adcSXI,
       subSXI,sbcSXI,slbSXI,srbSXI,anbSXI,gorSXI,xobSXI,dupSXI,drpSXI,swpSXI,
       ovrSXI,pckSXI,rotSXI,I21,I22,I23,I24,eqSXI,grSXI,leSXI,zrSXI,sysLXD,
       liLXD,lixLXD,ldLXD,stLXD,lxiLXD,sxiLXD,lxdLXD,sxdLXD,addLXD,adcLXD,
       subLXD,sbcLXD,slbLXD,srbLXD,anbLXD,gorLXD,xobLXD,dupLXD,drpLXD,swpLXD,
       ovrLXD,pckLXD,rotLXD,I25,I26,I27,I28,eqLXD,grLXD,leLXD,zrLXD,sysSXD,
       liSXD,lixSXD,ldSXD,stSXD,lxiSXD,sxiSXD,lxdSXD,sxdSXD,addSXD,adcSXD,
       subSXD,sbcSXD,slbSXD,srbSXD,anbSXD,gorSXD,xobSXD,dupSXD,drpSXD,swpSXD,
       ovrSXD,pckSXD,rotSXD,I29,I30,I31,I32,eqSXD,grSXD,leSXD,zrSXD,sysADD,
       liADD,lixADD,ldADD,stADD,lxiADD,sxiADD,lxdADD,sxdADD,addADD,adcADD,
       subADD,sbcADD,slbADD,srbADD,anbADD,gorADD,xobADD,dupADD,drpADD,swpADD,
       ovrADD,pckADD,rotADD,I33,I34,I35,I36,eqADD,grADD,leADD,zrADD,sysADC,
       liADC,lixADC,ldADC,stADC,lxiADC,sxiADC,lxdADC,sxdADC,addADC,adcADC,
       subADC,sbcADC,slbADC,srbADC,anbADC,gorADC,xobADC,dupADC,drpADC,swpADC,
       ovrADC,pckADC,rotADC,I37,I38,I39,I40,eqADC,grADC,leADC,zrADC,sysSUB,
       liSUB,lixSUB,ldSUB,stSUB,lxiSUB,sxiSUB,lxdSUB,sxdSUB,addSUB,adcSUB,
       subSUB,sbcSUB,slbSUB,srbSUB,anbSUB,gorSUB,xobSUB,dupSUB,drpSUB,swpSUB,
       ovrSUB,pckSUB,rotSUB,I41,I42,I43,I44,eqSUB,grSUB,leSUB,zrSUB,sysSBC,
       liSBC,lixSBC,ldSBC,stSBC,lxiSBC,sxiSBC,lxdSBC,sxdSBC,addSBC,adcSBC,
       subSBC,sbcSBC,slbSBC,srbSBC,anbSBC,gorSBC,xobSBC,dupSBC,drpSBC,swpSBC,
       ovrSBC,pckSBC,rotSBC,I45,I46,I47,I48,eqSBC,grSBC,leSBC,zrSBC,sysSLB,
       liSLB,lixSLB,ldSLB,stSLB,lxiSLB,sxiSLB,lxdSLB,sxdSLB,addSLB,adcSLB,
       subSLB,sbcSLB,slbSLB,srbSLB,anbSLB,gorSLB,xobSLB,dupSLB,drpSLB,swpSLB,
       ovrSLB,pckSLB,rotSLB,I49,I50,I51,I52,eqSLB,grSLB,leSLB,zrSLB,sysSRB,
       liSRB,lixSRB,ldSRB,stSRB,lxiSRB,sxiSRB,lxdSRB,sxdSRB,addSRB,adcSRB,
       subSRB,sbcSRB,slbSRB,srbSRB,anbSRB,gorSRB,xobSRB,dupSRB,drpSRB,swpSRB,
       ovrSRB,pckSRB,rotSRB,I53,I54,I55,I56,eqSRB,grSRB,leSRB,zrSRB,sysANB,
       liANB,lixANB,ldANB,stANB,lxiANB,sxiANB,lxdANB,sxdANB,addANB,adcANB,
       subANB,sbcANB,slbANB,srbANB,anbANB,gorANB,xobANB,dupANB,drpANB,swpANB,
       ovrANB,pckANB,rotANB,I57,I58,I59,I60,eqANB,grANB,leANB,zrANB,sysGOR,
       liGOR,lixGOR,ldGOR,stGOR,lxiGOR,sxiGOR,lxdGOR,sxdGOR,addGOR,adcGOR,
       subGOR,sbcGOR,slbGOR,srbGOR,anbGOR,gorGOR,xobGOR,dupGOR,drpGOR,swpGOR,
       ovrGOR,pckGOR,rotGOR,I61,I62,I63,I64,eqGOR,grGOR,leGOR,zrGOR,sysXOB,
       liXOB,lixXOB,ldXOB,stXOB,lxiXOB,sxiXOB,lxdXOB,sxdXOB,addXOB,adcXOB,
       subXOB,sbcXOB,slbXOB,srbXOB,anbXOB,gorXOB,xobXOB,dupXOB,drpXOB,swpXOB,
       ovrXOB,pckXOB,rotXOB,I65,I66,I67,I68,eqXOB,grXOB,leXOB,zrXOB,sysDUP,
       liDUP,lixDUP,ldDUP,stDUP,lxiDUP,sxiDUP,lxdDUP,sxdDUP,addDUP,adcDUP,
       subDUP,sbcDUP,slbDUP,srbDUP,anbDUP,gorDUP,xobDUP,dupDUP,drpDUP,swpDUP,
       ovrDUP,pckDUP,rotDUP,I69,I70,I71,I72,eqDUP,grDUP,leDUP,zrDUP,sysDRP,
       SYS,LI,LIX,stDRP,LD,sxiDRP,ST,sxdDRP,LXI,SXI,LXD,SXD,ADD,ADC,SUB,SBC,
       SLB,dupDRP,drpDRP,SRB,ANB,GOR,XOB,I73,I74,I75,I76,eqDRP,grDRP,leDRP,
       zrDRP,sysSWP,liSWP,lixSWP,ldSWP,stSWP,lxiSWP,sxiSWP,lxdSWP,sxdSWP,
       addSWP,adcSWP,subSWP,sbcSWP,slbSWP,srbSWP,anbSWP,gorSWP,xobSWP,DUP,
       drpSWP,SRP,ovrSWP,pckSWP,rotSWP,I77,I78,I79,I80,eqSWP,grSWP,leSWP,
       zrSWP,sysOVR,liOVR,lixOVR,ldOVR,stOVR,lxiOVR,sxiOVR,lxdOVR,sxdOVR,
       addOVR,adcOVR,subOVR,sbcOVR,slbOVR,srbOVR,anbOVR,gorOVR,xobOVR,dupOVR,
       drpOVR,swpOVR,ovrOVR,pckOVR,rotOVR,I81,I82,I83,I84,eqOVR,grOVR,leOVR,
       zrOVR,sysPCK,liPCK,lixPCK,ldPCK,stPCK,lxiPCK,sxiPCK,lxdPCK,sxdPCK,
       addPCK,adcPCK,subPCK,sbcPCK,slbPCK,srbPCK,anbPCK,gorPCK,xobPCK,dupPCK,
       drpPCK,swpPCK,ovrPCK,pckPCK,rotPCK,I85,I86,I87,I88,eqPCK,grPCK,lePCK,
       zrPCK,sysROT,liROT,lixROT,ldROT,stROT,lxiROT,sxiROT,lxdROT,sxdROT,
       addROT,adcROT,subROT,sbcROT,slbROT,srbROT,anbROT,gorROT,xobROT,dupROT,
       drpROT,swpROT,ovrROT,pckROT,rotROT,I89,I90,I91,I92,eqROT,grROT,leROT,
       zrROT,sysB,liB,lixB,ldB,stB,lxiB,sxiB,lxdB,sxdB,addB,adcB,subB,sbcB,
       slbB,srbB,anbB,gorB,xobB,dupB,drpB,swpB,ovrB,pckB,rotB,I93,I94,I95,I96,
       eqB,grB,leB,zrB,sysBS,liBS,lixBS,ldBS,stBS,lxiBS,sxiBS,lxdBS,sxdBS,
       addBS,adcBS,subBS,sbcBS,slbBS,srbBS,anbBS,gorBS,xobBS,dupBS,drpBS,swpBS,
       ovrBS,pckBS,rotBS,I97,I98,I99,I100,eqBS,grBS,leBS,zrBS,sysBR,liBR,lixBR,
       ldBR,stBR,lxiBR,sxiBR,lxdBR,sxdBR,addBR,adcBR,subBR,sbcBR,slbBR,srbBR,
       anbBR,gorBR,xobBR,dupBR,drpBR,swpBR,ovrBR,pckBR,rotBR,I101,I102,I103,
       I104,eqBR,grBR,leBR,zrBR,sysR,liR,lixR,ldR,stR,lxiR,sxiR,lxdR,sxdR,adR,
       adcR,subR,sbcR,slbR,srbR,anbR,gorR,xobR,dupR,drpR,swpR,ovrR,pckR,rotR,
       I105,I106,I107,I108,eqR,grR,leR,zrR,sysEQ,liEQ,lixEQ,ldEQ,stEQ,lxiEQ,
       sxiEQ,lxdEQ,sxdEQ,addEQ,adcEQ,subEQ,sbcEQ,slbEQ,srbEQ,anbEQ,gorEQ,xobEQ,
       dupEQ,drpEQ,swpEQ,ovrEQ,pckEQ,rotEQ,I109,I110,I111,I112,SWP,grEQ,leEQ,
       zrEQ,sysGR,liGR,lixGR,ldGR,stGR,lxiGR,sxiGR,lxdGR,sxdGR,addGR,adcGR,
       subGR,sbcGR,slbGR,srbGR,anbGR,gorGR,xobGR,dupGR,drpGR,swpGR,ovrGR,pckGR,
       rotGR,I113,I114,I115,I116,eqGR,OVR,leGR,zrGR,sysLE,liLE,lixLE,ldLE,stLE,
       lxiLE,sxiLE,lxdLE,sxdLE,addLE,adcLE,subLE,sbcLE,slbLE,srbLE,anbLE,gorLE,
       xobLE,dupLE,drpLE,swpLE,ovrLE,pckLE,rotLE,I117,I118,I119,I120,eqLE,grLE,
       PCK,zrLE,sysZR,liZR,lixZR,ldZR,stZR,lxiZR,sxiZR,lxdZR,sxdZR,addZR,adcZR,
       subZR,sbcZR,slbZR,srbZR,anbZR,gorZR,xobZR,dupZR,drpZR,swpZR,ovrZR,pckZR,
       rotZR,CI,EI,DI,BRK,eqZR,grZR,leZR,ROT

# object handling routines

proc errorAssert =
  system.writeLN (stderr, "[navmFrontend] ob.fInit[ofs] != true")
  quit ()

proc setup* (ob: var tNavmFrontend,disAsm: bool,codeSize: int) =
  ob.fDisAsm = disAsm
  ob.vA      = 0
  ob.oVmMem  = 0

  ob.sVmMem = @[]
  ob.init (codeSize)

proc setTraceOfs* (ob: var tNavmFrontend,ofs: int) =
  assert (ob.fInit != false)

  try:
    ob.oVmMem = ofs
  except EAssertionFailed:
    errorAssert ()

proc getTraceOfs* (ob: var tNavmFrontend): int =
  assert (ob.fInit != false)

  try:   result = ob.oVmMem
  except EAssertionFailed: errorAssert ()

proc setDisAsmFlag* (ob: var tNavmFrontend,flag: bool) =
  assert (ob.fInit != false)

  try:   ob.fDisAsm = flag
  except EAssertionFailed: errorAssert ()

proc getDisAsmFlag* (ob: var tNavmFrontend): bool =
  assert (ob.fInit != false)

  try:   result = ob.fDisAsm
  except EAssertionFailed: errorAssert ()

proc addIns* (ob: var tNavmFrontend,bundle: var tNavmUCell,ins: eNavmIn) =
  assert (ob.fInit != false)
  var bgSlot: tNavmIns = ord (ins)

  try:
      bundle = bundle shl 5
      bundle = bundle + cast [tNavmUCell](bgSlot)
  except EAssertionFailed:
    errorAssert ()
  
proc pokeBundle* (ob: var tNavmFrontend,bundle: tNavmUCell) =
  assert (ob.fInit != false)

  try:
    var b: tNavmSCell = cast[tNavmSCell](bundle)
    ob.sVmMem.add (b)
  except EAssertionFailed:
    errorAssert ()

proc pokeImm* (ob: var tNavmFrontend,imm: tNavmSCell) =
  assert (ob.fInit != false)

  try:   ob.sVmMem.add (imm)
  except EAssertionFailed: errorAssert ()

proc peekBundle* (ob: tNavmFrontend,ofs: int): tNavmUCell =
  assert (ob.fInit != false)

  proc error =
    system.writeLN (stderr,"[peekBundle] ob.sVmMem[ofs] = NIL")
    quit ()

  try:    result = cast[tNavmUCell](ob.sVmMem[ofs])
  except  EAssertionFailed: errorAssert ()
  except: error ()

proc peekImm* (ob: tNavmFrontend,ofs:int): tNavmSCell =
  assert (ob.fInit != false)

  proc error =
    system.writeLN (stderr,"[peekImm] ob.sVmMem[ofs] = NIL")
    quit ()
  
  try:    result = ob.sVmMem[ofs]
  except  EAssertionFailed: errorAssert ()
  except: error ()

proc error (msg: string) =
  system.writeLN (stderr, msg)
  quit ()

proc compBundleDRM* (ob: var tNavmFrontend) =
  var
    vBndl: tNavmUCell = cast[tNavmUCell](ob.sVmMem[ob.oVmMem])
    vInsA: int8 = 0
    vInsB: int8 = 0
    i:     int  = 1

  assert (ob.fInit != false)

  try:
    ob.oVmMem = ob.oVmMem + 1

    when sizeof (tNavmUCell) > 4:
      const num = 12
    elif sizeof (tNavmUCell) > 2:
      const num = 6
    elif sizeof (tNavmUCell) < 3:
      const num = 3
    else:
      error ("[compBundleDRM] tNavmUCell.size != 2,4,8")

    block compile:
      while i <= num:
        vInsA = cast[int8](vBndl and 0x1F)
        vInsB = cast[int8]((vBndl shr 5) and 0x1F)
        vBndl = vBndl shr 5

        case vInsA
          of ord (eNavmIn.li):
               ob.liImm (ob.sVmMem[ob.oVmMem],ob.fDisAsm)
               ob.oVmMem = ob.oVmMem + 1
          of ord (eNavmIn.br):
               ob.br (ob.fDisAsm)
               break compile
          of ord (eNavmIn.eq):
               ob.eq (ob.fDisAsm)
               if cfNavmCompUnified != false and vInsB > ord (eNavmIn.r):
                  vBndl = vBndl shr 5
                  i = i + 1
          of ord (eNavmIn.gr):
               ob.gr (ob.fDisAsm)
               if cfNavmCompUnified != false and vInsB > ord (eNavmIn.r):
                  vBndl = vBndl shr 5
                  i = i + 1
          of ord (eNavmIn.le):
               ob.le (ob.fDisAsm)
               if cfNavmCompUnified != false and vInsB > ord (eNavmIn.r):
                  vBndl = vBndl shr 5
                  i = i + 1
          of ord (eNavmIn.zr):
               ob.zr (ob.fDisAsm)
               if (cfNavmCompUnified != false) and vInsB > ord (eNavmIn.r):
                  vBndl = vBndl shr 5
                  i = i + 1
          of ord (eNavmIn.sys): error ("[compBundleDRM] sys")
          of ord (eNavmIn.b):   error ("[compBundleDRM] b")
          of ord (eNavmIn.bs):  error ("[compBundleDRM] bs")
          of ord (eNavmIn.pck): error ("[compBundleDRM] pck")
          of ord (eNavmIn.lx):  ob.lx  (ob.fDisAsm)
          of ord (eNavmIn.ld):  ob.ld  (ob.fDisAsm)
          of ord (eNavmIn.st):  ob.st  (ob.fDisAsm)
          of ord (eNavmIn.lxi): ob.lxi (ob.fDisAsm)
          of ord (eNavmIn.sxi): ob.sxi (ob.fDisAsm)
          of ord (eNavmIn.lxd): ob.lxd (ob.fDisAsm)
          of ord (eNavmIn.sxd): ob.sxd (ob.fDisAsm)
          of ord (eNavmIn.add): ob.add (ob.fDisAsm)
          of ord (eNavmIn.adc): ob.adc (ob.fDisAsm)
          of ord (eNavmIn.sub): ob.sub (ob.fDisAsm)
          of ord (eNavmIn.sbc): ob.sbc (ob.fDisAsm)
          of ord (eNavmIn.slb): ob.slb (ob.fDisAsm)
          of ord (eNavmIn.srb): ob.srb (ob.fDisAsm)
          of ord (eNavmIn.anb): ob.anb (ob.fDisAsm)
          of ord (eNavmIn.gor): ob.gor (ob.fDisAsm)
          of ord (eNavmIn.xob): ob.xob (ob.fDisAsm)
          of ord (eNavmIn.dup): ob.dup (ob.fDisAsm)
          of ord (eNavmIn.swp): ob.swp (ob.fDisAsm)
          of ord (eNavmIn.ovr): ob.ovr (ob.fDisAsm)
          of ord (eNavmIn.rot): ob.rot (ob.fDisAsm)
          of ord (eNavmIn.r):   ob.r   (ob.fDisAsm)
          of ord (eNavmIn.drp): i = i
          else: error ("[compBundleDRM] unexpected error")
  except EAssertionFailed:
    errorAssert ()


when isMainModule:
  var
    test:   tNavmFrontend
    Bundle: tNavmUCell
    trace:  int

  test.setup (true, 0xFFFF)

  trace = test.getTraceOfs ()

  test.addIns (bundle,eNavmIn.br)
  test.addIns (bundle,eNavmIn.add)
  test.addIns (bundle,eNavmIn.li)
  test.addIns (bundle,eNavmIn.sub)
  test.addIns (bundle,eNavmIn.li)
  test.addIns (bundle,eNavmIn.li)
  echo ("bundle: ",toHex (cast[biggestInt](bundle), 16))

  test.pokeBundle (bundle)
  test.pokeImm    (100'i64)
  test.pokeImm    (200'i64)
  test.pokeImm    (300'i64)
  echo ("sVmMem[0]: ",toHex (cast[biggestInt](test.sVmMem[0]), 16))
  echo ("sVmMem[1]: ",test.sVmMem[1])
  echo ("sVmMem[2]: ",test.sVmMem[2])
  echo ("sVmMem[3]: ",test.sVmMem[2])

  echo ("first instruction: ",bundle and 0x1F)

  test.CompBundleDRM ()

  test.release ()
