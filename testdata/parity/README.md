# Language-parity probe fixtures (Milestone 6 Stage 1)

Curated CESIL programs for the **corpus gate**: what Visual CESIL 2.0 accepts for
source forms that Jacobs examples rely on. Results feed Stage 2 parser work.

See [Milestone 6](../../docs/milestones/06-language-parity.md),
[`docs/language/open-questions.md`](../../docs/language/open-questions.md), and the
probe log in [`PROBE.md`](PROBE.md). Compatibility matrix stub: [`MATRIX.md`](MATRIX.md).

**Do not treat current `cesil-core` behaviour as the oracle.** Probe Visual CESIL
(or inspect its shipped examples / release notes) before promoting language-reference
statuses.

## Layout

Programs use conventional CESIL columns (label / instruction / operand) with spaces,
matching `examples/total.ces`. Identifiers stay within six characters.

## Windows probe

**Status:** gate fixtures checked live on Visual CESIL (2026-08-03). Stage 3 stubs and
`smoke-print-out.ces` also probed the same day (see [`PROBE.md`](PROBE.md)). Matrix stub:
[`MATRIX.md`](MATRIX.md).

Original procedure (kept for re-probes):

1. Prefer **Visual CESIL 2.0** on Windows (local distribution; not vendored here — see
   [`docs/language/compatibility-corpus.md`](../../docs/language/compatibility-corpus.md)).
2. Check and/or Run each gate fixture below.
3. Also open external `Example 1.ces` / `Example 2.ces` from that distribution.
4. Fill [`PROBE.md`](PROBE.md), then update [`MATRIX.md`](MATRIX.md) and
   `docs/language/` for Q4–Q6.

## Fixture index

### Gate fixtures (required for Stage 1)

| File | Open Q | Intent |
|------|--------|--------|
| `gate-unsigned-constant.ces` | Q4 | Unsigned literals (`LOAD 0`, `ADD 8`) |
| `gate-star-comment.ces` | Q5 | `*` full-line comments (Jacobs style) |
| `gate-paren-comment.ces` | Q5 | Classic `(` comment line (Jacobs rejects; deliberate diverge control) |
| `gate-data-no-star.ces` | Q6 | Data section ending at EOF (no trailing `*`) |
| `gate-data-with-star.ces` | Q6 | Classic trailing `*` (control) |
| `smoke-print-out.ces` | Q8 seed | Tiny `PRINT` / `OUT` / `LINE` (live Run: `Hi1`) |

### Stage 3-oriented stubs (optional in Stage 1)

| File | Open Q | Intent |
|------|--------|--------|
| `probe-unset-var.ces` | Q1 | `LOAD` of a never-stored name (compile OK; unset reads as `0`) |
| `probe-case-fold.ces` | Q2 | `LOAD +42` / `STORE Foo` / `LOAD FOO` / `OUT` → `0` (case-sensitive stores) |

These stubs do **not** block Stage 1 close-out.
