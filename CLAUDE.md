# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project

Sim6502 is an educational MOS 6502 CPU simulator: a reusable C++17 static library (`cpu6502`), a demo executable
(`sim6502`), and a GoogleTest suite. The library is installable and consumable downstream via `find_package(cpu6502)`.

`AGENTS.md` and `.github/copilot-instructions.md` carry the same project guidance for other agents — keep them in sync
when the build workflow or conventions here change.

## Commands

```sh
cmake -S . -B build                                   # configure (also updates the googletest submodule)
cmake --build build --config Debug --target all       # build library, tests, example
ctest --test-dir build --output-on-failure            # run all tests
cmake --build build --target format-check             # clang-format dry run (CI gate)
cmake --build build --target format                   # apply clang-format in place
cmake --build build --target sim6502 && ./build/examples/sim6502   # build + run the demo
cmake --install build --prefix install                # install headers/lib + export CMake package
```

Run a single test — `gtest_discover_tests` registers each case as `Suite.TestName`:

```sh
ctest --test-dir build -R 'CPUTest.Execute_LDAAbsoluteX_PageCross_Consumes5Cycles' --output-on-failure
ctest --test-dir build -R 'CPUTest.Execute_STA' --output-on-failure   # regex over a group
./build/tests/cpu6502_tests --gtest_filter='MemoryTest.*'             # or drive the gtest binary directly
```

Skip the submodule update on configure with `-DGIT_SUBMODULE=OFF`. Warnings (`-Wall -Wextra -Wpedantic -Wconversion
-Wsign-conversion -Wshadow`) are on by default; `-DCPU6502_ENABLE_WARNINGS=OFF` disables them.

CI (`.github/workflows/cmake-ubuntu.yml`) builds **Release**, then runs `format-check`, then `ctest`. Note that
`tests/cpu_test.cpp` has an `#ifdef NDEBUG`-gated case, so a Debug run and a Release run execute different test sets.

CI pins **clang-format 22.1.8** (`CLANG_FORMAT_VERSION` in the workflow, pip-installed into a venv and passed via
`-DCLANG_FORMAT_EXECUTABLE`), because formatting genuinely differs between clang-format versions — `ubuntu-latest`
otherwise ships 18.1.3, which wraps some constructs differently and silently broke CI once. Configure prints the
version it found; match it locally with `pip install clang-format==22.1.8` and
`-DCLANG_FORMAT_EXECUTABLE=<venv>/bin/clang-format` if yours disagrees. Long `<<` chains mixing adjacent string
literals (`"text " << "0x" << ...`) near the 120-column limit are the construct versions disagree on — prefer short
statements or a loop.

## Architecture

Two classes, no namespace, include guards, plain global types (`Byte`/`Word`/`u32` are `using` aliases in `mem.hpp`).

- `Memory` (`include/cpu6502/mem.hpp`, `src/mem.cpp`) — flat 64 KB `Byte` array, no bus/mapping layer. Word access is
  little-endian and wraps `0xFFFF → 0x0000` because addresses are 16-bit; `tests/mem_test.cpp` pins that behavior.
  `Memory` never touches cycle counts.
- `CPU` (`include/cpu6502/cpu.hpp`, `src/cpu.cpp`) — holds `PC/SP/A/X/Y`, a bitfield `StatusFlags PS`, a public
  `cycles` counter, and a `Memory&`. Registers are public so tests can seed them directly (`cpu.X = 0x10;`).

**Execution is cycle-driven, not instruction-driven.** `Execute(u32 exec_cycles)` computes `target = cycles +
exec_cycles` and loops decoding opcodes until `cycles >= target`. A caller therefore hands over an exact cycle budget;
an under-budget call leaves an instruction half-executed and an over-budget call runs into the next one. `Reset()` reads
the vector at `0xFFFC/0xFFFD`, sets `SP = 0x00FD`, sets `I` and `U`, and seeds `cycles = 6` — tests assert those exact
values, so all cycle assertions are written relative to a captured `start = cpu.cycles`.

**Cycles are accumulated by the helpers, never by the opcode cases.** This is the single most important convention:

- `FetchByte()` / `FetchWord()` — read at `PC`, advance `PC`, add 1 / 2 cycles.
- `ReadByteAndTick(addr)` / `WriteByteAndTick(addr, v)` — the data access, +1 cycle.
- `AddrZeroPage`, `AddrAbsolute`, `AddrZeroPageX/Y`, `AddrAbsoluteX/Y`, `AddrIndexedIndirectX`,
  `AddrIndirectIndexedY` — return an effective address and charge the mode's own penalties.

Opcode cases in the `Execute` switch are one-liners that compose these (`LDA(ReadByteAndTick(AddrAbsoluteX()));`); the
cycle count falls out of the composition. Only cases with no memory operand (`NOP`, the `BRK` stub) touch `cycles`
directly. Do not open-code address math or `cycles +=` inside a case.

**Read vs. store addressing modes are separate helpers.** Loads add a page-cross penalty (`AddrAbsoluteX`,
`AddrAbsoluteY`, `AddrIndirectIndexedY` compare `base & 0xFF00` against `addr & 0xFF00`); stores always pay the extra
cycle, so they use the `...Store` variants (`AddrAbsoluteXStore`, `AddrAbsoluteYStore`,
`AddrIndirectIndexedYStore`). Picking the wrong one is the easiest way to get cycle counts subtly wrong.

Zero-page indexed modes wrap in 8 bits via `static_cast<Byte>(base + X)`, and `(Indirect,X)` also wraps its zero-page
pointer high-byte lookup — follow that idiom rather than masking by hand.

Unknown opcodes `std::abort()` in Debug builds and are silently skipped under `NDEBUG`.

## Adding an instruction

1. Implement the opcode case in the `Execute` switch in `src/cpu.cpp`, reusing existing addressing helpers. Add a new
   `Addr*` helper only when the mode genuinely does not exist yet; register-effect helpers like `LDA/LDX/LDY` own the
   flag updates (`Z` from `== 0`, `N` from bit 7).
2. Add cases to `tests/cpu_test.cpp` following the established shape: write the reset vector at `0xFFFC/0xFFFD`, write
   raw opcode bytes at `0x8000`, `Reset()`, seed registers, capture `start = cpu.cycles`, `Execute(<exact budget>)`,
   then assert the result, `PC`, and `cycles == start + <budget>`. Cover zero-page wrap and both page-cross branches.
   There is no assembler helper — programs are always raw bytes written into `Memory`.
3. Move the entry from `docs/todo.md` to `docs/cpu-instructions.md` with its cycle count and `PC` delta. These two
   files, not `README.md`, are the source of truth for implemented vs. planned opcodes.
4. Optionally extend `examples/main.cpp`, which advances the CPU in stepped cycle budgets and prints state between
   steps — its `Execute(n)` budgets must be updated to match any added instruction bytes.

## Conventions and boundaries

- Style is `clang-format` (LLVM base, 4 spaces, 120 columns) — run `format` before committing; CI fails on drift.
- Write self-explanatory code with few inline comments; comment only public API or non-obvious behavior. The existing
  one-line `// LDA abs,X`-style opcode comments in the switch are the exception worth matching.
- Headers under `include/cpu6502/` are installed for downstream consumers — treat signature changes as public API
  changes. The consumable target is `cpu6502::cpu6502`; use that namespaced alias in any new target.
- `src/config.hpp.in` generates `build/src/include/cpu6502/config.hpp`; `<cpu6502/config.hpp>` will not resolve (in an
  IDE or otherwise) until the project has been configured and built at least once.
- Never edit vendored `external/googletest/` or generated output under `build/`, `cmake-build-debug/`,
  `build-debug-check/`, or `install/` — all are gitignored.
