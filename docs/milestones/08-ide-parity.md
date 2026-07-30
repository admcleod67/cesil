← [Project milestones index](../milestones.md)

## Milestone 8 — Visual CESIL IDE parity (planned)

This milestone brings the Qt IDE's workflow and information presentation to parity
with Visual CESIL 2.0. It preserves native macOS, Linux, and Windows behaviour rather
than copying the historical Windows appearance pixel for pixel.

It complements:

- [Project milestones](../milestones.md)
- [Milestone 6 — Visual CESIL language parity](06-language-parity.md)
- [Milestone 10 — Visual CESIL parity and 1.0 release](10-version-1-release.md)

### Goals

- Establish a documented UI and workflow reference from Visual CESIL 2.0.
- Present source, errors, and output with the same essential information and
  transitions as the reference application.
- Make the single-program edit → check → run workflow complete and predictable.
- Preserve cross-platform Qt conventions and accessibility.

### Starting point

Milestone 1 provides a single-buffer Qt IDE with Source, Errors, and Output tabs;
file actions; syntax checking; program execution; a toolbar; and a status bar.
Milestone 6 provides the language-compatible engine beneath it.

### Out of scope for Milestone 8

- New language features beyond Visual CESIL 2.0
- General-purpose IDE features such as projects, plugins, or multi-file builds
- Enhancements not present in Visual CESIL, unless required for native platform
  operation or accessibility
- Packaging / installers

---

## Deliverables

### Reference inventory and parity matrix

- Capture Visual CESIL 2.0 screenshots and workflows for file handling, checking,
  running, errors, and output.
- Record each user-visible element and transition as **match**, **gap**, or **not yet
  established**.
- Distinguish functional parity from Windows-specific decoration.
- Use the inventory to confirm the exact scope before changing the Qt layout.

### Error presentation

- Rework the Errors panel to expose the same useful fields, ordering, and summary
  information as Visual CESIL.
- Match the reference behaviour for successful checks, multiple errors, and
  compilation or runtime failures.
- Allow an error selection to identify the corresponding source location when that
  behaviour is present in the reference workflow.
- Keep diagnostic content supplied by `cesil-core`; the IDE owns only presentation
  and navigation.

### Editor and output workflow

- Match the reference transitions between Source, Errors, and Output after Check and
  Run.
- Restore a neutral Ready state once an edit makes a previous result stale.
- Match the useful source-editor presentation established by the reference
  inventory, including text layout and syntax treatment where applicable.
- Match output display, clearing, and focus behaviour.

### Menus, toolbar, status, and document lifecycle

- Verify action names, availability, ordering, shortcuts, and toolbar coverage
  against the reference workflow.
- Verify New, Open, Save, Save As, unsaved-change prompts, and window-title state.
- Ensure status messages describe the current edit/check/run state without
  presenting stale results as current.
- Keep keyboard access and native platform menu conventions intact.

### UI regression coverage

- Add focused tests for state transitions that can be exercised without fragile
  pixel comparisons.
- Maintain a short manual parity checklist for visual layout and platform-specific
  behaviour.
- Validate the workflow on macOS, Linux, and Windows before the milestone closes.

---

## Done when

- The UI parity matrix has no **not yet established** entries for Visual CESIL's
  essential edit → check → run workflow.
- The Errors panel, editor, output, actions, and state transitions satisfy the
  reference checklist.
- Automated tests cover stable application-state behaviour; the manual checklist
  covers visual and native-platform details.
- There are no known functional IDE gaps within the agreed Visual CESIL 2.0 scope.
