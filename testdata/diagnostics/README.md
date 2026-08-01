# Diagnostic fixtures (Milestone 4 Stage 1)

Curated multi-error CESIL programs for locking recovery and accumulation
behaviour. Jacobs tools (Visual CESIL 2.0 or his Java CLI) are a **behavioural
minimum**, not a string catalog. This project keeps its own clear messages and may
report more errors or suppress cascades more tightly when documented.

See [Milestone 4](../../docs/milestones/04-diagnostic-accumulation.md) and the
completed probe log in [`PROBE.md`](PROBE.md).

## Layout

Programs use conventional CESIL columns (label / instruction / operand) with spaces,
matching `examples/total.ces`.

## Windows probe

**Status:** fixture set probed against **Visual CESIL** (2026-08-01). Observations
and deliberate differences are recorded in [`PROBE.md`](PROBE.md). Version / host
metadata still pending where screenshots did not show them.

Original procedure (kept for re-probes):

1. Prefer **Visual CESIL 2.0** on Windows. Jacobs' separate **Java command-line
   CESIL** is acceptable if that is the practical option — record which tool and
   version/date you used (the Java CLI is not Visual CESIL 2.0).
2. Open or run each `.ces` file below.
3. Note which error *conditions* appear (not exact wording), their order, and
   whether later lines still seem to be considered after an earlier error.
4. Fill in [`PROBE.md`](PROBE.md).

## Fixture index

| File | Intent |
|------|--------|
| `syntax-multi-unknown.ces` | Two unknown mnemonics with a valid `LINE` between them |
| `syntax-missing-operands.ces` | `LOAD` / `ADD` / `STORE` without operands, then `HALT` |
| `syntax-junk-after-mnemonic.ces` | Extra tokens after `LINE` / `HALT`, then a clean `HALT` |
| `semantic-multi-undefined-jump.ces` | Two jumps to distinct missing labels (`FAR`, `AWAY`) |
| `semantic-duplicate-labels.ces` | Duplicate `HERE` label plus jump to `MISS` |
| `semantic-bad-operands.ces` | Operand-shape failures (`PRINT`, `STORE`, `JUMP`) plus a legal variable `ADD` |
| `data-multi-invalid.ces` | Invalid data rows mixed with valid integers |
| `mixed-syntax-then-semantic.ces` | Unknown mnemonic then undefined jump |
| `mixed-recover-then-valid.ces` | Bad mnemonic then a valid short program |
| `structural-percent-boundary.ces` | `%` / `*` boundary with trailing tokens after data |
| `structural-empty-ish.ces` | Minimal `HALT` + empty data section |

## Project contract (Stage 2 target)

Locked from the Visual CESIL probe. A single parse should report **all independently
recoverable** conditions below (wording may differ). Fail-fast today only surfaces
the first. Cascades from discarded bad lines should be suppressed. Any recorded
error ⇒ `ok_ == false` (no execution).

| File | Intended conditions (this project) | vs Jacobs |
|------|------------------------------------|-----------|
| `syntax-multi-unknown.ces` | Two unknown-instruction errors; still see later lines including `HALT` | Match floor (Jacobs reported both) |
| `syntax-missing-operands.ces` | Missing operand on each of `LOAD`, `ADD`, `STORE` | **Exceed** — Jacobs missed bare `STORE` |
| `syntax-junk-after-mnemonic.ces` | Unexpected trailing tokens on the junk lines; later clean `HALT` still parsed | **Exceed** — Jacobs accepted with no errors |
| `semantic-multi-undefined-jump.ces` | Two undefined-label / jump-target failures | Match floor (Jacobs reported both) |
| `semantic-duplicate-labels.ces` | Duplicate label + undefined jump target | Match floor (Jacobs reported both) |
| `semantic-bad-operands.ces` | Shape errors for unquoted `PRINT`, numeric `STORE`, and non-label `JUMP`; `ADD XYZ` is a legal variable operand (not a shape error) | **Exceed** on `PRINT` / `STORE` shapes; Jacobs only flagged `JUMP` |
| `data-multi-invalid.ces` | Invalid data integer for each bad row; valid `1`/`2`/`3` still collected when recoverable | Match floor (Jacobs reported both bad rows) |
| `mixed-syntax-then-semantic.ces` | Unknown instruction + undefined jump (both) | **Exceed** — Jacobs stopped after the unknown mnemonic |
| `mixed-recover-then-valid.ces` | One unknown instruction; remaining valid instructions still enter IR for validation | Match floor (single unknown; clean tail) |
| `structural-percent-boundary.ces` | Trailing post-`*` content diagnosed without treating the data section as code | Match intent (Jacobs: invalid data on the trailing line) |
| `structural-empty-ish.ces` | Successful compile of minimal `HALT` + empty data | Match (Jacobs accepted; control baseline) |

Do **not** add compile-time “undefined variable” diagnostics from these fixtures.
Whether a never-stored name reads as `0` is a Milestone 6 language-parity probe, not
a Stage 2 accumulation requirement. See [`PROBE.md`](PROBE.md) open questions.

Stage 2 Catch2 tests should consume these files (or equivalent embedded sources)
once the core accumulates diagnostics.

**Note:** Jump/label names in fixtures stay within the engine's 6-character identifier
limit (`FAR` / `AWAY` / `MISS`). The Visual CESIL probe used longer names
(`NOWHERE` / `ALSOGONE` / `MISSING`); conditions are the same.
