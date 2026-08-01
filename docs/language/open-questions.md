← [Language reference index](README.md)

# Open questions and conflicts

Items below must **not** be closed by copying current `cesil-core` behaviour. Resolve
them in [Milestone 6](../milestones/06-language-parity.md) by probing Visual CESIL
(and re-checking classic sources), then update this table and the relevant reference
pages.

## Probe checklist

| ID | Topic | Conflict / uncertainty | Suggested probe | Status |
|----|--------|------------------------|-----------------|--------|
| Q1 | Undefined variables | Classic teaching often reads never-stored names as `0`; Jacobs rejects undefined *labels* at compile time. Distinct issues. | `LOAD UNSET` / `OUT` with no `STORE`; `LOAD +0` then `ADD UNSET` | **open** |
| Q2 | Identifier case | Whether `STORE Foo` / `LOAD FOO` alias | Case variants on stores and on jump labels | **open** |
| Q3 | Integer range / overflow | Classic 24-bit vs host-width arithmetic | Values near ±2²³; multiply overflow | **open** |
| Q4 | Signed vs unsigned constants | Classic prefers `+0`; Visual CESIL examples use `0`, `8`, etc. | Load/add unsigned literals from Example programs | **open** (lean accept for corpus) |
| Q5 | Comment syntax | Classic `(` vs Visual CESIL `*` comment lines | Open Example 1 / Division with leading `*` lines | **open** (needed for corpus) |
| Q6 | Trailing `*` after data | Classic requires end marker; many Jacobs examples omit it | Run Example 1 / Example 2 without final `*` | **open** |
| Q7 | `DIVIDE` rounding | Classic toward-zero-style wording vs Jacobs | Negative dividend / positive divisor cases | **open** |
| Q8 | `OUT` / `PRINT` / `LINE` bytes | Spacing and newlines for golden tests | Capture Jacobs output for Example 1–2 | **open** |
| Q9 | Fall off end without `HALT` | Success vs error | Program that ends without `HALT` | **open** |
| Q10 | Runtime banner text | Exact `*** … ***` strings | Capture Jacobs stdout on `IN` exhaustion and divide-by-zero | **open** |
| Q11 | Label length > 6 | Classic max 6; Jacobs probe used longer names in some tools | `JUMP NOWHERE` style names | **open** (this engine currently max 6) |

## Settled divergences (not open)

These are intentional and already documented:

| Topic | Status |
|-------|--------|
| Multi-error recovery beyond Jacobs fail-fast | **deliberate diverge** / **specified** (M4) |
| Trailing junk after zero-operand mnemonics diagnosed | **deliberate diverge** |
| Bare `STORE` and some operand shapes diagnosed | **deliberate diverge** |
| Own diagnostic message text | **specified** |
| CESIL “Plus” excluded from 1.0 | **specified** |

## Source disagreements (summary)

| Topic | Classic (ICL / Wikipedia / cesil.org) | Visual CESIL | This project today (note only) |
|-------|----------------------------------------|--------------|--------------------------------|
| Comments | `(` line comments | `*` comment lines in examples | Recognises `(`; `*` is end-of-data token |
| Constants | Often require sign | Unsigned allowed | Optional `+`/`-`; unsigned digit forms parse |
| Data terminator | `*` | Often omitted | Expects `*` in fixtures |
| Integer width | 24-bit cited | Unclear from available notes | Host `int` parse limits |
| Variable init | Not always explicit | Zero at start (2.0 notes) | Accumulator 0; empty store map reads as 0 |

“This project today” is **not** the specification for **open** rows.
