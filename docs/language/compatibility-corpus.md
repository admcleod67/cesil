← [Language reference index](README.md)

# Compatibility corpus (Visual CESIL)

Online Visual CESIL documentation is not always reachable. For Milestone 5–6 work,
use a local Visual CESIL 2.0 distribution when available.

## Local Visual CESIL 2.0 tree

Keep a Visual CESIL 2.0 distribution **outside** this repository (third-party /
copyrighted material). Point Milestone 6 golden work at that local copy or another
agreed corpus path — do not commit absolute host paths here.

Typical contents used when drafting this reference:

| File | Role (from inspection) |
|------|-------------------------|
| `Example 1.ces` | Total-with-print loop; `*` comment lines; `%` data; unsigned `LOAD 0`; no trailing `*` |
| `Example 2.ces` | GCD; `*` comments; unsigned data; no trailing `*` |
| `Example 3.ces` … `Example 5.ces` | Further Jacobs examples |
| `Division.ces` | Non-integer division demo; lowercase store names; `*` comments |
| `Easter.ces` | Calendar example (S. Robinson) |
| `99Beers.ces` | Longer output example |
| `Release Notes.htm` | Product notes through 2.0 (variable init to zero; `+` constants; `IN` past end errors; blank lines ignored) |
| `Visual CESIL.exe` | Windows IDE binary |

## In-repo probe fixtures (Milestone 6 Stage 1)

Gate probes for Q4–Q6 (unsigned constants, `*` / `(` comments, data terminator) live
under [`testdata/parity/`](../../testdata/parity/). See that directory’s
[`README.md`](../../testdata/parity/README.md), [`PROBE.md`](../../testdata/parity/PROBE.md),
and [`MATRIX.md`](../../testdata/parity/MATRIX.md). Do not vendor Jacobs sources into
the repo; use an external Visual CESIL 2.0 tree for his examples.

## Implications for the dialect

From Stage 1 probe evidence (shipped examples + release notes; see `PROBE.md`):

- `*` full-line comments in the **code** section — **specified** (corpus); must not
  break classic end-of-data `*`
- Unsigned numeric literals in code and data — **specified** (corpus)
- Data sections that end at EOF without a final `*` line — **specified** (corpus);
  classic trailing `*` remains valid
- Variables initialised to zero at run start (2.0) — still **Jacobs-observed** (Stage 3)
- Optional `+` on positive constants; `-0` disallowed (1.2 notes) — **Jacobs-observed** /
  keep accepting `+`; `-0` still **open** / **Jacobs-observed**

## In-repo examples

This repository’s own samples remain the day-to-day demos:

- [`examples/total.ces`](../../examples/total.ces) — Wikipedia-style total
- [`examples/hello.ces`](../../examples/hello.ces) — minimal `PRINT` / `LINE` / `HALT`

They use classic-friendly signed `+0` and an explicit trailing `*`.
