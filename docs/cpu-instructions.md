# Implemented CPU Instructions

This document tracks the 6502 instructions and addressing modes that are currently implemented and covered by tests.

## Load / Store
- [x] LDA (Immediate) – Load accumulator with next byte; sets Z if result==0, N from bit 7.
- [x] LDA (Zero Page) – Load A from zero-page address (1-byte address); sets Z, N.
- [x] LDA (Absolute) – Load A from 16-bit absolute address; sets Z, N.
- [x] LDA (Zero Page,X) – Load A from (ZP + X) & 0xFF; sets Z, N. Fixed 4 cycles.
- [x] LDA (Absolute,X) – Load A from (abs + X); sets Z, N. +1 cycle on page cross.
- [x] LDA (Absolute,Y) – Load A from (abs + Y); sets Z, N. +1 cycle on page cross.
- [x] LDA (Indirect,X) – Load A from address at ZP pointer (operand + X) & 0xFF; sets Z, N. Fixed 6 cycles (ZP wraps).
- [x] LDA (Indirect),Y – Load A from address at ZP pointer then + Y; sets Z, N. +1 cycle on page cross.
- [x] LDX (Immediate) – Load X with next byte; sets Z if result==0, N from bit 7.
- [x] LDX (Zero Page) – Load X from zero-page address; sets Z, N.
- [x] LDX (Absolute) – Load X from absolute address; sets Z, N.
- [x] LDX (Zero Page,Y) – Load X from (ZP + Y) & 0xFF; sets Z, N. Fixed 4 cycles.
- [x] LDX (Absolute,Y) – Load X from (abs + Y); sets Z, N. +1 cycle on page cross.
- [x] LDY (Immediate) – Load Y with next byte; sets Z if result==0, N from bit 7.
- [x] LDY (Zero Page) – Load Y from zero-page address; sets Z, N.
- [x] LDY (Absolute) – Load Y from absolute address; sets Z, N.
- [x] LDY (Zero Page,X) – Load Y from (ZP + X) & 0xFF; sets Z, N. Fixed 4 cycles.
- [x] LDY (Absolute,X) – Load Y from (abs + X); sets Z, N. +1 cycle on page cross.
- [x] STA (Zero Page) – Store A to zero-page address. Fixed 3 cycles.
- [x] STA (Absolute) – Store A to 16-bit absolute address. Fixed 4 cycles.
- [x] STA (Zero Page,X) – Store A to (ZP + X) & 0xFF. Fixed 4 cycles.
- [x] STA (Absolute,X) – Store A to (abs + X). Fixed 5 cycles.
- [x] STA (Absolute,Y) – Store A to (abs + Y). Fixed 5 cycles.
- [x] STA (Indirect,X) – Store A to address at ZP pointer (operand + X) & 0xFF. Fixed 6 cycles (ZP wraps).
- [x] STA (Indirect),Y – Store A to address at ZP pointer then + Y. Fixed 6 cycles.
- [x] STX (Zero Page) – Store X to zero-page address. Fixed 3 cycles.
- [x] STX (Absolute) – Store X to 16-bit absolute address. Fixed 4 cycles.
- [x] STX (Zero Page,Y) – Store X to (ZP + Y) & 0xFF. Fixed 4 cycles.
- [x] STY (Zero Page) – Store Y to zero-page address. Fixed 3 cycles.
- [x] STY (Absolute) – Store Y to 16-bit absolute address. Fixed 4 cycles.
- [x] STY (Zero Page,X) – Store Y to (ZP + X) & 0xFF. Fixed 4 cycles.

## Other / Misc
- [x] NOP – No operation. Fixed 2 cycles.

## Notes
- Cycle counting is accurate for implemented LDA/LDX/LDY modes, including page-cross penalties for Absolute,X / Absolute,Y and (Indirect),Y.
- Store instructions use fixed cycle counts for their implemented indexed modes, including `STA Absolute,X`, `STA Absolute,Y`, and `STA (Indirect),Y`.
- Zero Page indexed modes and `(Indirect,X)` use their fixed cycle counts and zero-page wrap behavior.
- Tests for implemented behavior live in `tests/cpu_test.cpp`.
