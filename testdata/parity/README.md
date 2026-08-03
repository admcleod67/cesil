# Language-parity probe fixtures (Milestone 6)

Curated CESIL programs for Visual CESIL 2.0 parity work: Stage 1 corpus gate,
Stage 2 parser locks, Stage 3 runtime probes, and Stage 4 golden outputs.

See [Milestone 6](../../docs/milestones/06-language-parity.md),
[`docs/language/open-questions.md`](../../docs/language/open-questions.md),
[`PROBE.md`](PROBE.md), and [`MATRIX.md`](MATRIX.md).

Prefer Visual CESIL live Check/Run (or classic sources where cited) when extending
the corpus. Settled Stage 1–4 rules are locked by Catch2 tests.

## Layout

Programs use conventional CESIL columns (label / instruction / operand) with spaces,
matching `examples/total.ces`. Identifiers stay within six characters except
`runtime-long-label.ces` (Q11).

## Windows probe

1. Prefer **Visual CESIL 2.0** on Windows (local distribution; not vendored — see
   [`docs/language/compatibility-corpus.md`](../../docs/language/compatibility-corpus.md)).
2. Check and/or Run fixtures; record exact output for runtime probes.
3. Fill [`PROBE.md`](PROBE.md); update [`MATRIX.md`](MATRIX.md) and `docs/language/`.

Gate fixtures, stubs, and **runtime** fixtures were live-checked/run on Visual CESIL
2026-08-03. Results are in [`PROBE.md`](PROBE.md). Q9/Q10 engine gaps closed in Stage 4.

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

| File | Q | Intent / Jacobs live |
|------|---|----------------------|
| `runtime-divide-neg.ces` | Q7 | `-7 / 2` → `-3` (toward zero) |
| `runtime-outdigits.ces` | Q8 | `n=42 m=-3` |
| `runtime-no-halt.ces` | Q9 | Jacobs: `** ERROR: No HALT at end of program` |
| `runtime-in-exhaust.ces` | Q10 | Jacobs: `** ERROR: Attempt to read more data than was provided` |
| `runtime-divzero.ces` | Q10 | Jacobs: `** ERROR: Attempted division by zero` |
| `runtime-overflow.ces` | Q3 | `8388607 * 2` → `16777214` |
| `runtime-long-label.ces` | Q11 | `JUMP NOWHERE` → undefined label (Jacobs allows 7-char name) |

### Golden corpus (Stage 4)

Project-owned programs and expected stdout under [`golden/`](golden/) (not Jacobs
source trees). Covered by `GoldenCorpusTest`.
