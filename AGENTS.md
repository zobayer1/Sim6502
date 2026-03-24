# AGENTS.md

## Repo map
- `src/` contains the reusable `cpu6502` library implementation: `src/cpu.cpp` executes opcodes and counts cycles; `src/mem.cpp` provides 64 KB byte-addressable RAM.
- `include/cpu6502/` is the public API (`cpu.hpp`, `mem.hpp`). The code uses simple global types/classes, include guards, and no namespace wrapping.
- `examples/main.cpp` is the best end-to-end usage sample: it writes a reset vector, loads program bytes into memory, calls `Reset()`, then advances the CPU with exact cycle budgets.
- `tests/` is the behavioral spec. `tests/cpu_test.cpp` encodes expected opcodes, PC movement, zero-page wrapping, and page-cross cycle penalties; `tests/mem_test.cpp` defines memory boundary behavior.
- Do not edit vendored code under `external/googletest/` or generated/build output under `build/`, `cmake-build-debug/`, or `install/`.

## Architecture and data flow
- `CPU` owns registers/state but only accesses RAM through a stored `Memory&` (`include/cpu6502/cpu.hpp`). There is no bus abstraction yet.
- Instruction execution is cycle-driven, not instruction-count-driven: `CPU::Execute(u32 exec_cycles)` loops until `cycles` reaches the target (`src/cpu.cpp`).
- Keep cycle accounting in the same style as existing code: opcode fetches increment through `FetchByte/FetchWord`, memory reads use `ReadByteAndTick`, and indexed addressing helpers apply wrap/page-cross penalties.
- `Reset()` reads the reset vector from `0xFFFC/0xFFFD`, sets `SP = 0x00FD`, sets `I`/`U`, and initializes `cycles = 6`. Tests assume this exact behavior.
- Word accesses are little-endian (`src/mem.cpp`). `ReadWord`/`WriteWord` wrap from `0xFFFF` to `0x0000` because addresses are 16-bit; `tests/mem_test.cpp` covers this behavior.

## Build / test / run
- Configure: `cmake -S . -B build`
- Build everything: `cmake --build build --config Debug --target all`
- Run tests: `ctest --test-dir build --output-on-failure`
- Run formatter checks: `cmake --build build --target format-check`
- Apply formatting: `cmake --build build --target format`
- Build/run the sample: `cmake --build build --target sim6502 && ./build/examples/sim6502`
- Install/export package: `cmake --install build --prefix install`

## CMake and integration points
- The consumable library target is `cpu6502::cpu6502` (`src/CMakeLists.txt`, `examples/CMakeLists.txt`). Mirror that target name in any new examples/tests.
- `src/config.hpp.in` generates a public header at `build/src/include/cpu6502/config.hpp`; if IDEs cannot resolve `<cpu6502/config.hpp>`, configure/build first.
- The project exports a CMake package (`cmake/cpu6502Config.cmake.in`, root `CMakeLists.txt`) for downstream `find_package(cpu6502)` use.
- GoogleTest is a git submodule at `external/googletest`; configure auto-runs `cmake/UpdateSubmodules.cmake` unless `-DGIT_SUBMODULE=OFF` is passed.

## Codebase-specific editing rules
- When adding an instruction, implement the opcode path in `src/cpu.cpp` and add matching tests in `tests/cpu_test.cpp` that assert register result, `PC`, and exact cycle consumption.
- Follow existing addressing-mode patterns instead of open-coding address math inside the switch. Example: zero-page indexed modes wrap with `static_cast<Byte>(base + X/Y)`; absolute indexed modes add one cycle only on page cross.
- Use memory-loaded programs in tests/examples rather than helper assemblers. Existing tests write raw opcodes to `0x8000` and set the reset vector manually.
- Keep public API changes intentional: headers in `include/cpu6502/` are installed for downstream consumers, so changing signatures affects the package interface.
- Style is `clang-format` with 4 spaces, LLVM-based rules, and 120-column limit (`.clang-format`).
- Instruction status now lives in `docs/cpu-instructions.md` and `docs/todo.md`; use those files instead of `README.md` when checking implemented vs planned opcodes.
