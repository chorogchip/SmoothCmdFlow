# TODO

This project is currently a SmoothCmdFlow language prototype: the design notes are ahead of the reusable API, and `src/main.cpp` contains most of the working parser experiment.

## Immediate

- [ ] Make the project compile from a clean checkout.
  - `src/Lexer.h` contains an unfinished static `lex(std::string_view)` implementation mixed with code that expects `source_` and an instance method.
  - `src/main.cpp` constructs `Lexer lexer(source);` and calls `lexer.lex()`, but `Lexer.h` does not currently define that constructor or member function.
- [ ] Decide the first build system.
  - Add either a minimal `CMakeLists.txt` or another explicit build command documented in `README.md`.
  - Keep the first target small: one parser demo executable and, later, one library target.
- [ ] Move the frontend prototype out of `main.cpp`.
  - Extract `FrontendError`, lexer, normalizer, parser, AST nodes, and pretty printing into focused source/header files.
  - Keep `main.cpp` as a thin demo or test driver.
- [ ] Add basic parser regression tests.
  - Cover the currently valid examples:
    - `entity.filter.stepblock.is stone | summon lightning`
    - `entity.filter.stepblock.is stone | summon lightning | player`
    - `entity.filter.type.is creeper | kill`
  - Cover invalid/edge examples already listed in `main.cpp`.

## Language Semantics

- [ ] Finalize token classification.
  - Replace the hard-coded `kId0`/`kId1` sets with data loaded from `SmoothCmdFlowSetup`.
  - Decide how numeric literals, quoted strings, enum values, and domain identifiers are represented.
- [ ] Define the canonical AST shape.
  - Current demo AST is `Leaf(id0)` and `Call(id1, children)`.
  - The docs describe global functions, method functions, argument functions, default/array types, and return types; the AST needs enough metadata to support those semantics.
- [ ] Implement semantic validation after parsing.
  - Validate global function use at sentence start and argument start.
  - Validate method function use after compatible receiver types.
  - Validate argument function use when a specific input type is expected.
  - Enforce the 0-argument or 1-argument function rule.
- [ ] Implement derived type behavior.
  - `Array<T>` should expose functions of its original type `T` without adding broad method sets directly to array types.
  - Confirm and encode the "filter the most recent Array" decision from `docs/solved_design_problems/filter.md`.
- [ ] Decide whether blank-name proxy functions are part of the language.
  - `docs/solved_design_problems/too_many_argument.md` suggests blank functions for tuple-like input such as positions.
  - If accepted, update the grammar/spec and parser design around semantic parsing.

## Public API

- [ ] Design `SmoothCmdFlowSetup`.
  - Store function declarations: name, arity, function kind, receiver type, argument type, return type, and candidate priority/order.
  - Store default type declarations; represent array-ness in function signatures, not as separately registered types.
  - Enforce the "max 8 choices per context" rule during setup validation.
- [ ] Implement `SmoothCmdFlowDevice::execute()`.
  - Parse the current input.
  - Run semantic validation.
  - Return success/failure and populate `err_msg_` with actionable messages.
- [ ] Implement `SmoothCmdFlowDevice::get_command_candidate()`.
  - Return context-aware autocomplete candidates from the current cursor/input.
  - Include candidates for global, method, and argument functions according to the current parse state.
- [ ] Revisit `apply_autocomplete()`.
  - Current behavior replaces the entire input with the only candidate.
  - It should probably complete only the current token/prefix and preserve the surrounding command.

## Documentation

- [ ] Fix and expand `docs/language_spec.md`.
  - Correct typos such as `expernal` and `argumet`.
  - Add a precise grammar section for the normalized syntax currently used by the parser.
  - Separate lexical syntax, normalization rules, parse grammar, and semantic rules.
- [ ] Update `README.md`.
  - Document the current project status honestly: prototype parser, incomplete device/setup API.
  - Add build/run instructions once a build system exists.
  - Keep the examples aligned with actual accepted parser behavior.
- [ ] Convert `docs/idea_naive.txt` into structured design history.
  - Preserve useful examples and rejected grammar attempts.
  - Move active decisions into `language_spec.md`; keep old notes as rationale.
- [ ] Expand the Minecraft domain sketch.
  - Split original commands, proposed SCF commands, and unresolved mappings.
  - Mark which commands are supported by the current setup model once setup exists.

## Quality

- [ ] Add formatting and compile checks.
  - Choose a C++ standard and compiler flags.
  - Add a one-command local check documented in the README.
- [ ] Add unit tests for normalization.
  - Verify that every raw `Id0` gets a synthesized `|`.
  - Verify that `.` is discarded.
  - Verify that missing closing bars for raw `Id1` are appended based only on raw bar counts.
- [ ] Add unit tests for error messages.
  - Empty input.
  - Unknown token.
  - Unexpected top-level `|`.
  - Unterminated `Id1` call.
  - Empty nested sequence.
- [ ] Decide whether Korean/English docs are both first-class.
  - Current docs mix both. Keep this if intentional, or define one primary language and translate key specs.
