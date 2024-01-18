Microprocessor 6502 Simulator
=============================

Build
-----
1. Configure cmake project
    ```sh
    cmake -S . -B build
    ```
2. Build project
    ```sh
    cmake --build build --config Debug --target all
    ```
3. Run test
    ```sh
    ctest --test-dir build --output-on-failure
    ```
4. Run executable
    ```sh
    ./build/cpu6502/cpu6502
    ```

GoogleTest
----------
If submodule update fails, please checkout [googletest](https://github.com/google/googletest) as a submodule

```sh
git submodule add git@github.com:google/googletest.git external/googletest
```
