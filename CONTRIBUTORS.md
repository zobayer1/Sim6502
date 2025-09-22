# Contributors Guide

Thank you for your interest in improving Sim6502! This file covers both recognition (who contributed) and guidelines (how to contribute).

If you submit a PR that gets merged, please add yourself to the list below in the format shown.

## How to Contribute

- Discuss large changes in an issue first so we can align on scope and design.
- Small fixes are welcome directly as PRs.

### Workflow
1. Fork the repository and create a feature branch:
   - Branch name suggestion: `feat/<topic>` or `fix/<topic>`
2. Make changes and keep commits focused.
3. Run code formatting (only the project sources are targeted):
   - `cmake --build build --target format` (or run `clang-format -i` if you prefer)
4. Build and test locally:
   - `cmake -S . -B build`
   - `cmake --build build -j`
   - `ctest --test-dir build --output-on-failure`
5. Update submodules if needed (GoogleTest is a submodule):
   - Configure normally; submodules are auto-updated by `cmake/UpdateSubmodules.cmake`.
   - To skip updating submodules during configure: `-DGIT_SUBMODULE=OFF`.
6. Submit a Pull Request (PR) and link any related issues.

### Commit Messages
- Use clear messages that explain the why and the what.
- Conventional Commits are encouraged (optional):
  - `feat: add XYZ`, `fix: correct ABC`, `docs: update README`, `build:`, `test:`, etc.

### Return-Value Discarding (library API)
- By default, some APIs are annotated `[[nodiscard]]` to catch mistakes.
- If you intentionally want to discard return values:
  - Per call: assign to a `[[maybe_unused]]` variable.
  - Build-time: configure with `-DCPU6502_ALLOW_DISCARD_RETURNS=ON` to disable the warnings in your build.

### Pull Request Checklist
- [ ] Code formatted (see `format` and `format-check` targets)
- [ ] Builds on Linux with CMake
- [ ] Tests updated/added and passing (`ctest`)
- [ ] No new warnings, or warnings explained in PR description
- [ ] Public headers stable (if you changed the library API, document it)

### Code Style & Tooling
- C++17
- Formatting via `clang-format` (targets: `format`, `format-check`)
- Tests via GoogleTest (`ctest`)
- CMake 3.28+

### Legal
- Project is MIT-licensed. Include third-party license notices as needed.
- No CLA required. If your organization requires a DCO sign-off, you can use:
  - `git commit -s` (adds the `Signed-off-by:` trailer)

## Contributors

Add yourself in this list with one line per contributor (sorted by first contribution date):

- Zobayer Hasan (GitHub: @zobayer1, email: zobayer1@gmail.com) — project author and maintainer

---

## Maintainers

- Zobayer Hasan (@zobayer1, zobayer1@gmail.com)

## Questions

- Open an issue or start a discussion if you have questions about the process or need guidance.
