← [Project milestones index](../milestones.md)

## Milestone 0 — Core library and CLI (completed)

This document defines **Milestone 0**: the CESIL language core and command-line
runner. It establishes the reusable library and `cesil run` workflow so Milestone 1
could add a Qt IDE without reworking the interpreter.

**Release note:** Milestone 0 is complete. The CLI is part of the `0.1.0` tree
(`${PROJECT_VERSION}` from CMake); version reporting for the CLI landed with
Milestone 1.

It complements:

- [Project milestones](../milestones.md)
- [Milestone 1 — IDE edit → check → run](01-ide-mvp.md)

### Goals

- Deliver a clean C++20 CESIL front end and interpreter as a static library.
- Provide a console CLI that parses and runs `.ces` programs.
- Cover the core with Catch2 unit tests and a small set of examples.

### Out of scope for Milestone 0

- Qt desktop IDE
- Syntax highlighting or debugger UI
- Packaging / installers

---

## Deliverables

### Core library (`cesil-core`)

- Lexer, parser, and semantic validation
- Interpreter with accumulator, named store, and program counter
- Structured diagnostics (line/column)
- `IoHost` abstraction for console, IDE, or other hosts
- Optional execution hooks for future stepping and breakpoints

### CLI

- Executable `cesil` with `run <file>`
- Console-backed `IoHost`
- Diagnostics printed to stderr on parse or run failure

### Tests and examples

- Catch2 tests under `tests/core/`
- Sample programs under `examples/`

---

## Done when

- `cmake` configures and builds `cesil-core` and `cesil`
- `ctest` passes for the core suite
- `cesil run examples/hello.ces` (and similar) runs successfully
