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
IDE automated tests currently cover diagnostic helpers only; this milestone extends
`tests/ide/` with focused editor-behaviour coverage rather than opening a separate
test-infrastructure project.

### Out of scope for Milestone 3

- Find / Replace and search navigation
- Syntax highlighting or completion
- Multiple documents, projects, or split editors
- Automatic source reformatting
- Diagnostic presentation and navigation (owned by Milestone 2)
- Multiple-error compilation and parser recovery (owned by Milestone 4)

### Implementation stages

Ship as one milestone and one `0.3.0` checkpoint. Split the work into ordered
stages so Edit-menu wiring and CESIL Tab behaviour can land and be reviewed
separately:

1. **Confirm source columns** (confirmed) — Label, instruction, and operand
   columns are locked at **1 / 9 / 17** (1-based) with visual tab width **8**,
   matching [`examples/total.ces`](../../examples/total.ces),
   [`examples/hello.ces`](../../examples/hello.ces), and Obelisk layout practice.
   Values live in `src/ide/SourceEditorLayout.hpp` for Stage 3 and its tests.
2. **Edit actions and menu** (done) — Shared Undo / Redo / Cut / Copy / Paste /
   Select All actions with platform shortcuts, Edit menu after File, and enabled
   states wired to the Source editor only (`SourceEditActions`).
3. **CESIL-aware Tab behaviour** (done) — Tab / Shift-Tab advance to columns
   `{1, 9, 17, …}` by inserting spaces (`SourceEditor` + `SourceIndent`); existing
   tabs still display at width 8.
4. **Close-out** — Document, title, cursor, and status checks across the new edit
   paths; automated editor-behaviour tests; cross-platform manual checklist; bump
   CLI and IDE to `0.3.0`.

Stages 2 and 3 are largely independent after stage 1. Stage 4 runs once both
feature stages are in place.

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
- Tab inserts **spaces** to the next stop (not tab characters). Existing `\t`
  characters in files remain accepted and display at visual width 8.

Confirmed source layout (1-based columns), recorded in
`src/ide/SourceEditorLayout.hpp`:

| Field | Column |
|--------|--------|
| Label | 1 |
| Instruction | 9 |
| Operand | 17 |
| Visual tab width | 8 |

Tab stops are `{1, 9, 17}` and then every +8 (25, 33, …). These match the
conventional CESIL label / instruction / operand fields in
[`examples/total.ces`](../../examples/total.ces) and
[`examples/hello.ces`](../../examples/hello.ces) (spaces or tabs that expand under
width 8). The lexer already expands tabs with width 8 for diagnostic columns;
parsing remains whitespace-token based, so columns are an editor convention rather
than a language rule. Visual CESIL 2.0 sample binaries are not vendored here; a
full layout audit remains Milestone 6.

### Document behaviour

- Confirm that edit actions correctly mark the document modified and update the
  window title.
- Ensure Undo can return a document to its unmodified state where Qt's document
  model permits.
- Preserve cursor-position status updates after menu actions and indentation.
- Keep unsaved-change prompts unchanged.

### Verification

Automated coverage grows with this milestone's feature work (see the testing
strategy in the [project milestones index](../milestones.md)):

- Add IDE tests for Tab/Shift-Tab at each configured source column and for
  indent/outdent with single and multiple selected lines.
- Add automated checks for Edit action enabled states where they can be exercised
  without fragile full-window UI automation.
- Exercise each Edit action through its menu and shortcut on a short manual
  checklist.
- Manually verify native menu labels and shortcuts on macOS, Linux, and Windows.
- Keep MainWindow wiring tests light unless a helper can be extracted cleanly.

---

## Done when

- The Edit menu exposes all planned commands with native shortcuts.
- Action enabled states follow editor selection and undo/redo availability.
- Tab and Shift-Tab implement the documented CESIL indentation model for single and
  multiline editing.
- Editing operations update document, title, cursor, and status state correctly.
- Automated editor-behaviour tests and the cross-platform manual checklist pass.
- The CLI and IDE report `0.3.0`.
