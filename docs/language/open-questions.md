← [Language reference index](README.md)

# Open questions and conflicts

Items below must **not** be closed by copying current `cesil-core` behaviour. Resolve
remaining **open** rows in [Milestone 6](../milestones/06-language-parity.md) by
probing Visual CESIL (and re-checking classic sources), then update this table and
the relevant reference pages.

Stage 1 corpus-gate evidence:
[`testdata/parity/PROBE.md`](../../testdata/parity/PROBE.md).

## Probe checklist

| ID | Topic | Conflict / uncertainty | Suggested probe | Status |
|----|--------|------------------------|-----------------|--------|
| Q1 | Undefined variables | Classic teaching often reads never-stored names as `0`; Jacobs rejects undefined *labels* at compile time. Distinct issues. | `testdata/parity/probe-unset-var.ces` | **open** (Stage 3) |
| Q2 | Identifier case | Whether `STORE Foo` / `LOAD FOO` alias | `testdata/parity/probe-case-fold.ces` | **open** (Stage 3) |
| Q3 | Integer range / overflow | Classic 24-bit vs host-width arithmetic | Values near ±2²³; multiply overflow | **open** (Stage 3) |
| Q4 | Signed vs unsigned constants | Classic prefers `+0`; Visual CESIL examples use `0`, `8`, etc. | `gate-unsigned-constant.ces` + Example 1 | **specified** — accept unsigned (Stage 1) |
| Q5 | Comment syntax | Classic `(` vs Visual CESIL `*` comment lines | `gate-star-comment.ces` / `gate-paren-comment.ces` | **specified** — accept both in code (Stage 1); data `*` remains end marker |
| Q6 | Trailing `*` after data | Classic requires end marker; many Jacobs examples omit it | `gate-data-no-star.ces` / `gate-data-with-star.ces` | **specified** — accept EOF or trailing `*` (Stage 1) |
| Q7 | `DIVIDE` rounding | Classic toward-zero-style wording vs Jacobs | Negative dividend / positive divisor cases | **open** (Stage 3) |
| Q8 | `OUT` / `PRINT` / `LINE` bytes | Spacing and newlines for golden tests | `smoke-print-out.ces`; Example 1–2 stdout | **open** (Stage 3/4) |
| Q9 | Fall off end without `HALT` | Success vs error | Program that ends without `HALT` | **open** (Stage 3) |
| Q10 | Runtime banner text | Exact `*** … ***` strings | Capture Jacobs stdout on `IN` exhaustion and divide-by-zero | **open** (Stage 3) |
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

| Topic | Classic (ICL / Wikipedia / cesil.org) | Visual CESIL | This dialect (after Stage 1) |
|-------|----------------------------------------|--------------|------------------------------|
| Comments | `(` line comments | `*` comment lines in examples | **Both** in code section (**specified**) |
| Constants | Often require sign | Unsigned allowed | **Both** signed and unsigned (**specified**) |
| Data terminator | `*` | Often omitted (EOF) | **Either** `*` or EOF (**specified**) |
| Integer width | 24-bit cited | Unclear from available notes | Still **open** (Q3) |
| Variable init | Not always explicit | Zero at start (2.0 notes) | Still **Jacobs-observed** / **open** (Q1) |

Engine implementation of Q4–Q6 is Stage 2; “this dialect” above is the specification,
not a claim that `cesil-core` already matches.
