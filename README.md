Microprocessor 6502 Simulation
==============================

An educational 6502 CPU simulator with a reusable C++ library (libcpu6502), a small example program (sim6502), and unit
tests using GoogleTest. The library can be installed and used by other CMake projects via find_package(cpu6502).

System Dependencies
-------------------
- C++17 compiler (e.g., GCC/g++, Clang)
- CMake 3.28 or newer
- GoogleTest: Git submodule under `external/googletest`

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
3. Build project
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

Code Style
----------
Use the CMake targets to format only files in `include/`, `src/`, `tests/`, and `examples/`:
```sh
cmake --build build --target format-check  # verify formatting (no changes)
cmake --build build --target format        # apply formatting in-place
```
Optional: run clang-format manually if you prefer:
```sh
clang-format -i include/cpu6502/*.hpp src/*.cpp tests/*.cpp examples/*.cpp
```

CPU 6502 References
-------------------
- The 6502 Instruction Set (masswerk): https://www.masswerk.at/6502/6502_instruction_set.html
- 6502.org (datasheets, tutorials, forums): https://www.6502.org/
- Visual 6502 (transistor-level simulator): https://visual6502.org/
- Easy 6502 (assembler tutorial): https://skilldrick.github.io/easy6502/
- WDC W65C02S Datasheet (modern CMOS 6502): https://wdc65xx.com/wp-content/uploads/w65c02s.pdf
- MOS Technology 6502 Microprocessor Datasheet: http://archive.6502.org/datasheets/mos_6500_mpu_nov_1976.pdf

License
-------
This is a demonstration project for educational purposes. It is provided "as is" without any warranties. Licensed under
the MIT License. See [LICENSE](./LICENSE) for details.
