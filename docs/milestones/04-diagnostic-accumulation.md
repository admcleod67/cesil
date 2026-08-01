← [Project milestones index](../milestones.md)

## Milestone 4 — Diagnostic accumulation and parser recovery (completed)

This milestone changes compilation from fail-fast reporting to collecting multiple
independent errors in one pass. Syntax parsing recovers at safe line boundaries,
semantic validation continues where results remain meaningful, and both the CLI and
IDE receive the same deterministic diagnostic set.

**Release note:** Milestone 4 is complete. The CLI and IDE report `0.4.0`
(`${PROJECT_VERSION}` from CMake). Cut git tag `v0.4.0` when ready.

It complements:

- [Project milestones](../milestones.md)
- [Milestone 2 — Diagnostic workflow](02-diagnostic-workflow.md)
- [Milestone 3 — Editor ergonomics](03-editor-ergonomics.md)
- [Milestone 6 — Visual CESIL language parity](06-language-parity.md)

### Goals

- Report multiple independent compilation errors without requiring repeated
  check/run cycles.
- Recover only at boundaries where subsequent source can be interpreted reliably.
- Avoid misleading cascaded diagnostics caused by an earlier malformed line.
- Preserve identical diagnostic behaviour in the core, CLI, and IDE.
- Treat Andrew Jacobs' implementations (Visual CESIL 2.0, or his Java CLI when that
  is the practical probe) as a **behavioural minimum**: cover at least the error
  *conditions* and safe recovery points observed there. This project may report
  more errors, suppress cascades more aggressively, or use clearer wording — those
  choices are deliberate, not defects, when documented.

### Starting point

Milestone 2 provides a structured Errors table capable of displaying and navigating
multiple diagnostics. Before this milestone the parser and semantic validator
returned after their first error, so the table normally received only one row.
Existing core tests were smoke-level and assumed fail-fast compilation; this
milestone made multi-error behaviour the testable contract. This project keeps its
own clear diagnostic messages rather than cloning Jacobs wording.

### Out of scope for Milestone 4

- Continuing execution after a runtime error
- Warning or note diagnostics
- Changes to Errors-table presentation or navigation
- Word-for-word Visual CESIL (or Jacobs CLI) error messages
- Matching Jacobs only as an upper bound (exceeding his diagnostic set is allowed
  when it remains correct and non-cascading)
- New language syntax or instructions

### Implementation stages

Shipped as one milestone and one `0.4.0` checkpoint. Work landed in ordered stages.
Unlike Milestone 3's independent Edit and Tab tracks, syntax recovery and semantic
accumulation share one compiler pipeline and shipped together in stage 2:

1. **Reference fixtures and probe** (done) —
   Curated multi-error programs live under [`testdata/diagnostics/`](../../testdata/diagnostics/).
   The set was probed against **Visual CESIL** (2026-08-01). Observed error
   *conditions*, ordering, recovery points, and deliberate differences are in
   [`PROBE.md`](../../testdata/diagnostics/PROBE.md); the Stage 2 contract is locked in
   [`testdata/diagnostics/README.md`](../../testdata/diagnostics/README.md). Version /
   host metadata in the probe log may still be filled in later.
2. **Core recovery and accumulation** (done) — Line-oriented syntax recovery, semantic
   validation of independently checkable items, cascade suppression, and Catch2
   coverage driven by the stage 1 fixtures (`DiagnosticAccumulationTest`). Partial IR
   never executes when any error was recorded. Implements the Jacobs floor and the
   documented deliberate exceedances (trailing junk, bare `STORE`, `PRINT`/`STORE`
   shapes, syntax-then-semantic pair).
3. **Close-out** (done) — `finalizeDiagnostics` sorts by source location, drops exact
   duplicates, and enforces `kMaxCompilationDiagnostics` (100) with a suppression
   diagnostic; CLI and IDE present the full finalized set; version remains `0.4.0`.

Stage 2 depended on stage 1. Stage 3 ran once the core multi-error contract and its
tests were in place.

**Out of scope reminder:** undefined *variables* (never-stored names) are not a
Stage 2 decision. Jacobs rejects undefined *labels* at compile time; whether an
unset store name is legal (often read as `0`) belongs to
[Milestone 6](06-language-parity.md).

---

## Deliverables

### Reference behaviour

- Curated in-repo fixtures under `testdata/diagnostics/` (roughly a dozen focused
  files, not a fuzzer corpus) — **done**.
- Probe against Visual CESIL with conditions, ordering, and recovery recorded in
  [`PROBE.md`](../../testdata/diagnostics/PROBE.md) — **done** (tool: Visual CESIL;
  version/host metadata still pending). Treat that log as a **minimum**. Do not
  require string-for-string message text.
- Project contract and deliberate exceedances locked in
  [`testdata/diagnostics/README.md`](../../testdata/diagnostics/README.md) — **done**.
  Documented exceedances: trailing junk after zero-operand mnemonics; missing
  operand on bare `STORE`; wrong `PRINT`/`STORE` operand shapes; unknown mnemonic
  plus undefined jump in one pass.
- Prefer clear existing wording over cloning Jacobs' message text.

### Syntax recovery

- Replaced immediate parser termination with line-oriented recovery for malformed
  instruction lines.
- After recording a line error, discard only the unconsumed remainder of that line
  and resume at the next source line.
- Do not add a malformed instruction or its label to the executable instruction
  stream.
- Continue through the code section so independent errors on later lines are
  reported.
- Recover from invalid data-section rows at the next line while preserving valid
  data-row ordering.
- Treat structural boundaries such as end-of-file and the `%` data marker
  deliberately; recovery must not reinterpret data as instructions or vice versa.

### Semantic accumulation

- Validate all independently checkable labels, operands, data values, and jump
  targets instead of returning after the first failure.
- Track invalid intermediate entries so later validation can skip checks that would
  only produce cascade errors.
- Continue validating unaffected instructions and symbols.
- Keep the overall parse result unsuccessful if any error was recorded; partial IR
  must never be executed.

### Diagnostic ordering and limits

- Return diagnostics in deterministic source order via `finalizeDiagnostics`, using
  line and column where available (unlocated positions sort after located ones)
  while preserving stable order for ties.
- Emit each underlying source problem once (exact `(line, column, message)`
  duplicates are dropped).
- Cap retained diagnostics at `kMaxCompilationDiagnostics` (100). When truncated,
  keep the first 99 finalized items and append
  `further compilation errors suppressed` (line/column 0).
- Keep `DiagnosticSeverity::Error` for all diagnostics in this milestone.

### CLI and IDE integration

- Keep `ParseResult::diagnostics_` as the shared result consumed by both front ends.
- The CLI prints every collected diagnostic in order.
- The IDE displays every row via `showDiagnostics` / `DiagnosticModel`, reports the
  correct compilation-error count, and navigates each located error.
- Keep runtime failures fail-fast and separate from compilation-error counts.

### Verification

Grew the Catch2 core suite as the acceptance criteria for this milestone (see the
testing strategy in the [project milestones index](../milestones.md)):

- Drive multi-error tests from the stage 1 fixture collection where practical.
- Parser tests with multiple malformed instruction lines.
- Semantic tests with multiple independent operand, label, and jump failures.
- Data-section tests with multiple invalid rows mixed with valid rows.
- Mixed syntax/semantic cases that verify recovery without cascades.
- Deterministic ordering, duplicate suppression, the diagnostic limit, and
  unsuccessful results with partial internal state (`finalizeDiagnostics` unit
  coverage plus a parse ordering case).
- Confirm CLI and IDE surfaces show the complete ordered set and correct counts.
- Keep existing single-error and diagnostic-presentation tests green as a
  regression baseline.

### Manual checklist

Exercise on each supported platform (macOS, Linux, Windows):

- [ ] `cesil --version` reports `cesil 0.4.0`.
- [ ] `cesil run` on a multi-error fixture (e.g. `testdata/diagnostics/mixed-syntax-then-semantic.ces`)
      prints every diagnostic in source order on stderr.
- [ ] IDE Check on the same fixture fills the Errors table with every row and a status
      count matching the row count.
- [ ] IDE Run on a multi-error program does not execute; Errors table and count match Check.
- [ ] Double-click / activate a located Errors row navigates Source to that line/column.
- [ ] A clean program still Checks/Runs successfully; runtime failures remain separate
      from compilation-error counts.

---

## Done when

- A curated multi-error fixture set exists and has been probed against a Jacobs
  reference tool (Visual CESIL 2.0 or Java CLI), with the behavioural minimum and
  any deliberate differences documented.
- A single parse reports all independently recoverable compilation errors within
  the documented limit.
- Later valid lines are still parsed and validated after an earlier recoverable
  error.
- Cascaded errors from invalid intermediate state are suppressed.
- Diagnostics are deterministic, source ordered, and not duplicated.
- Programs with any compilation error cannot execute.
- CLI and IDE present the same complete diagnostic set.
- All core and IDE tests pass.
- The CLI and IDE report `0.4.0`.
