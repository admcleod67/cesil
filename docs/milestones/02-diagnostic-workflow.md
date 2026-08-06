← [Project milestones index](../milestones.md)

## Milestone 2 — Diagnostic workflow (completed)

This milestone turns the Errors panel from a formatted list into a useful
diagnostic workflow modelled on Visual CESIL: diagnostics are presented as structured
rows, and activating a row takes the user to the corresponding source line. Status
messages are updated so previous check and run results do not remain current after
the source changes.

**Release note:** Milestone 2 is complete. The CLI and IDE report `0.2.0`
(`${PROJECT_VERSION}` from CMake). Cut git tag `v0.2.0` when ready.

It complements:

- [Project milestones](../milestones.md)
- [Milestone 1 — IDE edit → check → run](01-ide-mvp.md)
- [Milestone 3 — Editor ergonomics](03-editor-ergonomics.md)
- [Milestone 8 — Visual CESIL IDE parity](08-ide-parity.md)

### Goals

- Present structured diagnostics clearly rather than flattening them into text.
- Make every diagnostic with a source location directly navigable.
- Keep the status bar consistent with the current source and latest operation.
- Establish testable diagnostic behaviour that later Visual CESIL parity work can
  refine.

### Starting point

Milestone 1 displayed formatted diagnostics in a read-only `QListWidget`.
`cesil::Diagnostic` already carried severity, message, and optional one-based line
and column values. Check and Run selected the Errors tab on failure, but the IDE did
not navigate from an error to its source.

### Out of scope for Milestone 2

- Changes to parser or runtime diagnostic semantics
- Language-level error-message parity (owned by Milestone 6)
- Visible severity presentation while every emitted diagnostic is an error; revisit
  this when warnings or notes are introduced
- General editor commands and indentation behaviour (owned by Milestone 3)
- Syntax highlighting
- Broader IDE layout parity beyond the diagnostic workflow

---

## Deliverables

### Structured Errors panel

- Replaced the formatted list with a non-editable, single-selection `QTableView`.
- Follows the Visual CESIL reference with two visible columns: **Line** and
  **Description**.
- Line is compact; Description stretches to fill the remaining width.
- Shows only actual diagnostic rows.
- Preserves diagnostic order from `cesil-core`.
- Leaves the table empty after a successful check and reports
  `No compilation errors` in the status bar.
- Failures without detailed diagnostics show one unlocated, non-navigable
  Description row.
- Retains column and severity as model roles/metadata rather than visible columns.

### Source navigation

- Activating a diagnostic with a valid line (double-click or Enter/Return) switches
  to the Source tab.
- Positions the cursor at the diagnostic column when present, otherwise at the start
  of the line, clamping into the available document range.
- Ensures the target is visible and returns keyboard focus to the editor.
- Unlocated fallback rows remain visible and do not navigate.

### Status lifecycle

- Clear feedback for Ready, successful checks, diagnostic counts, compilation
  failures, runtime failures, successful runs, and saves.
- Compilation summaries follow Visual CESIL status-bar forms (Milestone 8 Stage 2):
  `No compilation errors`, `N compilation error(s)`.
- Any user source modification replaces the previous operation or save result with
  `Ready`.
- Tab selection alone does not reset the status.
- New and Open finish in `Ready`; Save reports `Saved.`

### Maintainable diagnostic presentation

- `DiagnosticModel` (`QAbstractTableModel`) owns structured diagnostics in the IDE
  layer.
- Helpers cover compilation summaries and source-position clamping.
- Qt Test coverage exercises model fields/metadata, empty/fallback states,
  summaries, and cursor clamping. This is the IDE automated-test baseline;
  broader coverage continues with later milestones rather than a standalone
  coverage project (see the testing strategy in the
  [project milestones index](../milestones.md)).

---

## Done when

- Errors are displayed as structured **Line** and **Description** table rows.
- Column and severity remain available as metadata without adding redundant visible
  columns.
- Double-clicking or activating a located diagnostic opens the correct source
  position.
- Unlocated diagnostics remain visible without causing invalid navigation.
- Successful checks leave the table empty, while failures without detailed
  diagnostics remain understandable.
- Editing after any reported operation restores `Ready` without relying on tab
  selection.
- Check, Run, New, Open, and Save produce consistent status messages.
- Automated IDE tests and the existing core suite pass.
- The CLI and IDE report `0.2.0`.
