# Copilot Instructions

Use the repository root `AGENTS.md` as the primary source of project-specific guidance.

## Build and test
Use the repo's CMake workflow:

```sh
cmake -S . -B build
cmake --build build --config Debug --target all
ctest --test-dir build --output-on-failure
```

## Formatting
- Format generated code with the repository's root `.clang-format`.
- Prefer the provided CMake targets:

```sh
cmake --build build --target format-check
cmake --build build --target format
```

## Code generation style
- Generate little to no inline comments.
- Prefer self-explanatory code.
- Only add docstrings or comments when they are necessary for public APIs or non-obvious behavior.
