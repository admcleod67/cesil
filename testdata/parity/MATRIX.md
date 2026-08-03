# Compatibility matrix stub (Milestone 6 Stage 1)

Gate topics only. Status values: **match** (this engine already agrees with settled
rule), **gap** (engine must change in Stage 2), **Jacobs-observed** / **specified**
(see language reference), **deliberate diverge**, **not yet established**.

Evidence: [`PROBE.md`](PROBE.md) (Visual CESIL 2.0 live Check + distribution examples /
release notes).

| Topic | Q | Fixture(s) | Rule status | Engine vs rule | Stage 2 action |
|-------|---|------------|-------------|----------------|----------------|
| Unsigned numeric constants | Q4 | `gate-unsigned-constant.ces` | **specified** (accept; Jacobs live Check) | **gap** if engine rejects unsigned forms; else **match** | Ensure unsigned literals parse in code and data |
| `*` full-line comments | Q5 | `gate-star-comment.ces` | **specified** (accept in code; Jacobs live Check) | **gap** (today `*` is end-of-data token only) | Recognise start-of-line `*` comments without breaking data `*` |
| `(` line comments | Q5 | `gate-paren-comment.ces` | **deliberate diverge** (classic keep; Jacobs **rejects**) | **match** if `(` already skipped | Preserve `(`; do not require Jacobs acceptance |
| Data ends at EOF (no `*`) | Q6 | `gate-data-no-star.ces` | **specified** (accept; Jacobs live Check) | **gap** if engine requires trailing `*` | Allow EOF to end data section |
| Data ends with `*` | Q6 | `gate-data-with-star.ces` | **specified** (accept; Jacobs live Check) | **match** if already accepted | Keep working |

Broader matrix rows (instructions, runtime edges) land in later Milestone 6 stages.

## Early Stage 3 observations (not Stage 1 gate)

| Topic | Q | Fixture(s) | Rule status | Notes |
|-------|---|------------|-------------|-------|
| Never-stored variable (`LOAD`) | Q1 | `probe-unset-var.ces` | **Jacobs-observed** | Live Check: no error. Unset read as `0` confirmed via case-fold Run (`LOAD FOO` after `STORE Foo` with 42 → `0`) |
| Store-name case | Q2 | `probe-case-fold.ces` | **specified** (case-sensitive) | Live Run: `LOAD +42` / `STORE Foo` / `LOAD FOO` / `OUT` → `0`. Engine should preserve identifier case (no fold) |
| `PRINT`/`OUT` adjacency | Q8 | `smoke-print-out.ces` | **Jacobs-observed (partial)** | Live Run: `Hi1` — no auto space between string and digit |
