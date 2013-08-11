The Vanar compiler
==================
Matthias Schirm <matasan@gmx.de>
v0.1, 2013-8

[verse, Laozi (translated by Derek Lin), Tao te Ching]
The Tao that can be spoken is not the eternal Tao
The name that can be named is not the eternal name
The nameless is the origin of Heaven and Earth
The named is the mother of myriad things
Thus, constantly without desire, one observes its essence
Constantly with desire, one observes its manifestations
These two emerge together but differ in name
The unity is said to be the mystery
Mystery of mysteries, the door to all wonders

:numbered!:
[abstract]
Abstract
--------

Vanar is an AOT compiler that ensures effective compilation with minimal resource consumption by using a MISC-oriented ISA as platform-independent intermediate code.

Vanar produces highly optimized machine code without the need for processor-intensive SSA analysis, allowing it to be used in dynamic or ressource-restricted environments.

:numbered:

Inroduction
-----------

// this part was confusing, and didn't explain what NAVM was:
// I'm not sure I got the translation right.

The Vanar compiler consists of several parts:

  - a compact, platform-independent virtual instruction-set architecture (nAVM)
  - a frontend compiler interface for generating navm instructions
  - a set of platform-specific CPU descriptions for transforming
    the nAVM ISA into equivalent machine-code

As nAVM is a MISC oriented ISA specially designed for efficient compilation 
*and* interpretation, the common overhead of the whole compilation system
is minimal compared to traditional AOT or JIT compilation, yet it generates
highly optimized machine code for common CPU architectures.

The rest of this document is organized as follows:

  - Section 2 describes the nAVM instruction set.
  - Section 3 describes the structure of the compiler and the interface for generating machine-code.
  - Section 4 describes a set of optional code-optimizations provided for dynamic code generation.


The nAVM instruction-set architecture
------------------------------------

nAVM Level 1 is a MISC oriented ISA designed for efficient compilation
*and* interpretation, independent of special instruction-set features or
dispatching strategies. As the encoding is word-size oriented, the ISA defines
separate encodings for 16, 32 and 64 bit architectures, identical at the
immediate-code level but *not* in their binary representations!

nAVM contains 16 basic instructions (requiring 4 bits each), and 24 complex instructions.
// ?? what does "witth extended encoding" mean?

Because modern machines are able to deal with 8-bit bytes more efficiently than
4-bit nibbles, each possible pair of 4-bit instructions is given an 8-bit opcode.

These are opcodes are then bundled to produce an *execution word* (EW), the
length of which varies based on word size of the target machine:

|==========================================================
| word size | opcodes/word | instructions/word
| 16 bit    | 2            | 4
| 32 bit    | 4            | 8
| 64 bit    | 8            | 16
|==========================================================

Some of the instructions require numeric parameters. These are encoded
as word-sized signed integers, and follow the EW in order.

EW's can be interpreted, executed in hardware or compiled into machine-code for a specific processor architecture. Interpretation is effective independent of specific dispatch strategies because the base unit of execution is the opcode bundle, not individual instructions.

The nAVM ISA and its encoding is also very effective implementable for FPGA's or ASIC's, both in terms of needed space as well as archivable performance.

// I don't understand what the following sentence means. IPC=instructions/cycle?
FPGA implementations for example can expect to achieve 0.5 IPC with branch and interrupt latencies of only 1 clock cycle.

Register set
~~~~~~~~~~~~

nAVM is a dual-stack architecture following MISC (Minimal Instruction-
Set) principles. As in FORTH, the main *data stack* contains data, and the *return stack* contains the current excution context (though it can also be used as a temporary stack within a procedure).

Except for LI (load-immediate) and four stack-shuffling operators, the naVM instructions all implicitly address the topmost element (or "TOS", for "top of stack") of one of the two stacks. The following internal registers are therefore not directly addressable:

.Register set
[width="70%",options="header"]
|==========================================================
| Register | Usage                      | Size
| A        | Accumulator (TOS)          | word size
| D        | Data-stack pointer         | word size
| R        | Return-stack pointer       | address word-size
| F        | Condition-code register    | byte 
| W        | internal scratch register  | word size
|==========================================================

A Register
^^^^^^^^^^

This register is an accumulator holding the results of all logic and arithmetic
operations. It is the destination for most instructions, referencing the first
data-stack element as second parameter and its actual value as first one. Thus
register A can also be seen as internal cache for the actual top-of-stack (TOS)
element, referencing the element register D points to as actual second-of-stack
(SND) parameter.

D Register
^^^^^^^^^^

The data-stack pointer is an offset into a register array of minimal eight
parameter values. Its value references the second parameter for all arithmetic
and logic operations. By processing, the D register is automatical decremented
or incremented dependend of the operation, rising a DSB exception in case of
out-of-bounce references. If this happens a unconditional branch to address 0
is taken pusing the content of register D onto the data stack. 

R Register
^^^^^^^^^^

Accessing the eight level depth address-stack is limited to the BS
(Branch Subroutine) and BR (Branch Return) instructions. The offset of these
register is incremented by processing subroutine branches (BS) and decremented
by processing the assoziated BR instruction. In case of out-of-bounce
references an RSB exception exception is raised, followed by an unconditional
branch to address 1 and pusing of its actual register content onto the
data-stack.

F register
^^^^^^^^^^

The flag register hold exactly two states: All bits set in case of a detected
condition and all bits cleared otherwise. Only the CP instruction affecting
this register.

W register
^^^^^^^^^^

The W (as working) register is an internal scratch buffer needed by some
instructions.

