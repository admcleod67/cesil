← [Project milestones index](../milestones.md)

## Milestone 8 — Visual CESIL IDE parity (planned)

This milestone brings the Qt IDE's **main window** workflow and information
presentation to parity with Visual CESIL 2.0. It preserves native macOS, Linux, and
Windows behaviour rather than copying the historical Windows appearance pixel for
pixel.

It complements:

- [Project milestones](../milestones.md)
- [Milestone 1 — IDE edit → check → run](01-ide-mvp.md)
- [Milestone 2 — Diagnostic workflow](02-diagnostic-workflow.md)
- [Milestone 3 — Editor ergonomics](03-editor-ergonomics.md)
- [Milestone 6 — Visual CESIL language parity](06-language-parity.md)
- [Milestone 10 — Visual CESIL parity and 1.0 release](10-version-1-release.md)

The Visual CESIL **Debugger** dialogue is **not** part of this milestone. Reserve it
for a later pre-1.0 milestone (planned as **Milestone 9** unless scope changes). The
1.0 contract should include that Debugger once that milestone completes.

**Release checkpoint:** assign at close-out (expected **`0.8.0`**,
`project(cesil VERSION …)`). Cut git tag `v0.8.0` when ready. Do not bump the version
until Stage 4.

### Goals

- Establish a documented UI and workflow reference from Visual CESIL 2.0 for the
  main window (not the Debugger).
- Present source, errors, and output with the same essential information and
  transitions as the reference application.
- Make the single-program edit → check → run workflow complete and predictable.
- Provide a Help → About dialogue comparable in *role* to Visual CESIL's, using this
  project's own branding and license text.
- Preserve cross-platform Qt conventions and accessibility.

### Starting point

| Prior work | What the IDE already has |
|------------|--------------------------|
| [Milestone 1](01-ide-mvp.md) | Single-buffer Qt IDE; Source / Errors / Output tabs; File New/Open/Save/Save As; Build → Check syntax; Run; toolbar; status bar; `QApplication` name / organisation / version |
| [Milestone 2](02-diagnostic-workflow.md) | Structured Errors table; activate row → source location; status cleared to Ready when the source becomes stale after a check/run |
| [Milestone 3](03-editor-ergonomics.md) | Edit menu; CESIL-aware Tab / Shift-Tab; action enablement tied to the editor |
| [Milestone 6](06-language-parity.md) | Language-compatible `cesil-core` used in-process by Check and Run |

Today the menu bar is File / Edit / Build / Run (no Help, no Debug). Check lives under
Build; Run has its own top-level menu. Jacobs uses File / Edit / Build / Debug / Help —
Stage 1 must decide how this project maps Run vs Debug without implementing the
Debugger dialogue here.

### Out of scope for Milestone 8

- The Visual CESIL Debugger dialogue (step / run-in-debugger / stop / reset, live
  variables and accumulator, speed slider, debugger-local Source / Output / Data) —
  **Milestone 9** (or another reserved pre-1.0 slot)
- Implementing Debug-menu actions that open or drive that dialogue (a disabled stub
  or omitted Debug menu is an inventory decision in Stage 1)
- New language features beyond Visual CESIL 2.0
- General-purpose IDE features (projects, plugins, multi-file builds, split editors)
- Packaging / installers
- Cloning Andrew Jacobs' copyright, About body text, or vendor branding
- Pixel-perfect Windows chrome
- Features absent from Visual CESIL unless required for native platform operation or
  accessibility

---

## Implementation stages

Ship as one milestone and one pre-1.0 checkpoint (version assigned at Stage 4). Work
lands in ordered stages. **Stage 1 must settle scope before large layout or menu
rewrites** — several editor-chrome items (syntax colouring, mnemonic gutter, Find,
Print) are easy to over-scope.

1. **Reference inventory and matrix** (next) — Capture Visual CESIL 2.0 main-window
   screenshots and workflows (file handling, Check, Run, Errors, Output, status,
   menus/toolbar, Help → About). Record each user-visible element and transition as
   **match**, **gap**, **deliberate diverge**, or **defer** (e.g. to Milestone 9).
   Distinguish functional parity from Windows-only decoration. Artefacts live under
   [`testdata/ide/`](../../testdata/ide/) (create in this stage: `README.md`,
   `PROBE.md`, `MATRIX.md`; screenshots optional / local-only if preferred). Little or
   no IDE code beyond notes. Lock what Stage 2–3 will implement.

2. **Workflow and Errors parity** (after Stage 1) — Align Check / Run tab selection,
   Output clearing and focus, Ready / stale status wording, and Errors panel fields /
   summary / empty-success behaviour with the matrix. Refine navigation on top of
   Milestone 2 where the inventory requires it. Keep diagnostic *text* from
   `cesil-core` (Milestone 4 / 6 wording policy unchanged). Grow focused IDE tests for
   stable state transitions.

3. **Shell chrome** (after Stage 1; may overlap late Stage 2) — Menus, toolbar,
   shortcuts, and enablement per the matrix; document title and dirty-state behaviour;
   Help → About (product name, `${PROJECT_VERSION}`, this project's copyright/license,
   optional URL; native Help / application-menu placement). Apply only the
   source-editor presentation items Stage 1 marked in-scope (e.g. monospace / column
   cues). Do **not** implement Debugger UI.

4. **Close-out** (last) — Manual parity checklist for visual and platform-specific
   details; validate on macOS, Linux, and Windows; assign **`0.8.0`**; mark the
   milestone completed; point “Next up” at the Debugger milestone (9) or the next
   allocated slot. List any explicit deferrals in the matrix.

Stage 2 and 3 both depend on Stage 1. Stage 4 depends on 2 and 3.

### Stage 1 settle-or-defer checklist

Inventory must explicitly classify at least:

| Topic | Typical options |
|-------|-----------------|
| Menu topology | Jacobs Build/Debug/Help vs this project's Build/Run; where Check and Run live |
| Debug menu | Omit until M9 / disabled stub / Help-only until Debugger lands |
| Errors columns and summaries | Match useful fields; keep own message text |
| Status strings | Ready, compilation failed / N errors, program finished, etc. |
| Source presentation | Monospace, tab stops, colouring, left “mnemonic” gutter — in M8, diverge, or defer |
| Find / Replace, Print | In M8 only if present and essential in Jacobs; else defer or diverge |
| About | In M8 (role parity; own text) |

---

## Deliverables

### Reference inventory and parity matrix

- Live or screenshot-backed probe of Visual CESIL 2.0 main-window behaviour.
- Matrix under [`testdata/ide/`](../../testdata/ide/) with no **not yet established**
  rows for Stage 1–settled main-window scope when the milestone closes.
- Explicit **defer** rows for Debugger and any editor chrome postponed past M8.

### Error presentation

- Errors panel exposes the same useful fields, ordering, and summary information as
  Visual CESIL where the matrix requires it.
- Match reference behaviour for successful checks, multiple errors, and compilation
  or runtime failures at the *presentation* layer.
- Error selection identifies the corresponding source location when that behaviour is
  in the reference workflow (build on Milestone 2).
- Diagnostic content remains supplied by `cesil-core`.

### Editor and output workflow

- Match reference transitions between Source, Errors, and Output after Check and Run.
- Restore a neutral Ready (or inventory-equivalent) state once an edit makes a
  previous result stale.
- Match output display, clearing, and focus behaviour.
- Apply only Stage 1–approved source-editor presentation changes.

### Menus, toolbar, status, document lifecycle, and About

- Verify action names, availability, ordering, shortcuts, and toolbar coverage against
  the matrix (native platform menu conventions intact).
- Verify New, Open, Save, Save As, unsaved-change prompts, and window-title / dirty
  state.
- Status messages describe the current edit / check / run state without presenting
  stale results as current.
- Help → About: this product's name, version aligned with `cesil --version` / Qt
  application version, copyright/license for *this* project, optional project URL;
  dismiss with OK. Match role, not Jacobs wording.

### UI regression coverage

Follow the testing strategy in the [project milestones index](../milestones.md): grow
automated checks with this milestone's workflow work; do not treat coverage as a
separate goal.

- Focused tests for state transitions that do not require fragile pixel comparisons.
- Short manual parity checklist (including opening About) for layout and
  platform-specific behaviour.
- Workflow validated on macOS, Linux, and Windows before close-out.

---

## Done when

- Stage 1–4 complete; matrix has no **not yet established** entries for the agreed
  main-window scope (Debugger and other deferrals remain explicit **defer** /
  **deliberate diverge** rows, not silent gaps).
- Errors panel, editor/output workflow, actions, About dialogue, and state transitions
  satisfy the reference checklist.
- Automated tests cover stable application-state behaviour; the manual checklist
  covers visual and native-platform details.
- CLI and IDE report the Stage 4 checkpoint version (expected `0.8.0`).
- There are no known functional IDE gaps within this milestone's agreed Visual CESIL
  2.0 **main-window** scope.
