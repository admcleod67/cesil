# ✨ CESIL Programming Language
*A modern C++20 re‑imagining of a classic educational language*

---

## 🕹️ What is CESIL?

CESIL — *Computer Education in Schools Instruction Language* — was a small, assembly‑style teaching language used in UK schools in the 1970s. It introduced programming concepts using:

- a single accumulator
- named store locations
- a compact instruction set
- a trailing data section for input

This project brings CESIL into the modern era with a clean, modular **C++20** implementation designed for teaching, tinkering, retro‑computing, and embedding in other tools.

---

## 🌟 Inspiration & History

My own journey with CESIL began in the early 1980s, when I attempted to build a CESIL programming environment for the **Sinclair ZX81**. That first experiment led to several more versions over the years — most of them written in BASIC — for machines such as:

- the **RML 380Z**
- the **BBC Micro**
- the **Amstrad CPC464**

Later in the decade, I created a more generic CESIL interpreter that could be compiled with standard C compilers and run under **CP/M** and **DOS**. CESIL has been a quiet companion through much of my computing life, and this modern C++20 implementation is a continuation of that long‑running fascination.

A major influence on this project is the excellent **Visual CESIL** environment created by **Andrew Jacobs**:  
<http://www.6502.org/users/obelisk/cesil/index.html>

Andrew’s work captured the spirit of CESIL beautifully, and his contributions to the retro‑computing community were immense. Andrew sadly passed away in 2021, and this project is, in part, a small homage to his creativity, generosity, and passion for classic computing.

---

## 🚀 Features

- **Front end**
    - Lexer
    - Syntax parser
    - Semantic validation (`SemanticValidator`)
    - Structured IR output

- **Interpreter**
    - Accumulator + program counter
    - Named store
    - Execution hooks for stepping & breakpoints

- **I/O abstraction (`IoHost`)** for console, IDEs, or simulators
- **Diagnostics** with line/column information
- **Reusable core library** for embedding
- **Examples** under `examples/`

---

## 👋 A Tiny CESIL Example

```cesil
PRINT "Hello, world"
HALT
%
*
```

Run it with:

```
./build/src/cli/cesil run examples/hello.ces
```

---

## 📦 Requirements

- CMake **3.20+**
- A C++20‑capable compiler (Clang, GCC, MSVC)
- Network access on first configure (Catch2 is fetched automatically)
- **Qt desktop IDE (optional):** Qt **6.5+** (`Core`, `Widgets`) when `-DCESIL_BUILD_IDE=ON`

---

## 🗂️ Project Layout

| Path | Description |
|------|-------------|
| `src/core/` | Static library `cesil-core`: lexer, parser, runtime, errors |
| `src/cli/` | CLI executable `cesil` |
| `src/ide/` | Optional Qt desktop shell `cesil-ide` (see build flags below) |
| `examples/` | Sample `.ces` programs |
| `tests/core/` | Catch2 unit tests |

---

## 🔧 Build

Out‑of‑tree build:

```
cmake -S . -B build
cmake --build build
```

Release build:

```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

**Qt IDE** (empty desktop shell; links `cesil-core`):

```
cmake -S . -B build -DCESIL_BUILD_IDE=ON -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x/<kit>
cmake --build build --target cesil-ide
```

`CMAKE_PREFIX_PATH` should point at the Qt kit directory that contains `lib/cmake/Qt6` (for example `…/Qt/6.8.0/macos` on macOS or `…/Qt/6.8.0/gcc_64` on Linux).

---

## 🧪 Tests

Run all tests:

```
ctest --test-dir build --output-on-failure
```

---

## ▶️ Running Programs

```
cesil run <file>
```

Example:

```
./build/src/cli/cesil run examples/total.ces
```

---

## 🏗️ Architecture Overview

```
Source
  ↓
Lexer
  ↓
Parser (syntax IR)
  ↓
SemanticValidator (resolved IR)
  ↓
Interpreter
  ↓
IoHost (console / IDE / simulator)
```

---

## 🔌 Embedding CESIL (minimal example)

```cpp
#include <parser/Parser.hpp>
#include <runtime/Interpreter.hpp>

auto program = cesil::parseFile("examples/hello.ces");
cesil::Interpreter vm;
vm.run(program);
```

---

## 📜 Language Reference

CESIL documentation and historical material:

- Wikipedia: *CESIL*
- CESIL.org
- Visual CESIL by Andrew Jacobs
- Original teaching materials (various scans online)

---

## 🛣️ Roadmap

- Visual debugger / stepping UI
- Syntax highlighting
- CESIL‑to‑IR visualisation
- Hardware CESIL CPU
- More examples & teaching materials

---

## 🤝 Contributing

This repository is public for reference and historical interest.  
I’m **not** accepting direct contributions or pull requests.  
Forks are welcome under the MIT license.