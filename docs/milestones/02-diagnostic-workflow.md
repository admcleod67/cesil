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
- General editor commands and indentation behaviour (owned by Milestone 3)
- Syntax highlighting
- Broader IDE layout parity beyond the diagnostic workflow

---

## Deliverables

### Structured Errors panel

- Replace the formatted list with a non-editable, single-selection table.
- Present source line, column, severity, and message as separate fields.
- Give the message column the remaining width and keep numeric columns compact.
- Preserve diagnostic order from `cesil-core`.
- Represent diagnostics without a source location clearly and keep them
  non-navigable.
- Continue to present successful checks and failures without detailed diagnostics
  in a deliberate, consistent state.

### Source navigation

- Double-clicking a diagnostic with a valid line switches to the Source tab.
- Position the cursor at the diagnostic column when present, otherwise at the start
  of the line.
- Ensure the target is visible and return keyboard focus to the editor.
- Clamp defensive navigation to the available document range without hiding invalid
  diagnostic data.
- Keep keyboard activation available alongside double-click where supported by the
  table.

### Status lifecycle

- Retain clear feedback for Ready, successful checks, diagnostic counts, compilation
  failures, runtime failures, successful runs, and saves.
- On the first source modification after Check or Run, replace the previous result
  with `Ready`.
- Do not reset the status merely because the Source tab is selected.
- Ensure programmatic New and Open operations finish in `Ready`, while Save can
  continue to report `Saved.`.
- Keep cursor line/column reporting independent from the transient status message.

### Maintainable diagnostic presentation

- Replace list-specific diagnostic helpers with table-oriented population code.
- Keep conversion from `cesil::Diagnostic` to Qt presentation in the IDE layer.
- Avoid parsing the existing formatted diagnostic string to recover structured
  fields.

### Verification

- Verify multiple diagnostics retain their order and fields.
- Verify navigation with line and column, line only, and no source location.
- Verify status transitions after Check, Run, edit, New, Open, and Save.
- Add automated coverage for stable transformation and navigation logic where it
  can be separated from the window; keep a short manual checklist for interaction
  and focus behaviour.

---

## Done when

- Errors are displayed as structured table rows.
- Double-clicking or activating a located diagnostic opens the correct source
  position.
- Unlocated diagnostics remain visible without causing invalid navigation.
- Editing after Check or Run restores `Ready` without relying on tab selection.
- Check, Run, New, Open, and Save produce consistent status messages.
- The diagnostic workflow passes its automated checks and manual interaction
  checklist.
- The CLI and IDE report `0.2.0`.
