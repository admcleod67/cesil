← [Project milestones index](../milestones.md)

## Milestone 4 — Diagnostic accumulation and parser recovery (planned)

This milestone changes compilation from fail-fast reporting to collecting multiple
independent errors in one pass. Syntax parsing recovers at safe line boundaries,
semantic validation continues where results remain meaningful, and both the CLI and
IDE receive the same deterministic diagnostic set.

**Planned release checkpoint:** `0.4.0`.

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
multiple diagnostics. The parser and semantic validator currently return after their
first error, so the table normally receives only one row. Existing core tests are
smoke-level and assume fail-fast compilation; this milestone is where multi-error
behaviour becomes the testable contract rather than a separate coverage exercise.
This project keeps its own clear diagnostic messages rather than cloning Jacobs
wording.

### Out of scope for Milestone 4

- Continuing execution after a runtime error
- Warning or note diagnostics
- Changes to Errors-table presentation or navigation
- Word-for-word Visual CESIL (or Jacobs CLI) error messages
- Matching Jacobs only as an upper bound (exceeding his diagnostic set is allowed
  when it remains correct and non-cascading)
- New language syntax or instructions

### Implementation stages

Ship as one milestone and one `0.4.0` checkpoint. Work lands in ordered stages.
Unlike Milestone 3's independent Edit and Tab tracks, syntax recovery and semantic
accumulation share one compiler pipeline and ship together in stage 2:

1. **Reference fixtures and probe** — Author a small curated set of multi-error
   `.ces` programs (syntax, semantic, data-section, mixed, and structural edges).
   Run that set on Windows against Visual CESIL 2.0 when available; Jacobs' separate
   Java command-line CESIL is an acceptable practical probe (note which tool was
   used — it is not Visual CESIL 2.0). Record observed error *conditions*, ordering,
   and recovery points as a minimum. Lock this project's contract (which may be
   stricter or clearer) before changing the core.
2. **Core recovery and accumulation** — Line-oriented syntax recovery, semantic
   validation of independently checkable items, cascade suppression, and Catch2
   coverage driven by the stage 1 fixtures (plus focused unit cases). Partial IR
   must never execute when any error was recorded.
3. **Close-out** — Deterministic ordering, duplicate suppression, and the documented
   diagnostic limit; confirm CLI and IDE present the full set and correct counts;
   document the Jacobs floor and any deliberate differences; bump CLI and IDE to
   `0.4.0`.

Stage 2 depends on stage 1. Stage 3 runs once the core multi-error contract and its
tests are in place.

---

## Deliverables

### Reference behaviour

- Add a curated in-repo fixture collection (for example under `testdata/diagnostics/`)
  of programs with multiple syntax, semantic, and data-section errors — roughly a
  dozen focused files, not a fuzzer corpus.
- Exercise that collection on Windows with Visual CESIL 2.0 when available, or with
  Jacobs' Java CLI when that is the practical option; record which tool was used.
- Record which error conditions the reference reports, their ordering, and where
  parsing resumes. Treat that as a **minimum** for this project. Do not require
  string-for-string message text.
- Define this project's expected conditions and recovery rules from those findings
  plus the deliverables below; document where this project intentionally reports
  more, or suppresses cascades more tightly, than the reference.
- Prefer clear existing wording over cloning Jacobs' message text.

### Syntax recovery

- Replace immediate parser termination with line-oriented recovery for malformed
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

- Return diagnostics in deterministic source order, using line and column where
  available while preserving stable order for equal or unlocated positions.
- Emit each underlying source problem once.
- Apply a documented upper diagnostic limit to protect against pathological input;
  report when further diagnostics have been suppressed.
- Keep `DiagnosticSeverity::Error` for all diagnostics in this milestone.

### CLI and IDE integration

- Keep `ParseResult::diagnostics_` as the shared result consumed by both front ends.
- Confirm the CLI prints every collected diagnostic in order.
- Confirm the IDE displays every row, reports the correct compilation-error count,
  and navigates each located error.
- Keep runtime failures fail-fast and separate from compilation-error counts.

### Verification

Grow the Catch2 core suite as the acceptance criteria for this milestone (see the
testing strategy in the [project milestones index](../milestones.md)):

- Drive multi-error tests from the stage 1 fixture collection where practical.
- Add parser tests with multiple malformed instruction lines.
- Add semantic tests with multiple independent operand, label, and jump failures.
- Add data-section tests with multiple invalid rows mixed with valid rows.
- Add mixed syntax/semantic cases that verify recovery without cascades.
- Test deterministic ordering, duplicate suppression, the diagnostic limit, and
  unsuccessful results with partial internal state.
- Confirm CLI and IDE surfaces show the complete ordered set and correct counts.
- Keep existing single-error and diagnostic-presentation tests green as a
  regression baseline.

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
