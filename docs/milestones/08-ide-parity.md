← [Project milestones index](../milestones.md)

## Milestone 8 — Visual CESIL IDE parity (completed)

This milestone brought the Qt IDE's **main window** workflow and information
presentation to parity with Visual CESIL 2.0 for the agreed edit → compile → run
scope. It preserves native macOS, Linux, and Windows behaviour rather than copying
historical Windows appearance pixel for pixel.

**Release note:** Milestone 8 is complete. The CLI and IDE report `0.8.0`
(`${PROJECT_VERSION}` from CMake). Cut git tag `v0.8.0` when ready.

It complements:

- [Project milestones](../milestones.md)
- [Milestone 1 — IDE edit → check → run](01-ide-mvp.md)
- [Milestone 2 — Diagnostic workflow](02-diagnostic-workflow.md)
- [Milestone 3 — Editor ergonomics](03-editor-ergonomics.md)
- [Milestone 6 — Visual CESIL language parity](06-language-parity.md)
- [Milestone 9 — Visual CESIL Debugger](09-debugger.md)
- [Milestone 10 — Visual CESIL parity and 1.0 release](10-version-1-release.md)

### Goals

- Documented UI/workflow reference for the main window (not the Debugger).
- Source, errors, and output with the same essential information and transitions.
- Predictable single-program edit → compile → run workflow.
- Help → About with this project's branding and license.
- Cross-platform Qt conventions and accessibility.

### Starting point

Milestones 1–3 provided the IDE shell, Errors table, and Edit/Tab ergonomics.
Milestone 6 provided the language-compatible engine. Stage 1 inventory locked scope
under [`testdata/ide/`](../../testdata/ide/).

### Out of scope for Milestone 8

- Visual CESIL Debugger dialogue and Debug-menu actions — [Milestone 9](09-debugger.md)
- New language features, projects/plugins, packaging/installers
- Cloning Jacobs About text or vendor branding; pixel-perfect Windows chrome
- Mnemonic gutter, syntax colouring, Find, Print

### Implementation stages

Shipped as one milestone and pre-1.0 checkpoint **`0.8.0`**.

1. **Reference inventory and matrix** (done) — [`testdata/ide/`](../../testdata/ide/)
   PROBE/MATRIX; settle-or-defer classifications.
2. **Workflow and Errors parity** (done) — Jacobs compilation status strings;
   Check/Run tab and Ready/Output behaviour confirmed; post-run status deliberate diverge.
3. **Shell chrome** (done) — Help → About; Compile label; monospace Source/Output;
   File/Edit/Build/Run/Help without Debug.
4. **Close-out** (done) — Manual [`CHECKLIST.md`](../../testdata/ide/CHECKLIST.md);
   version `0.8.0`; milestone completed; Next up → Milestone 9.

### Deferred / deliberate divergences

| Item | Status |
|------|--------|
| Debugger / Debug menu | **defer** → Milestone 9 |
| Mnemonic gutter / syntax colouring | **defer** / teaching extra |
| Edit → Delete | optional later |
| Quit vs Exit; unsaved-prompt wording | **deliberate diverge** |
| Post-run status (`Program finished.` / `Run failed.`) | **deliberate diverge** |
| Top-level Run vs Jacobs Debug → Run | **deliberate diverge** |
| Own diagnostic message text | **deliberate diverge** (policy) |

---

## Deliverables (completed)

- Inventory and matrix under [`testdata/ide/`](../../testdata/ide/)
- Errors presentation and navigation (on Milestone 2); status summaries match Jacobs compile forms
- Compile / Run / Ready / Output workflow
- Help → About; Compile menu/toolbar; fixed-pitch Source/Output
- Manual checklist; `compilationErrorSummary` unit tests

---

## Done when

- [x] Stages 1–4 complete; matrix has no **not yet established** / blocking **gap** for main-window scope
- [x] Errors, editor/output workflow, actions, About, and state transitions satisfy the checklist
- [x] Automated tests cover stable helpers; manual checklist covers visual/platform details
- [x] CLI and IDE report `0.8.0`
- [x] No known functional gaps within the agreed main-window Visual CESIL 2.0 scope
