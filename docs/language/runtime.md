← [Language reference index](README.md)

# Runtime

## Start state

| Item | Rule | Status |
|------|------|--------|
| Accumulator | Starts at zero | **specified** |
| Store locations | Empty at start; Visual CESIL 2.0 also initialises variables to zero | **specified** / **Jacobs-observed** |
| Never-stored name read via `LOAD` / arithmetic | Not a compile error; reads as zero | **specified** |
| Program counter | First instruction | **specified** |
| Data pointer | First data value | **specified** |

## Control flow

- Sequential execution unless `JUMP` / `JIZERO` / `JINEG` redirects. **specified**
- `JIZERO` tests accumulator `== 0`; `JINEG` tests accumulator `< 0`. **specified**
- A program must end with `HALT`. Falling off the last instruction without `HALT` is an
  error (Visual CESIL live: `** ERROR: No HALT at end of program`). **specified**
  (engine **gap** — currently treats fall-off as success; see
  [`testdata/parity/PROBE.md`](../../testdata/parity/PROBE.md))
- `HALT` ends successfully. **specified**

## Arithmetic and range

- Operations update the accumulator as in [Instructions](instructions.md). **specified**
- Integer division truncates toward zero (e.g. `-7 / 2` → `-3`). **specified**
  (Jacobs live confirmed)
- Values use host `int` width; this dialect does **not** trap or clamp to the
  historical 24-bit CESIL range. **specified** (Jacobs live: `8388607×2` → `16777214`)
- `DIVIDE` by zero: runtime error. Visual CESIL banner:
  `** ERROR: Attempted division by zero`. **specified** (engine still emits classic
  `*** DIVISION BY ZERO ***` → **gap**)

## Output

- `PRINT` writes its string; `OUT` writes the accumulator as a bare decimal (no
  leading spaces/padding); `LINE` writes a newline. **specified**
- No automatic spaces between adjacent `PRINT` / `OUT` fragments. **specified**
  (`smoke-print-out.ces` → `Hi1`; `runtime-outdigits.ces` → `n=42 m=-3`)

## Runtime vs compile-time

Programs with any compilation error must not execute. **specified**
([Milestone 4](../milestones/04-diagnostic-accumulation.md))

Runtime failures are fail-fast (stop on first runtime error). **specified**

`IN` past end of data: Visual CESIL banner
`** ERROR: Attempt to read more data than was provided`. **specified**
(engine still emits classic `*** PROGRAM REQUIRES MORE DATA ***` → **gap**)
