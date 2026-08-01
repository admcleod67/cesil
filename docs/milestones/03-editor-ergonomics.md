← [Project milestones index](../milestones.md)

## Milestone 3 — Editor ergonomics (completed)

This milestone makes source editing feel complete and predictable. It adds the
standard Edit menu expected of a desktop editor and defines Tab behaviour suited to
CESIL's label, instruction, and operand layout.

**Release note:** Milestone 3 is complete. The CLI and IDE report `0.3.0`
(`${PROJECT_VERSION}` from CMake). Cut git tag `v0.3.0` when ready.

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

Milestone 1 used `QPlainTextEdit`, so core undo, redo, clipboard, selection, and
tab-insertion capabilities already existed. They were not exposed through an Edit
menu, and the default Tab behaviour had not been deliberately configured for CESIL
source. IDE automated tests covered diagnostic helpers only; this milestone extended
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

Shipped as one milestone and one `0.3.0` checkpoint. Work landed in ordered stages:

1. **Confirm source columns** (done) — Label, instruction, and operand columns locked
   at **1 / 9 / 17** (1-based) with visual tab width **8**, matching
   [`examples/total.ces`](../../examples/total.ces),
   [`examples/hello.ces`](../../examples/hello.ces), and Obelisk layout practice.
   Values live in `src/ide/SourceEditorLayout.hpp`.
2. **Edit actions and menu** (done) — Shared Undo / Redo / Cut / Copy / Paste /
   Select All actions with platform shortcuts, Edit menu after File, and enabled
   states wired to the Source editor only (`SourceEditActions`).
3. **CESIL-aware Tab behaviour** (done) — Tab / Shift-Tab advance to columns
   `{1, 9, 17, …}` by inserting spaces (`SourceEditor` + `SourceIndent`); existing
   tabs still display at width 8.
4. **Close-out** (done) — Status bar uses visual columns; document/title/cursor
   behaviour confirmed across Edit and Tab paths; cross-platform manual checklist;
   CLI and IDE report `0.3.0`.

---

## Deliverables

### Edit actions and menu

- Shared actions for Undo, Redo, Cut, Copy, Paste, and Select All.
- Edit menu with conventional grouping:
  - Undo / Redo
  - Cut / Copy / Paste
  - Select All
- Qt platform-standard key sequences connected to the Source editor.
- Action ownership consistent with the existing File, Build, and Run actions.

### Action state

- Undo and Redo enabled only when the corresponding operation is available.
- Cut and Copy enabled only when source text is selected.
- Paste follows clipboard availability via `canPaste()` and clipboard change signals.
- Edit actions always target the Source editor and do not mutate the read-only Errors
  or Output views.
- Normal context-menu editing behaviour preserved.

### CESIL-aware Tab behaviour

- Visual tab width 8 for display of existing tab characters.
- With no multiline selection, Tab inserts spaces to the next configured source
  column rather than moving focus away from the editor.
- Shift-Tab moves indentation back to the previous configured source column without
  deleting non-whitespace source text.
- With a multiline selection, Tab and Shift-Tab indent and outdent all selected
  lines while preserving the selection.
- Behaviour is deterministic for empty lines, partially indented lines, and mixed
  labels/instructions.
- Source remains ordinary whitespace accepted by the CESIL parser; Tab inserts
  **spaces** (not tab characters). Existing `\t` characters in files remain accepted.

Confirmed source layout (1-based columns), recorded in
`src/ide/SourceEditorLayout.hpp`:

| Field | Column |
|--------|--------|
| Label | 1 |
| Instruction | 9 |
| Operand | 17 |
| Visual tab width | 8 |

Tab stops are `{1, 9, 17}` and then every +8 (25, 33, …).

### Document behaviour

- Edit actions and indentation mark the document modified and update the window
  title.
- Undo can return a document to its unmodified state where Qt's document model
  permits.
- Cursor-position status uses CESIL visual columns (tab width 8) and updates after
  menu actions and indentation.
- Unsaved-change prompts are unchanged.

### Verification

Automated coverage grew with this milestone's feature work (see the testing
strategy in the [project milestones index](../milestones.md)):

- IDE tests for Tab/Shift-Tab at configured source columns and for indent/outdent
  with single and multiple selected lines (`cesil_ide_layout_tests`,
  `cesil_ide_indent_tests`).
- Automated checks for Edit action enabled states without full-window UI automation
  (`cesil_ide_edit_actions_tests`).
- MainWindow wiring kept light; helpers extracted as `SourceEditActions`,
  `SourceIndent`, and `SourceEditorLayout`.

### Manual checklist

Exercise on each supported platform (macOS, Linux, Windows):

- [ ] Edit menu exposes Undo, Redo, Cut, Copy, Paste, and Select All with native
      shortcuts and conventional separators.
- [ ] Undo/Redo enable with document history; Cut/Copy enable only with a Source
      selection; Paste follows clipboard availability.
- [ ] Edit commands mutate Source only (not Errors or Output), including when
      another tab is visible.
- [ ] Tab inserts spaces to the next stop (1 → 9 → 17 → 25…); Shift-Tab outdents
      leading whitespace only; multiline indent/outdent preserves selection.
- [ ] Tab does not insert `\t`; existing tabs in opened files still display correctly.
- [ ] Typing, Edit actions, and Tab update the window title modified marker; Undo can
      clear it when Qt allows.
- [ ] Status Col uses visual columns (matches Tab stops on space- and tab-indented
      lines); Ready appears after source edits; unsaved prompts on New/Open/Quit are
      unchanged.
- [ ] Native menu labels and shortcuts look correct for the platform.

---

## Done when

- The Edit menu exposes all planned commands with native shortcuts.
- Action enabled states follow editor selection and undo/redo availability.
- Tab and Shift-Tab implement the documented CESIL indentation model for single and
  multiline editing.
- Editing operations update document, title, cursor, and status state correctly.
- Automated editor-behaviour tests and the cross-platform manual checklist pass.
- The CLI and IDE report `0.3.0`.
