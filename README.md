# CESIL Programming Language

A modern **C++20** implementation of **CESIL** (Computer Education in Schools Instruction Language): a small educational assembly-style language with a single accumulator, named store locations, and a trailing data section for `IN`.

This repository provides a reusable **core library**, a **command-line runner**, unit tests, and example programs.

## Features

- **Front end**: lexer, syntax parser, and semantic validation (`SemanticValidator`) producing structured IR
- **Interpreter**: executes `Instruction` streams with an accumulator, program counter, and named store
- **I/O abstraction** (`IoHost`) for console, future IDE, or simulator front-ends
- **Execution hooks** for stepping and breakpoints
- **Diagnostics** with line and column information where available
- **Examples** under `examples/`

## Requirements

- **CMake** 3.20 or newer
- A **C++20**-capable compiler (Clang, GCC, or MSVC)
- **Network access on the first configure**: tests depend on **Catch2 v3.4.0**, which CMake fetches via `FetchContent` (see root `CMakeLists.txt`)

## Project layout

| Path | Description |
|------|-------------|
| `core/` | Static library **cesil-core**: public headers under `core/include/cesil/`, sources under `core/src/` |
| `cli/` | Executable target **cesil-cli**; the built binary is named **`cesil`** |
| `examples/` | Sample `.ces` sources |
| `core/tests/` | Test executable **cesil_tests** (Catch2) |

## Build

Configure and build out-of-tree:

```bash
cmake -S . -B build
cmake --build build
```

Release build (single-configuration generators):

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## Tests

```bash
ctest --test-dir build --output-on-failure
```

CTest registers one test, **`cesil_tests`**, which runs the Catch2-linked binary (lexer, parser, semantics, and smoke coverage).

## Running programs

After a successful build, run a CESIL source file with:

```bash
./build/cli/cesil run examples/hello.ces
./build/cli/cesil run examples/total.ces
```

The exact path to the `cesil` binary matches CMake’s output directory for the `cli` target; with a default Ninja or Makefile build it is typically **`build/cli/cesil`** (on Windows, `build/cli/cesil.exe` or under `build/cli/Debug/` depending on the generator).

Usage:

```text
cesil run <file>
```

Runtime numeric input for `IN` is read from standard input; output uses standard output.

## Architecture (overview)

1. **Lexer** tokenizes source (identifiers, numbers, strings, punctuation, newlines).
2. **Parser** (`Parser::parse` / `parseSyntax`) builds syntax-level IR: instructions with opcodes and line numbers, **raw operands** (`RawOperand`), label maps, and raw data tokens.
3. **SemanticValidator** resolves operands, validates data integers and jump targets, and fills final `Operand` values on each `Instruction`.
4. **Interpreter** runs the resolved IR against an `IoHost`.

Public API entry points for embedding include `cesil/parser/parser.hpp`, `cesil/interpreter/interpreter.hpp`, `cesil/lexer/lexer.hpp`, and related headers under `core/include/cesil/`.

## Language reference

CESIL is documented in teaching materials and on the web, for example:

- [Wikipedia: CESIL](https://en.wikipedia.org/wiki/Cesil)
- [CESIL.org](https://cesil.org/)

This project aims to accept common CESIL program and data-section layouts (`%` … `*`); see `examples/` for runnable samples.
