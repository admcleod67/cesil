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

## Implications already reflected in the reference

From examples + release notes (all **Jacobs-observed** until promoted):

- `*` used as a full-line comment marker in source (conflicts with classic end-of-data `*`)
- Unsigned numeric literals in code and data
- Data sections that end at EOF without a final `*` line
- Variables initialised to zero at run start (2.0)
- Optional `+` on positive constants; `-0` disallowed (1.2 notes)

## In-repo examples

This repository’s own samples remain the day-to-day demos:

- [`examples/total.ces`](../../examples/total.ces) — Wikipedia-style total
- [`examples/hello.ces`](../../examples/hello.ces) — minimal `PRINT` / `LINE` / `HALT`

They use classic-friendly signed `+0` and an explicit trailing `*`.
