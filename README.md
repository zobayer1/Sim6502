Microprocessor 6502 Simulation
==============================

[![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=flat&logo=c%2B%2B&logoColor=white)](https://img.shields.io/badge/C%2B%2B-00599C?style=flat&logo=c%2B%2B&logoColor=white)
[![Linux](https://img.shields.io/badge/Linux-FCC624?style=flat&logo=linux&logoColor=black)](https://img.shields.io/badge/Linux-FCC624?style=flat&logo=linux&logoColor=black)
[![CMake](https://github.com/zobayer1/Sim6502/actions/workflows/cmake-ubuntu.yml/badge.svg)](https://github.com/zobayer1/Sim6502/actions/workflows/cmake-ubuntu.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

An educational 6502 CPU simulator with a reusable C++ library (libcpu6502), a small example program (sim6502), and unit
tests using GoogleTest. The library can be installed and used by other CMake projects via find_package(cpu6502).

System Dependencies
-------------------
- C++17 compiler (e.g., GCC/g++ 15+, Clang 20+)
- CMake 3.28 or newer
- GoogleTest: Git submodule under `external/googletest`
- Platform: Linux

Build & Clean
-------------
1. Clean previous builds
    ```sh
    rm -rf build/ build-make/ cmake-build-debug/
    ```
2. Configure cmake project
    ```sh
    cmake -S . -B build
    ```
3. Build all targets (library, tests, example)
    ```sh
    cmake --build build --config Debug --target all
    ```

Install & Export
----------------
To install the cpu6502 library and export CMake targets:
```sh
cmake --install build --prefix install
```
This will install headers and libraries to the `install/` directory. Other projects can use `find_package(cpu6502)` and
`target_link_libraries(your_target cpu6502)` if you set `CMAKE_PREFIX_PATH` to the installation location.

Test
----
Run all tests:
```sh
ctest --test-dir build --output-on-failure
```

**NOTE**: `ReadWord` and `WriteWord` currently wrap from address `0xFFFF` to `0x0000` because addresses are handled as
16-bit values. This behavior is now covered by `tests/mem_test.cpp`.

Build and Run Example
---------------------
Run the main simulation executable:
```sh
cmake --build build --target sim6502
./build/examples/sim6502
```

Submodules (GoogleTest)
-----------------------
This project vendors GoogleTest as a Git submodule at `external/googletest`.

Automatic update during configure:
- CMake runs `cmake/UpdateSubmodules.cmake`, which updates submodules on configure if available.
- You can disable this behavior with:
  ```sh
  cmake -S . -B build -DGIT_SUBMODULE=OFF
  ```

Troubleshooting
---------------
- JetBrains shows <cpu6502/config.hpp> in red
  - Run configure/build so the generated header exists:
    ```sh
    cmake -S . -B build
    cmake --build build --target cpu6502
    ```
  - Ensure the IDE’s CMake profile points to the same build directory (build/), then “Reload CMake Project.”
  - Check that the header was generated at: build/src/include/cpu6502/config.hpp
- Submodule clone fails in CI with SSH (Permission denied)
  - Use HTTPS for googletest:
    ```sh
    git submodule set-url external/googletest https://github.com/google/googletest.git
    git submodule sync
    git submodule update --init --recursive
    ```
- Example path
  - The example executable lives at: ./build/examples/sim6502
- Stale build or missing headers
  - Clean and reconfigure:
    ```sh
    rm -rf build
    cmake -S . -B build
    cmake --build build --target all
    ```

Code Style
----------
Use the CMake targets to format only files in `include/`, `src/`, `tests/`, and `examples/`:
```sh
cmake --build build --target format-check
cmake --build build --target format
```
Optional: run clang-format manually if you prefer:
```sh
clang-format -i include/cpu6502/*.hpp src/*.cpp tests/*.cpp examples/*.cpp
```

Instruction Status
------------------
Instruction status is tracked in dedicated docs instead of being maintained inline in this README:

- Implemented and tested instructions: [`docs/cpu-instructions.md`](./docs/cpu-instructions.md)
- Current backlog / TODO: [`docs/todo.md`](./docs/todo.md)

Contributors
------------
We welcome contributions of all sizes. Please read the guidelines in [CONTRIBUTORS.md](./CONTRIBUTORS.md) for workflow,
commit messages, formatting, testing, and how to add yourself to the contributors list.

CPU 6502 References
-------------------
- The 6502 Instructions (masswerk): https://www.masswerk.at/6502/6502_instruction_set.html
- 6502.org (datasheets, tutorials, forums): https://www.6502.org/
- Visual 6502 (transistor-level simulator): https://visual6502.org/
- Easy 6502 (assembler tutorial): https://skilldrick.github.io/easy6502/
- WDC W65C02S (datasheet): https://wdc65xx.com/wp-content/uploads/w65c02s.pdf
- MOS 6502 MPU (datasheet): http://archive.6502.org/datasheets/mos_6500_mpu_nov_1976.pdf

License
-------
This is a demonstration project for educational purposes. It is provided "as is" without any warranties. Licensed under
the MIT License. See [LICENSE](./LICENSE) for details.
