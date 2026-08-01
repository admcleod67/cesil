← [Language reference index](README.md)

# Runtime

## Start state

| Item | Rule | Status |
|------|------|--------|
| Accumulator | Starts at zero | **specified** (common classic practice; confirm vs card if needed) |
| Store locations | Visual CESIL 2.0 release notes: variables initialised to zero at start of execution | **Jacobs-observed** |
| Never-stored name read via `LOAD` / arithmetic | Often treated as zero in classic CESIL teaching | **open** (do not invent a compile-time “undefined variable” error until probed) |
| Program counter | First instruction | **specified** |
| Data pointer | First data value | **specified** |

## Control flow

- Sequential execution unless `JUMP` / `JIZERO` / `JINEG` redirects. **specified**
- `JIZERO` tests accumulator `== 0`; `JINEG` tests accumulator `< 0`. **specified**
- Falling off the last instruction ends the run successfully (no error). **specified**
  (practical dialect; confirm Jacobs if needed — **open** only if corpus shows otherwise)
- `HALT` ends successfully. **specified**

## Arithmetic and range

- Operations update the accumulator as in [Instructions](instructions.md). **specified**
- Overflow / 24-bit wrap vs host-width arithmetic: **open**
- `DIVIDE` by zero: runtime error; classic banner `*** DIVISION BY ZERO ***`.
  **specified** (condition)

## Output

- `PRINT` writes its string; `OUT` writes the accumulator; `LINE` writes a newline.
  **specified**
- No automatic spaces between adjacent `PRINT` / `OUT` fragments unless present in the
  string. **specified** (classic I/O model)
- Exact digit formatting (leading spaces, width) for `OUT`: **open**

## Runtime vs compile-time

Programs with any compilation error must not execute. **specified**
([Milestone 4](../milestones/04-diagnostic-accumulation.md))

Runtime failures are fail-fast for this project (stop on first runtime error).
**specified** (current product rule; not expanded in Milestone 4)
