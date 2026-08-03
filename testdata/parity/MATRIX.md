# Compatibility matrix (Milestone 6 Stages 1–3)

Evidence: [`PROBE.md`](PROBE.md) (including live Visual CESIL runtime Run 2026-08-03).
Engine locks: `SourceCompatibilityTest`, `RuntimeSemanticsTest` (may lag Jacobs where
**gap** is noted).

## Source gate (Stages 1–2)

| Topic | Q | Fixture(s) | Rule status | Engine vs rule |
|-------|---|------------|-------------|----------------|
| Unsigned numeric constants | Q4 | `gate-unsigned-constant.ces` | **specified** | **match** |
| `*` full-line comments | Q5 | `gate-star-comment.ces` | **specified** | **match** |
| `(` line comments | Q5 | `gate-paren-comment.ces` | **deliberate diverge** | **match** (kept) |
| Data ends at EOF | Q6 | `gate-data-no-star.ces` | **specified** | **match** |
| Data ends with `*` | Q6 | `gate-data-with-star.ces` | **specified** | **match** |

## Runtime (Stage 3)

| Topic | Q | Fixture(s) | Rule status | Engine vs rule |
|-------|---|------------|-------------|----------------|
| Never-stored variable | Q1 | `probe-unset-var.ces` | **specified** | **match** |
| Store-name case | Q2 | `probe-case-fold.ces` | **specified** | **match** |
| Host-width arithmetic / no 24-bit trap | Q3 | `runtime-overflow.ces` | **specified** (live: `16777214`) | **match** |
| `DIVIDE` toward zero | Q7 | `runtime-divide-neg.ces` | **specified** (live: `-3`) | **match** |
| `PRINT`/`OUT` formatting | Q8 | `smoke-print-out.ces`, `runtime-outdigits.ces` | **specified** (live: `n=42 m=-3`) | **match** |
| Require `HALT` at end | Q9 | `runtime-no-halt.ces` | **specified** (Jacobs error if missing) | **gap** (engine allows fall-off success) |
| Runtime banners | Q10 | `runtime-in-exhaust.ces`, `runtime-divzero.ces` | **specified** (Jacobs `** ERROR: …` text) | **gap** (engine uses classic `*** … ***`) |
| Identifier max length 6 | Q11 | `runtime-long-label.ces` | **deliberate diverge** (classic max 6; Jacobs accepts longer) | **match** (max 6) |

Broader golden corpus rows land in Stage 4 (including closing Q9/Q10 gaps).
