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
- Falling off the last instruction ends the run successfully (no error). **specified**
- `HALT` ends successfully. **specified**

## Arithmetic and range

- Operations update the accumulator as in [Instructions](instructions.md). **specified**
- Integer division truncates toward zero (e.g. `-7 / 2` → `-3`). **specified**
- Values use host `int` width; this dialect does **not** trap or clamp to the
  historical 24-bit CESIL range. **specified** (Stage 3; live Jacobs overflow
  behaviour may be re-checked in Stage 4)
- `DIVIDE` by zero: runtime error; banner `*** DIVISION BY ZERO ***`. **specified**

## Output

- `PRINT` writes its string; `OUT` writes the accumulator as a bare decimal (no
  leading spaces/padding); `LINE` writes a newline. **specified**
- No automatic spaces between adjacent `PRINT` / `OUT` fragments. **specified**
  (`smoke-print-out.ces` → `Hi1`; `runtime-outdigits.ces` → `n=42 m=-3`)

## Runtime vs compile-time

Programs with any compilation error must not execute. **specified**
([Milestone 4](../milestones/04-diagnostic-accumulation.md))

Runtime failures are fail-fast (stop on first runtime error). **specified**

`IN` past end of data: banner `*** PROGRAM REQUIRES MORE DATA ***`. **specified**
