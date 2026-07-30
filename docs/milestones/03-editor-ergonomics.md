← [Project milestones index](../milestones.md)

## Milestone 3 — Editor ergonomics (planned)

This milestone makes source editing feel complete and predictable. It adds the
standard Edit menu expected of a desktop editor and defines Tab behaviour suited to
CESIL's label, instruction, and operand layout.

**Planned release checkpoint:** `0.3.0`.

It complements:

- [Project milestones](../milestones.md)
- [Milestone 2 — Diagnostic workflow](02-diagnostic-workflow.md)
- [Milestone 4 — Diagnostic accumulation and parser recovery](04-diagnostic-accumulation.md)
- [Milestone 8 — Visual CESIL IDE parity](08-ide-parity.md)

### Goals

- Expose standard editing operations through menus and native shortcuts.
- Keep edit actions synchronized with what the source editor can currently do.
- Make Tab and Shift-Tab useful for writing column-oriented CESIL source.
- Preserve normal text-editor expectations across macOS, Linux, and Windows.

### Starting point

Milestone 1 uses `QPlainTextEdit`, so core undo, redo, clipboard, selection, and
tab-insertion capabilities already exist. They are not exposed through an Edit menu,
and the default Tab behaviour has not been deliberately configured for CESIL source.

### Out of scope for Milestone 3

- Find / Replace and search navigation
- Syntax highlighting or completion
- Multiple documents, projects, or split editors
- Automatic source reformatting
- Diagnostic presentation and navigation (owned by Milestone 2)
- Multiple-error compilation and parser recovery (owned by Milestone 4)

---

## Deliverables

### Edit actions and menu

- Create shared actions for Undo, Redo, Cut, Copy, Paste, and Select All.
- Add an Edit menu with conventional grouping:
  - Undo / Redo
  - Cut / Copy / Paste
  - Select All
- Use Qt's platform-standard key sequences and connect actions to the source editor.
- Keep action ownership consistent with the existing File, Build, and Run actions.

### Action state

- Enable Undo and Redo only when the corresponding operation is available.
- Enable Cut and Copy only when source text is selected.
- Keep Paste behaviour consistent with clipboard availability and Qt platform
  conventions.
- Ensure Edit actions target the Source editor and do not unexpectedly mutate the
  read-only Errors or Output views.
- Preserve normal context-menu editing behaviour.

### CESIL-aware Tab behaviour

- Configure a documented visual tab width suitable for CESIL source.
- With no multiline selection, Tab inserts indentation to the next configured
  source column rather than moving focus away from the editor.
- Shift-Tab moves indentation back to the previous configured source column without
  deleting non-whitespace source text.
- With a multiline selection, Tab and Shift-Tab indent and outdent all selected
  lines while preserving the selection.
- Keep the behaviour deterministic for empty lines, partially indented lines, and
  mixed labels/instructions.
- Store source as ordinary whitespace accepted by the CESIL parser; do not introduce
  editor-only formatting metadata.

The initial source columns are the conventional CESIL layout: label, instruction,
and operand. Their exact positions should be confirmed against representative Visual
CESIL programs before implementation and then recorded in the editor tests.

### Document behaviour

- Confirm that edit actions correctly mark the document modified and update the
  window title.
- Ensure Undo can return a document to its unmodified state where Qt's document
  model permits.
- Preserve cursor-position status updates after menu actions and indentation.
- Keep unsaved-change prompts unchanged.

### Verification

- Exercise each Edit action through its menu and shortcut.
- Verify enabled states with empty text, selection changes, clipboard changes, and
  undo/redo history.
- Test Tab and Shift-Tab at each configured source column.
- Test indent/outdent with single and multiple selected lines.
- Manually verify native menu labels and shortcuts on macOS, Linux, and Windows.

---

## Done when

- The Edit menu exposes all planned commands with native shortcuts.
- Action enabled states follow editor selection and undo/redo availability.
- Tab and Shift-Tab implement the documented CESIL indentation model for single and
  multiline editing.
- Editing operations update document, title, cursor, and status state correctly.
- Automated editor-behaviour tests and the cross-platform manual checklist pass.
- The CLI and IDE report `0.3.0`.
