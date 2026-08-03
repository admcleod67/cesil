# Compatibility matrix (Milestone 6 Stages 1–3)

Evidence: [`PROBE.md`](PROBE.md). Engine locks: `SourceCompatibilityTest`,
`RuntimeSemanticsTest`.

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
| Host-width arithmetic / no 24-bit trap | Q3 | `runtime-overflow.ces` | **specified** | **match** (live Jacobs overflow Run deferred) |
| `DIVIDE` toward zero | Q7 | `runtime-divide-neg.ces` | **specified** | **match** |
| `PRINT`/`OUT` formatting | Q8 | `smoke-print-out.ces`, `runtime-outdigits.ces` | **specified** | **match** |
| Fall off end without `HALT` | Q9 | `runtime-no-halt.ces` | **specified** | **match** |
| Runtime banners | Q10 | `runtime-in-exhaust.ces`, `runtime-divzero.ces` | **specified** | **match** |
| Identifier max length 6 | Q11 | `runtime-long-label.ces` | **specified** (classic; Jacobs may be laxer) | **match** |

Broader golden corpus rows land in Stage 4.
