← [Project milestones index](../milestones.md)

## Milestone 6 — Visual CESIL language parity (completed)

This milestone brought the CESIL parser and runtime into line with the project
[language reference](05-language-reference.md) and with Andrew Jacobs' Visual CESIL
2.0 behaviour for the classic instruction set. Open questions were settled via probe,
engine gaps closed, and behaviour pinned with goldens rather than reimplementing every
instruction from scratch.

It complements:

- [Project milestones](../milestones.md)
- [Milestone 1 — IDE edit → check → run](01-ide-mvp.md)
- [Milestone 4 — Diagnostic accumulation and parser recovery](04-diagnostic-accumulation.md)
- [Milestone 5 — CESIL language reference](05-language-reference.md)
- [Milestone 8 — Visual CESIL IDE parity](08-ide-parity.md)

### Goals

- Verify the implementation against the Milestone 5 language reference.
- Match Visual CESIL control flow and program output for settled classic scope.
- Resolve **open** / **Jacobs-observed** items via probe, then update the reference
  and compatibility matrix.
- Turn settled behaviour into automated regression tests.
- Record compatibility findings so later IDE work rests on a stable engine.

### Starting point

Milestones 1–4 provided the instruction opcodes, parsing and validation,
case-sensitive identifiers, a data section, structured multi-error diagnostics, and
an interpreter used by both the CLI and IDE. Milestone 5 provided the language
reference under [`docs/language/`](../language/README.md).

### Out of scope for Milestone 6

- Authoring the language reference from scratch (owned by Milestone 5)
- IDE layout or error-panel presentation
- Pixel-level reproduction of Visual CESIL
- New instructions or language extensions not in the Milestone 5 classic scope
  (including CESIL “Plus”)
- Rich debugger and stepping UI
- Packaging / installers
- Vendoring Jacobs Example `.ces` / `.exe` trees into the repo
- Cutting git tag `v0.6.0` (optional; assign when ready)

### Implementation stages

Shipped as one milestone and pre-1.0 checkpoint **`0.6.0`** (`project(cesil VERSION 0.6.0)`).

1. **Corpus gate and dialect probes** (done) — Gate fixtures and probe log under
   [`testdata/parity/`](../../testdata/parity/). Live Visual CESIL Check settled Q4–Q6
   (`*` comments, unsigned constants, EOF-or-`*` data; `(` kept as deliberate diverge).
2. **Parser and source compatibility** (done) — Code-section `*` comments in
   `Parser::parseSyntax`; unsigned / EOF-or-`*` already matched; locked by
   `SourceCompatibilityTest`.
3. **Runtime semantics and matrix** (done) — Runtime fixtures and
   `RuntimeSemanticsTest`; live Run confirmed Q3/Q7/Q8 and exposed Q9/Q10 gaps;
   Q11 max-6 kept as deliberate diverge.
4. **Golden suite and close-out** (done) — Closed Q9 (require `HALT`) and Q10 (Jacobs
   `** ERROR: …` banners); project-owned goldens under
   [`testdata/parity/golden/`](../../testdata/parity/golden/) with `GoldenCorpusTest`
   (line-ending normalisation + CLI-path smoke); all fourteen opcodes covered;
   milestone marked completed at `0.6.0`.

**Deferred (post-1.0 / later):** vendoring full Jacobs Example 3–5 trees; relaxing
identifier max-6 to match longer Jacobs names.

---

## Deliverables

### Compatibility matrix against the language reference

Settled in [`testdata/parity/MATRIX.md`](../../testdata/parity/MATRIX.md): no **gap** or
**not yet established** rows for the Milestone 6 classic scope (Q11 remains
**deliberate diverge**).

### Instruction and machine semantics

Verified and locked: all fourteen classic opcodes; unset stores as zero; case-sensitive
names; toward-zero `DIVIDE`; host-width arithmetic; require trailing `HALT`; Jacobs
runtime banners.

### Source, data, and labels

Verified: `%` data section; EOF or `*` termination; `*` comments; unsigned constants;
forward labels; max-6 identifiers (deliberate diverge vs Jacobs).

### Output and errors

Pinned by goldens and runtime tests for `PRINT` / `OUT` / `LINE`, `IN` exhaustion,
division by zero, and missing `HALT`. Compile-time diagnostic wording remains this
project’s own (Milestone 4).

### Parity regression suite

- Project-owned happy-path and error goldens (`GoldenCorpusTest`)
- Focused parity runtime tests (`RuntimeSemanticsTest`)
- Source-gate tests (`SourceCompatibilityTest`)
- CLI-equivalent parse → interpret smoke (no shell-out required in CI)

---

## Done when

- [x] Language reference has no blocking **open** items for classic scope (leftovers
      deferred or **deliberate diverge**).
- [x] Compatibility matrix has no **not yet established** / **gap** rows for that scope.
- [x] Every documented instruction covered by an automated test.
- [x] Golden corpus pins happy-path and runtime-error stdout (with `\r\n` → `\n`
      normalisation).
- [x] Checkpoint version `0.6.0`; git tag optional.

### Release note (`0.6.0`)

Language-parity checkpoint: Visual CESIL-aligned source forms (`*` comments, unsigned
constants, EOF-or-`*` data), runtime semantics (toward-zero divide, host-width ints,
require `HALT`, Jacobs `** ERROR: …` banners), and an in-repo golden suite. Deliberate
divergences: classic `(` comments, max-6 identifiers, own compile diagnostic text.
