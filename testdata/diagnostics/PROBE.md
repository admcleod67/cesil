# Jacobs probe log

Visual CESIL probe for Milestone 4 Stage 1. Treat results as a **minimum** floor,
not exact message text to clone. Probe of the fixture set is complete; version /
host metadata below remains incomplete where screenshots did not show it.

## Tool

| Field | Value |
|-------|--------|
| Tool | Visual CESIL |
| Version / build date | _pending_ (not visible in screenshots) |
| Host OS | _pending_ |
| Operator / date | _pending_ / 2026-08-01 |

## Per-file observations

For each file: list error *conditions* in order (paraphrase is fine), and note
whether later lines still appeared to be checked after an earlier error.

| File | Jacobs conditions (ordered) | Recovery / later lines? | Notes |
|------|-----------------------------|-------------------------|-------|
| `syntax-multi-unknown.ces` | 1. unknown operation `FOO` (L1); 2. unknown operation `BAR` (L3) | Yes — both bad mnemonics reported; valid `LINE` between them produced no error | Status: 2 compilation error(s). Wording: `Unknown operation '…'` |
| `syntax-missing-operands.ces` | 1. invalid literal/identifier on `LOAD` (L1); 2. same on `ADD` (L2) | Partial — continued past L1 to report L2; `STORE` (L3) produced no diagnostic | Status: 2 compilation error(s). Wording: `Invalid literal constant or identifier`. Only first two of three missing-operand lines flagged |
| `syntax-junk-after-mnemonic.ces` | (none) | n/a — accepted as clean compile | Trailing tokens after `LINE` / `HALT` ignored; no errors reported |
| `semantic-multi-undefined-jump.ces` | 1. undefined label `NOWHERE` (L1); 2. undefined label `ALSOGONE` (L2) | Yes — both jumps reported | Status: 2 compilation error(s). Wording: `Reference to undefined label …` |
| `semantic-duplicate-labels.ces` | 1. label `HERE` redeclared (L2); 2. undefined label `MISSING` (L3) | Yes — duplicate and undefined jump both reported | Status: 2 compilation error(s). Wording: `Label 'HERE' is redeclared`; `Reference to undefined label MISSING` |
| `semantic-bad-operands.ces` | 1. no target label on `JUMP +0` (L3) | No — only one error; `PRINT HELLO` and `STORE +1` silent | Status: 1 compilation error(s). Wording: `No target label specified`. `ADD XYZ` is a valid variable operand shape — silence there is **not** treated as Jacobs laxness (see open questions) |
| `data-multi-invalid.ces` | 1. invalid data value (L5 `NOTANUMBER`); 2. invalid data value (L7 `ALSOBAD`) | Yes — both bad rows reported; valid `1`/`2`/`3` between them produced no error | Status: 2 compilation error(s). Wording: `Invalid data value ""` (quoted payload empty in the UI, not the source token) |
| `mixed-syntax-then-semantic.ces` | 1. unknown operation `NOPE` (L1) | No — undefined jump `GHOST` (L2) not reported after the syntax error | Status: 1 compilation error(s). Wording: `Unknown operation 'NOPE'` |
| `mixed-recover-then-valid.ces` | 1. unknown operation `BADOP` (L1) | Apparent yes — only `BADOP` flagged; valid `LOAD`/`OUT`/`LINE`/`HALT` tail produced no further errors | Status: 1 compilation error(s). Wording: `Unknown operation 'BADOP'` |
| `structural-percent-boundary.ces` | 1. invalid data value (L6) | n/a — single structural/data diagnostic | Status: 1 compilation error(s). Wording: `Invalid data value ""`. Trailing post-`*` tokens appear treated as (empty) invalid data rather than code |
| `structural-empty-ish.ces` | (none) | n/a — accepted as clean compile | Minimal `HALT` + empty data section; no errors reported — accepted as a valid control baseline |

## Deliberate differences (this project)

Where cesil will intentionally report **more** than Jacobs, or otherwise diverge.
Jacobs remains the behavioural **minimum** for conditions he does report.

- `syntax-junk-after-mnemonic.ces` — Jacobs accepts trailing tokens after zero-operand
  mnemonics (`LINE EXTRA`, `HALT ALSO`) with no diagnostic. This project should
  report unexpected trailing tokens on those lines (and still parse the clean
  trailing `HALT`).
- `syntax-missing-operands.ces` — Jacobs flagged bare `LOAD` / `ADD` but not bare
  `STORE`. This project should report a missing operand on each of `LOAD`, `ADD`,
  and `STORE`.
- `semantic-bad-operands.ces` — Jacobs only reported `JUMP +0`. This project should
  also report wrong operand *shapes* for unquoted `PRINT HELLO` and numeric
  `STORE +1`. Do **not** treat `ADD XYZ` as a shape error: an identifier is a legal
  arithmetic operand (variable). Whether an never-stored name is a compile error is
  a separate language question (Milestone 6), not a Milestone 4 accumulation target.
- `mixed-syntax-then-semantic.ces` — Jacobs stopped after unknown `NOPE` and did not
  report the undefined `GHOST` jump. This project should report both when the jump
  line remains independently checkable after syntax recovery.

## Open questions (not Milestone 4 decisions)

- **Undefined variables** — Distinct from undefined *labels* (which Jacobs does
  reject at compile time). Live Check on `testdata/parity/probe-unset-var.ces`:
  `LOAD UNSET` is **not** a compile error. Live Run evidence also shows unset names
  read as `0`, and store names are case-sensitive (`Foo` ≠ `FOO`) — see
  [`testdata/parity/PROBE.md`](../parity/PROBE.md). Do not add a compile-time
  undefined-variable diagnostic in Milestone 4.
