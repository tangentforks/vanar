The Vanar compiler
==================
Matthias Schirm <matasan@gmx.de>
v0.1, 2013-8

All that can be defined,
is not the all eternal;
concepts that can be conceived,
are not eternal concepts!
Inconceivable is all origin,
it is conceivable as the
myriad things' mother.

Therefore,
always without desire,
so see its mystery;
ever with desire,
so see its surface.

These two:
together emerging,
yet differing in names –
together call them
mysterious.
Mystery's
more mysterious –
all Mysteries'
Gate...

:numbered!:
[abstract]
Abstract
--------

Vanar (or Vanir, Wane) is an AOT compiler that ensured effective compilation
with minimal resource consumption by using a MISC-oriented ISA as platform-
independent intermediate code. The machine code generated generally proves to
be highly optimized without the need for onerous processor as memory dependent
SSA analysis, limiting efficient usage in dynamic or ressource restricted
environments.

:numbered:

Inroduction
-----------

The Vanar compiler consisting of a platform independend backend and frontend
interface, encapsulating platform-dependent CPU descriptions for transforming
the NAVM instruction-set architecture into equivalent machine-code. As NAVM
is a MISC oriented ISA specially designed for efficient compilation *and*
interpretation, the common overhead of the whole compilation system is
minimal compared to traditional AOT or JIT compilation yet generating highly
optimized machine code for common CPU architectures.

These documentary focuses on three main points:

- The AVM instruction-set architecture and its instruction encoding
- The structure of the compiler interfaces
- Internal as optional code-optimization strategies.

Because Vanar compiles sequences of AVM instruction bundles the second
section introducing encoding and instruction-set of the AVM Level 1 ISA.
Following the source-code structure of the compiler is explained inclusive
the interface for generating machine-code. At last the internal optimization
strategy of the compiler is analysed whereby optional code-optimizations,
common for dynamic code generation is discussed.

The AVM instruction-set architecture
------------------------------------

AVM Level 1 is a MISC oriented ISA specially designed for effective compilation
*and* interpretation independent of special instruction-set features or
dispatching strategies. As the encoding is word-size oriented, the ISA defines
three encodings for 16,32 and 64 bit architectures with compatibility only at
level of its immediate-code (ILR) and *not* its binary-representation!

There exist 16 basic, in addition to 24 complex instructions with extended
encoding. All possible combinations of two base instructions are combined
into operation-codes (opcodes) of 8-bit size whereby either two, four or eight
opcodes, dependent of the word-size are bundled into an execution word (EW),
encoding immediate parameters as signed integers of word-size following the
EW in order. EW's can be interpreted, executed in hardware or AOT compilated
into machine-code for a specific processor architecture. Interpretation is
thereby effective independent of specific dispatch strategies because base unit
of execution are opcode bundles, not single instructions. The AVM ISA and its
encoding is also very effective implementable for FPGA's or ASIC's, both in
terms of needed space as archivable performance. FPGA implementations for
example can expect to archive 0.5 IPC with branch and interrupt latencies of
only 1 clock.cycle.

Register set
~~~~~~~~~~~~

AVM is a dual stack-based architecture following MISC (Minimal Instruction-
Set) principles. There exist one data and a seperate return-address stack.
Register access for all instructions, except LI (load-immediate) and four
stack-shuffling operators implicitly address the actual first stack element
(TOS) of either the data or return-address stack. The whole register-set is
therefore not direct accessible:

.Register set
[width="60%",options="header"]
|===============================================
| Register | Usage
| A        | Accumulator (TOS)
| D        | Data-stack pointer
| R        | Return-stack pointer
| F        | Condition-code register
| W        | internal scratch register
|===============================================

