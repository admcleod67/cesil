← [Project milestones index](../milestones.md)

## Milestone 2 — Diagnostic workflow (planned)

This milestone turns the Errors panel from a formatted list into a useful
diagnostic workflow modelled on Visual CESIL: diagnostics are presented as structured
rows, and activating a row takes the user to the corresponding source line. Status
messages are updated so previous check and run results do not remain current after
the source changes.

**Planned release checkpoint:** `0.2.0`.

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

Milestone 1 displays formatted diagnostics in a read-only `QListWidget`.
`cesil::Diagnostic` already carries severity, message, and optional one-based line
and column values. Check and Run select the Errors tab on failure, but the IDE does
not currently navigate from an error to its source.

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

- Replace the formatted list with a non-editable, single-selection table.
- Follow the Visual CESIL reference with two visible columns: **Line** and
  **Description**.
- Keep Line compact and give Description the remaining width.
- Show only actual diagnostic rows rather than reproducing the reference
  application’s empty grid rows.
- Preserve diagnostic order from `cesil-core`.
- Represent diagnostics without a source location clearly and keep them
  non-navigable.
- Leave the table empty after a successful check; report
  `No compilation errors.` in the status bar rather than adding a placeholder row.
- If compilation or execution fails without detailed diagnostics, add one unlocated,
  non-navigable Description row explaining the failure.
- Retain column and severity as row metadata rather than visible columns.
- Use column metadata for precise navigation.
- Retain severity so future warnings and notes can be counted, filtered, and
  presented without changing the diagnostic model.

### Source navigation

- Double-clicking a diagnostic with a valid line switches to the Source tab.
- Position the cursor at the diagnostic column when present, otherwise at the start
  of the line.
- Ensure the target is visible and return keyboard focus to the editor.
- Clamp defensive navigation to the available document range without hiding invalid
  diagnostic data.
- Enter or Return activates the selected diagnostic using the same behaviour as
  double-click.

### Status lifecycle

- Retain clear feedback for Ready, successful checks, diagnostic counts, compilation
  failures, runtime failures, successful runs, and saves.
- Match the useful Visual CESIL summary while using correct singular and plural
  wording, such as `1 compilation error` and `2 compilation errors`.
- On any user source modification, replace the previous operation or save result
  with `Ready`.
- Do not reset the status merely because the Source tab is selected.
- Ensure programmatic New and Open operations finish in `Ready`, while Save can
  continue to report `Saved.`.
- Keep cursor line/column reporting independent from the transient status message.

### Maintainable diagnostic presentation

- Replace list-specific diagnostic helpers with table-oriented population code.
- Keep conversion from `cesil::Diagnostic` to Qt presentation in the IDE layer.
- Store the complete structured diagnostic with its row; do not discard column or
  severity merely because they are not visible.
- Avoid parsing the existing formatted diagnostic string to recover structured
  fields.

### Verification

- Verify multiple diagnostics retain their order, visible fields, and hidden
  metadata.
- Verify navigation with line and column, line only, and no source location.
- Verify both double-click and Enter/Return activation.
- Verify that successful checks leave the table empty and failures without detailed
  diagnostics create one unlocated explanatory row.
- Verify singular and plural compilation-error summaries.
- Verify status transitions after Check, Run, edit, New, Open, and Save.
- Add automated coverage for stable transformation and navigation logic where it
  can be separated from the window; keep a short manual checklist for interaction
  and focus behaviour.

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
- The diagnostic workflow passes its automated checks and manual interaction
  checklist.
- The CLI and IDE report `0.2.0`.
