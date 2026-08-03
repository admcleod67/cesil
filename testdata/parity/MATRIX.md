# Compatibility matrix (Milestone 6 Stages 1–4)

Evidence: [`PROBE.md`](PROBE.md) (including live Visual CESIL runtime Run 2026-08-03).
Engine locks: `SourceCompatibilityTest`, `RuntimeSemanticsTest`, `GoldenCorpusTest`.

## Source gate (Stages 1–2)

| Topic | Q | Fixture(s) | Rule status | Engine vs rule |
|-------|---|------------|-------------|----------------|
| Unsigned numeric constants | Q4 | `gate-unsigned-constant.ces` | **specified** | **match** |
| `*` full-line comments | Q5 | `gate-star-comment.ces` | **specified** | **match** |
| `(` line comments | Q5 | `gate-paren-comment.ces` | **deliberate diverge** | **match** (kept) |
| Data ends at EOF | Q6 | `gate-data-no-star.ces` | **specified** | **match** |
| Data ends with `*` | Q6 | `gate-data-with-star.ces` | **specified** | **match** |

## Runtime (Stages 3–4)

| Topic | Q | Fixture(s) | Rule status | Engine vs rule |
|-------|---|------------|-------------|----------------|
| Never-stored variable | Q1 | `probe-unset-var.ces` | **specified** | **match** |
| Store-name case | Q2 | `probe-case-fold.ces` | **specified** | **match** |
| Host-width arithmetic / no 24-bit trap | Q3 | `runtime-overflow.ces` | **specified** (live: `16777214`) | **match** |
| `DIVIDE` toward zero | Q7 | `runtime-divide-neg.ces` | **specified** (live: `-3`) | **match** |
| `PRINT`/`OUT` formatting | Q8 | `smoke-print-out.ces`, `runtime-outdigits.ces`, `golden/print-out.*` | **specified** (live: `n=42 m=-3`) | **match** |
| Require `HALT` at end | Q9 | `runtime-no-halt.ces`, `golden/error-no-halt.out` | **specified** | **match** |
| Runtime banners | Q10 | `runtime-in-exhaust.ces`, `runtime-divzero.ces`, `golden/error-*.out` | **specified** | **match** |
| Identifier max length 6 | Q11 | `runtime-long-label.ces` | **deliberate diverge** (classic max 6; Jacobs accepts longer) | **match** (max 6) |

## Golden corpus (Stage 4)

Project-owned fixtures under [`golden/`](golden/) (no copyrighted Jacobs `.ces` trees):

| Fixture | Role | Opcodes exercised |
|---------|------|-------------------|
| `hello.ces` | Minimal `PRINT` / `LINE` / `HALT` | PRINT, LINE, HALT |
| `total.ces` | Wikipedia/Example-1-style total-until-negative | LOAD, STORE, IN, ADD, JUMP, JINEG, PRINT, OUT, LINE, HALT |
| `print-out.ces` | Adjacency / multi-digit / negative | PRINT, LOAD, OUT, LINE, HALT |
| `arith-jizero.ces` | Remaining arithmetic + `JIZERO` | LOAD, SUBTRACT, MULTIPLY, STORE, JIZERO, OUT, LINE, HALT |
| Error goldens | Q9/Q10 banners via parity runtimes | IN, DIVIDE (+ fall-off without HALT) |

All fourteen classic opcodes appear in at least one automated golden or parity runtime test.
