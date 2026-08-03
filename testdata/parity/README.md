# Language-parity probe fixtures (Milestone 6)

Curated CESIL programs for Visual CESIL 2.0 parity work: Stage 1 corpus gate,
Stage 2 parser locks, and Stage 3 runtime probes.

See [Milestone 6](../../docs/milestones/06-language-parity.md),
[`docs/language/open-questions.md`](../../docs/language/open-questions.md),
[`PROBE.md`](PROBE.md), and [`MATRIX.md`](MATRIX.md).

**Do not treat current `cesil-core` behaviour as the oracle** for unsettled edges.
Prefer Visual CESIL live Check/Run (or classic sources where cited).

## Layout

Programs use conventional CESIL columns (label / instruction / operand) with spaces,
matching `examples/total.ces`. Identifiers stay within six characters except
`runtime-long-label.ces` (Q11).

## Windows probe

1. Prefer **Visual CESIL 2.0** on Windows (local distribution; not vendored — see
   [`docs/language/compatibility-corpus.md`](../../docs/language/compatibility-corpus.md)).
2. Check and/or Run fixtures; record exact output for runtime probes.
3. Fill [`PROBE.md`](PROBE.md); update [`MATRIX.md`](MATRIX.md) and `docs/language/`.

Gate fixtures and early stubs were live-checked 2026-08-03. Runtime fixtures below
are for Stage 3 (re-probe on Windows when available).

## Fixture index

### Gate fixtures (Stage 1)

| File | Q | Intent |
|------|---|--------|
| `gate-unsigned-constant.ces` | Q4 | Unsigned literals |
| `gate-star-comment.ces` | Q5 | `*` full-line comments |
| `gate-paren-comment.ces` | Q5 | Classic `(` comment (Jacobs rejects) |
| `gate-data-no-star.ces` | Q6 | Data ends at EOF |
| `gate-data-with-star.ces` | Q6 | Classic trailing `*` |
| `smoke-print-out.ces` | Q8 | `PRINT`/`OUT` adjacency (`Hi1`) |

### Early stubs

| File | Q | Intent |
|------|---|--------|
| `probe-unset-var.ces` | Q1 | Never-stored `LOAD` → `0` |
| `probe-case-fold.ces` | Q2 | Case-sensitive stores |

### Runtime probes (Stage 3)

| File | Q | Intent |
|------|---|--------|
| `runtime-divide-neg.ces` | Q7 | `-7 / 2` toward-zero vs Jacobs |
| `runtime-outdigits.ces` | Q8 | Multi-digit / negative `OUT` after `PRINT` |
| `runtime-no-halt.ces` | Q9 | Fall off end without `HALT` |
| `runtime-in-exhaust.ces` | Q10 | `IN` with no data — banner text |
| `runtime-divzero.ces` | Q10 | `DIVIDE 0` — banner text |
| `runtime-overflow.ces` | Q3 | `8388607 * 2` near/above 24-bit max |
| `runtime-long-label.ces` | Q11 | Jump to 7-char label `NOWHERE` |
