← [Language reference index](README.md)

# Instructions

Classic CESIL has **fourteen** instructions. Operand shapes below follow the ICL /
Wikipedia / cesil.org classic picture, with Visual CESIL notes where they matter.

Mnemonics are shown in upper case; recognition is case-insensitive (**specified**).

## Memory and accumulator

| Mnemonic | Operand | Effect | Status |
|----------|---------|--------|--------|
| `LOAD` | Store name or integer constant | Copy operand value into the accumulator | **specified** |
| `STORE` | Store name | Copy accumulator into that store | **specified** |
| `IN` | None | Read next data-section integer into the accumulator | **specified** |

## Arithmetic

Results replace the accumulator.

| Mnemonic | Operand | Effect | Status |
|----------|---------|--------|--------|
| `ADD` | Store or constant | Accumulator ← accumulator + operand | **specified** |
| `SUBTRACT` | Store or constant | Accumulator ← accumulator − operand | **specified** |
| `MULTIPLY` | Store or constant | Accumulator ← accumulator × operand | **specified** |
| `DIVIDE` | Store or constant | Accumulator ← accumulator ÷ operand (integer) | **specified** |

`DIVIDE` by zero is a runtime error. Visual CESIL banner:
`** ERROR: Attempted division by zero`. **specified** (see [Runtime](runtime.md))

Integer division truncates toward zero (e.g. `-7 / 2` → `-3`). **specified**
(Jacobs live confirmed)

## Control

| Mnemonic | Operand | Effect | Status |
|----------|---------|--------|--------|
| `JUMP` | Label | Unconditional branch to that label | **specified** |
| `JIZERO` | Label | Branch if accumulator is zero | **specified** |
| `JINEG` | Label | Branch if accumulator is negative | **specified** |
| `HALT` | None | Terminate the program | **specified** |

Jump operands must be labels, not numeric constants. **specified**  
Missing jump targets are compile-time errors. **specified** (Jacobs and this project)

## Output

| Mnemonic | Operand | Effect | Status |
|----------|---------|--------|--------|
| `PRINT` | Quoted string | Write the string with no automatic newline | **specified** |
| `OUT` | None | Write the accumulator as decimal text with no automatic newline | **specified** |
| `LINE` | None | Write a newline (start a new output line) | **specified** |

Exact spacing around `OUT` values and host newline conventions (`\n` vs `\r\n`) are
**open** for byte-identical golden tests (normalise line endings where required).
Adjacent `PRINT` / `OUT` with no separator is **Jacobs-observed**
(`smoke-print-out.ces` live Run → `Hi1`).

## Operand shape summary

| Instruction group | Allowed operands |
|-------------------|------------------|
| `LOAD`, `ADD`, `SUBTRACT`, `MULTIPLY`, `DIVIDE` | Identifier (store) or integer constant |
| `STORE` | Identifier (store name) only |
| `JUMP`, `JIZERO`, `JINEG` | Identifier (label) only |
| `PRINT` | Quoted string only |
| `IN`, `OUT`, `LINE`, `HALT` | None |

Wrong shapes are compile-time errors in this project; Jacobs is laxer on some shapes
(**deliberate diverge** — see [Diagnostics](diagnostics.md)).
