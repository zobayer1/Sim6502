# Implemented CPU Instructions

This document tracks the 6502 instructions and addressing modes that are currently implemented and covered by tests.

## Command behavior summary
- `LDA` loads a value into the accumulator (`A`) and updates the `Z` and `N` flags from the loaded result.
- `LDX` loads a value into the `X` register and updates the `Z` and `N` flags from the loaded result.
- `LDY` loads a value into the `Y` register and updates the `Z` and `N` flags from the loaded result.
- `STA` stores the accumulator (`A`) into memory and leaves the register contents unchanged.
- `STX` stores the `X` register into memory and leaves the register contents unchanged.
- `STY` stores the `Y` register into memory and leaves the register contents unchanged.
- `NOP` performs no data movement and only advances execution state.

## Load / Store
- [x] `LDA` (Immediate) – Loads the next byte directly into `A`; updates `Z` and `N`. Cycles: 2. `PC`: +2.
- [x] `LDA` (Zero Page) – Loads `A` from a one-byte zero-page address; updates `Z` and `N`. Cycles: 3. `PC`: +2.
- [x] `LDA` (Absolute) – Loads `A` from a 16-bit address; updates `Z` and `N`. Cycles: 4. `PC`: +3.
- [x] `LDA` (Zero Page,X) – Loads `A` from `(ZP + X) & 0xFF`; updates `Z` and `N`. Cycles: 4 fixed. `PC`: +2.
- [x] `LDA` (Absolute,X) – Loads `A` from `abs + X`; updates `Z` and `N`. Cycles: 4, or 5 on page cross. `PC`: +3.
- [x] `LDA` (Absolute,Y) – Loads `A` from `abs + Y`; updates `Z` and `N`. Cycles: 4, or 5 on page cross. `PC`: +3.
- [x] `LDA` (Indirect,X) – Loads `A` through a zero-page pointer indexed by `X`; updates `Z` and `N`. Cycles: 6 fixed. `PC`: +2.
- [x] `LDA` (Indirect),Y – Loads `A` through a zero-page pointer, then adds `Y`; updates `Z` and `N`. Cycles: 5, or 6 on page cross. `PC`: +2.
- [x] `LDX` (Immediate) – Loads the next byte directly into `X`; updates `Z` and `N`. Cycles: 2. `PC`: +2.
- [x] `LDX` (Zero Page) – Loads `X` from a one-byte zero-page address; updates `Z` and `N`. Cycles: 3. `PC`: +2.
- [x] `LDX` (Absolute) – Loads `X` from a 16-bit address; updates `Z` and `N`. Cycles: 4. `PC`: +3.
- [x] `LDX` (Zero Page,Y) – Loads `X` from `(ZP + Y) & 0xFF`; updates `Z` and `N`. Cycles: 4 fixed. `PC`: +2.
- [x] `LDX` (Absolute,Y) – Loads `X` from `abs + Y`; updates `Z` and `N`. Cycles: 4, or 5 on page cross. `PC`: +3.
- [x] `LDY` (Immediate) – Loads the next byte directly into `Y`; updates `Z` and `N`. Cycles: 2. `PC`: +2.
- [x] `LDY` (Zero Page) – Loads `Y` from a one-byte zero-page address; updates `Z` and `N`. Cycles: 3. `PC`: +2.
- [x] `LDY` (Absolute) – Loads `Y` from a 16-bit address; updates `Z` and `N`. Cycles: 4. `PC`: +3.
- [x] `LDY` (Zero Page,X) – Loads `Y` from `(ZP + X) & 0xFF`; updates `Z` and `N`. Cycles: 4 fixed. `PC`: +2.
- [x] `LDY` (Absolute,X) – Loads `Y` from `abs + X`; updates `Z` and `N`. Cycles: 4, or 5 on page cross. `PC`: +3.
- [x] `STA` (Zero Page) – Stores `A` into a one-byte zero-page address without changing `A`. Cycles: 3. `PC`: +2.
- [x] `STA` (Absolute) – Stores `A` into a 16-bit address without changing `A`. Cycles: 4. `PC`: +3.
- [x] `STA` (Zero Page,X) – Stores `A` into `(ZP + X) & 0xFF` without changing `A`. Cycles: 4 fixed. `PC`: +2.
- [x] `STA` (Absolute,X) – Stores `A` into `abs + X` without changing `A`. Cycles: 5 fixed. `PC`: +3.
- [x] `STA` (Absolute,Y) – Stores `A` into `abs + Y` without changing `A`. Cycles: 5 fixed. `PC`: +3.
- [x] `STA` (Indirect,X) – Stores `A` through a zero-page pointer indexed by `X` without changing `A`. Cycles: 6 fixed. `PC`: +2.
- [x] `STA` (Indirect),Y – Stores `A` through a zero-page pointer, then adds `Y`, without changing `A`. Cycles: 6 fixed. `PC`: +2.
- [x] `STX` (Zero Page) – Stores `X` into a one-byte zero-page address without changing `X`. Cycles: 3. `PC`: +2.
- [x] `STX` (Absolute) – Stores `X` into a 16-bit address without changing `X`. Cycles: 4. `PC`: +3.
- [x] `STX` (Zero Page,Y) – Stores `X` into `(ZP + Y) & 0xFF` without changing `X`. Cycles: 4 fixed. `PC`: +2.
- [x] `STY` (Zero Page) – Stores `Y` into a one-byte zero-page address without changing `Y`. Cycles: 3. `PC`: +2.
- [x] `STY` (Absolute) – Stores `Y` into a 16-bit address without changing `Y`. Cycles: 4. `PC`: +3.
- [x] `STY` (Zero Page,X) – Stores `Y` into `(ZP + X) & 0xFF` without changing `Y`. Cycles: 4 fixed. `PC`: +2.

## Other / Misc
- [x] `NOP` – Performs no data or flag changes. Cycles: 2. `PC`: +1.

## Cycles and PC
- The emulator is cycle-driven: `CPU::Execute(u32 exec_cycles)` keeps running until the requested cycle budget is consumed.
- `PC` advances as bytes are fetched: opcode fetches use `FetchByte()`, one-byte operands add one more byte, and absolute operands use `FetchWord()` to add two bytes.
- Immediate and zero-page instructions therefore advance `PC` by 2 bytes, absolute instructions advance `PC` by 3 bytes, and `NOP` advances `PC` by 1 byte.
- Implemented load instructions may gain an extra cycle on page cross for `Absolute,X`, `Absolute,Y`, and `(Indirect),Y`.
- Implemented store instructions use fixed cycle counts for their supported indexed modes; they do not add a page-cross penalty in the current implementation.
- Zero-page indexed modes wrap with 8-bit behavior, and `(Indirect,X)` also wraps its zero-page pointer lookup.

## Notes
- Cycle and `PC` behavior above reflect the current implementation in `src/cpu.cpp` and the assertions in `tests/cpu_test.cpp`.
- Tests for implemented behavior live in `tests/cpu_test.cpp`.
