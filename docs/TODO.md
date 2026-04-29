# TODO

SmoothCmdFlow now has a C++17 build, a runtime `SmoothCmdFlowSetup`, a stack-based `SmoothCmdFlowDevice::execute()`, and a small terminal entry point in `src/main.cpp`.

## Done

- [x] Replace the old parser prototype in `src/main.cpp` with an interactive terminal.
- [x] Add a `Makefile`.
  - `make all` builds `build/smoothcmdflow`.
  - `make run` starts the terminal.
  - `make test` builds and runs the header test examples.
- [x] Make `SmoothCmdFlowSetup` runtime-only.
  - Stores default type IDs.
  - Represents `Array<T>` through `TypeRef`.
  - Stores function declarations with callback pointer `void* (*fp)(void* bel, void** in)`.
  - Does not serialize callbacks.
- [x] Implement `SmoothCmdFlowDevice::execute()`.
  - Uses `Lexer`.
  - Uses one stack for nested function calls.
  - Executes callbacks.
  - Supports omitted trailing `|` at EOF.
  - Supports numeric literals through setup type `I64`.
- [x] Add basic executable tests in `src/testexamples.h`.
  - Method call with omitted trailing bar.
  - Explicit trailing bar.
  - Zero-argument method.
  - N-ary global function.
  - Nested global arguments.
  - Type mismatch rejection.
  - Unknown method rejection.

## Next

- [ ] Improve `SmoothCmdFlowDevice` error messages.
  - Include function names, expected/actual type IDs, and token text where useful.
  - Add tests that assert specific error categories.
- [ ] Implement context-aware `get_command_candidate()`.
  - Current version returns all function names.
  - It should parse the current partial input and return only valid global/method/argument candidates.
- [ ] Rework `apply_autocomplete()`.
  - Current behavior replaces the whole input with the only candidate.
  - It should complete the current token/prefix and preserve the rest of the command line.
- [ ] Decide memory ownership for callback results.
  - Current callback contract returns raw `void*`.
  - Document whether callbacks return borrowed pointers, owned pointers, arena-backed pointers, or domain-managed objects.
- [ ] Add string literal support.
  - Lexer currently supports identifiers, numbers, `.`, and `|`.
  - Add quoted string tokens and map them to setup type `String`.
- [ ] Add tests for arrays and method dispatch on `Array<T>`.
  - Confirm whether array receivers call element methods and return `Array<Out>` or another agreed type behavior.
- [ ] Update `docs/language_spec.md`.
  - Fix typos such as `expernal` and `argumet`.
  - Document the runtime callback model.
  - Document EOF trailing-bar sugar separately from BNF.
  - Align examples with currently supported syntax.
- [ ] Update `README.md`.
  - Add build/run/test commands.
  - Replace stale examples with commands supported by `testexamples.h`.

