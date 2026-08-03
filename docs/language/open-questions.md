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
| Q1 | Undefined variables | Classic teaching often reads never-stored names as `0`; Jacobs rejects undefined *labels* at compile time. Distinct issues. | `testdata/parity/probe-unset-var.ces` (+ case-fold Run) | **Jacobs-observed** — compile OK; unset name reads as `0` at runtime (Stage 3; promote to **specified** when wiring Stage 3 tests) |
| Q2 | Identifier case | Whether `STORE Foo` / `LOAD FOO` alias | `testdata/parity/probe-case-fold.ces` | **specified** — case-sensitive store names (live Run: `LOAD +42` / `STORE Foo` / `LOAD FOO` / `OUT` → `0`) |
| Q3 | Integer range / overflow | Classic 24-bit vs host-width arithmetic | Values near ±2²³; multiply overflow | **open** (Stage 3) |
| Q4 | Signed vs unsigned constants | Classic prefers `+0`; Visual CESIL examples use `0`, `8`, etc. | `gate-unsigned-constant.ces` + Example 1 | **specified** — accept unsigned (Stage 1; Jacobs live Check: no error) |
| Q5 | Comment syntax | Classic `(` vs Visual CESIL `*` comment lines | `gate-star-comment.ces` / `gate-paren-comment.ces` | **specified** — accept `*` in code (Jacobs live Check); keep `(` as **deliberate diverge** (Jacobs rejects with ~unrecognised instruction on L1); data `*` remains end marker |
| Q6 | Trailing `*` after data | Classic requires end marker; many Jacobs examples omit it | `gate-data-no-star.ces` / `gate-data-with-star.ces` | **specified** — accept EOF or trailing `*` (Stage 1; both live Check: no error) |
| Q7 | `DIVIDE` rounding | Classic toward-zero-style wording vs Jacobs | Negative dividend / positive divisor cases | **open** (Stage 3) |
| Q8 | `OUT` / `PRINT` / `LINE` bytes | Spacing and newlines for golden tests | `smoke-print-out.ces`; Example 1–2 stdout | **Jacobs-observed (partial)** — live Run: `Hi1` (no space between `PRINT` and `OUT`); multi-digit / padding / exact newlines still **open** (Stage 3/4) |
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
| Classic `(` line comments (Jacobs rejects; uses `*` instead) | **deliberate diverge** |
| Own diagnostic message text | **specified** |
| CESIL “Plus” excluded from 1.0 | **specified** |

## Source disagreements (summary)

| Topic | Classic (ICL / Wikipedia / cesil.org) | Visual CESIL | This dialect (after Stage 1) |
|-------|----------------------------------------|--------------|------------------------------|
| Comments | `(` line comments | `*` comment lines; live Check rejects `(` | **`*`** required for Jacobs; **`(`** kept as **deliberate diverge** |
| Constants | Often require sign | Unsigned allowed (live Check) | **Both** signed and unsigned (**specified**) |
| Data terminator | `*` | Often omitted (EOF); trailing `*` also accepted (live Check) | **Either** `*` or EOF (**specified**) |
| Integer width | 24-bit cited | Unclear from available notes | Still **open** (Q3) |
| Variable init / unset reads | Often treated as `0` | Compile OK; unset reads as `0` (live) | **Jacobs-observed** (Q1) |
| Identifier case (stores) | Often unclear / folded in teaching materials | Case-sensitive (`Foo` ≠ `FOO`) | **specified** (Q2) |

Engine implementation of Q4–Q6 is Stage 2; “this dialect” above is the specification,
not a claim that `cesil-core` already matches.
