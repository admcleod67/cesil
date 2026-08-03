← [Language reference index](README.md)

# Machine model

## Overview

CESIL is a small, non-interactive, assembly-like instructional language.

| Property | Rule | Status |
|----------|------|--------|
| Accumulator | One integer accumulator holds the current working value | **specified** |
| Store locations | Named scalar integer variables (“stores”); no arrays or structures | **specified** |
| Stack / call stack | None; no subroutines or functions in classic CESIL | **specified** |
| Interaction | No live keyboard input; all numeric input comes from the program data section via `IN` | **specified** |
| Instruction count | Fourteen classic instructions | **specified** |

Sources: [cesil.org](https://cesil.org) architecture notes; [Wikipedia](https://en.wikipedia.org/wiki/CESIL); ICL CES reference card.

## Integers

| Topic | Rule | Status |
|-------|------|--------|
| Domain | Integers only (no floating-point type) | **specified** |
| Historical range | Classic materials and cesil.org cite **24-bit signed** integers: −8 388 608 … +8 388 607 | **specified** (historical claim; not enforced) |
| Enforcement | This dialect uses host `int` arithmetic without 24-bit clamp or overflow trap | **specified** (Milestone 6 Stage 3) |
## Namespaces

Labels (code line names) and store names share the same identifier spelling rules
(see [Source form](source-form.md)). A name used as a jump target refers to a labelled
instruction; a name used with `LOAD` / `STORE` / arithmetic refers to a store
location. Confusing undefined *labels* with undefined *variables* is a common error —
see [Open questions](open-questions.md).

## Execution outline

1. Start with accumulator and store state as defined under [Runtime](runtime.md).
2. Execute instructions in order unless a jump redirects control.
3. `IN` consumes the next data value; exhausting data is a runtime error condition.
4. Stop on `HALT`, on falling off the end of the instruction list, or on a runtime error.
