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
  check/fix cycles.
- Recover only at boundaries where subsequent source can be interpreted reliably.
- Avoid misleading cascaded diagnostics caused by an earlier malformed line.
- Preserve identical diagnostic behaviour in the core, CLI, and IDE.
- Establish Visual CESIL's multiple-error behaviour as the compatibility reference.

### Starting point

Milestone 2 provides a structured Errors table capable of displaying and navigating
multiple diagnostics. The parser and semantic validator currently return after their
first error, so the table normally receives only one row. Existing core tests are
smoke-level and assume fail-fast compilation; this milestone is where multi-error
behaviour becomes the testable contract rather than a separate coverage exercise.

### Out of scope for Milestone 4

- Continuing execution after a runtime error
- Warning or note diagnostics
- Changes to Errors-table presentation or navigation
- Final error-message wording parity (owned by Milestone 6)
- New language syntax or instructions

---

## Deliverables

### Reference behaviour

- Exercise Visual CESIL 2.0 with programs containing multiple syntax, semantic, and
  data-section errors.
- Record which errors it reports together, their ordering, and where it resumes
  parsing.
- Use the reference findings to refine recovery details without reproducing
  misleading cascades or implementation-specific crashes.

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

- A single parse reports all independently recoverable compilation errors within
  the documented limit.
- Later valid lines are still parsed and validated after an earlier recoverable
  error.
- Cascaded errors from invalid intermediate state are suppressed.
- Diagnostics are deterministic, source ordered, and not duplicated.
- Programs with any compilation error cannot execute.
- CLI and IDE present the same complete diagnostic set.
- Reference behaviour and any deliberate safety differences are documented.
- All core and IDE tests pass.
- The CLI and IDE report `0.4.0`.
