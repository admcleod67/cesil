← [Project milestones index](../milestones.md)

## Milestone 9 — Visual CESIL Debugger (planned)

This milestone adds the Visual CESIL **Debugger** dialogue and related Debug-menu
actions so the pre-1.0 IDE matches the reference app’s stepping and inspection
workflow. It builds on main-window parity from
[Milestone 8](08-ide-parity.md) and the language engine from
[Milestone 6](06-language-parity.md).

Behaviour is parity with Visual CESIL 2.0’s Debugger, not a pixel copy of the
Windows Forms chrome. Native macOS, Linux, and Windows Qt conventions apply.

**Release checkpoint:** expected **`0.9.0`** (`project(cesil VERSION …)`). Cut git
tag `v0.9.0` when ready. Do not treat the version as closed until Stage 4 (CMake may
already read `0.9.0` from preparatory bumps — Stage 4 confirms CLI/IDE agree).

It complements:

- [Project milestones](../milestones.md)
- [Milestone 6 — Visual CESIL language parity](06-language-parity.md)
- [Milestone 8 — Visual CESIL IDE parity](08-ide-parity.md)
- [Milestone 10 — Visual CESIL parity and 1.0 release](10-version-1-release.md)
- Debugger notes in [`testdata/ide/PROBE.md`](../../testdata/ide/PROBE.md)

### Goals

- Add a **Debug** menu with actions that match the useful Visual CESIL set
  (open Debugger; Stop when a debug run is active — see settle-or-defer).
- Ship a Debugger dialogue with: Source (current-instruction highlight), Variables
  (Name / Value), Accumulator, Speed (Slow–Fast), Data, Output; buttons Run, Step,
  Stop, Reset, Quit.
- Drive execution through `cesil-core` (extend [`ExecutionHooks`](../../src/core/runtime/Hooks.hpp)
  / interpreter APIs as needed for step, pause, reset, and paced continuous run).
- Keep the Milestone 8 main-window **Run** menu for non-debug execution (**deliberate
  diverge** from Jacobs placing Run only under Debug).
- Preserve accessibility and native menu placement; fixed-size Jacobs dialogue is
  not required if a resizable Qt dialogue is clearer.

### Starting point

| Prior work | Available today |
|------------|-----------------|
| [Milestone 8](08-ide-parity.md) | Main window: Source / Errors / Output; Compile; top-level Run; Help → About; no Debug menu |
| [Milestone 6](06-language-parity.md) | Language-compatible parse + interpret; Jacobs runtime banners |
| `Interpreter` | `load` / `reset` / `run`; `accumulator()`, `programCounter()`, `store()`, `program()` |
| `ExecutionHooks` | `beforeInstruction_`, `afterInstruction_`, `shouldBreak_` (breakpoint stop before PC) |
| [`testdata/ide/PROBE.md`](../../testdata/ide/PROBE.md) | Debug menu labels; Debugger pane list; fixed-size note from Release Notes 1.2 |

### Out of scope for Milestone 9

- New language features or CESIL “Plus”
- Packaging / installers
- IR visualisation, hardware CESIL, or debugger features beyond Visual CESIL 2.0
  (e.g. arbitrary breakpoint UI not present in Jacobs)
- Replacing or removing the Milestone 8 top-level Run menu
- Main-window mnemonic gutter / syntax colouring (still deferred teaching chrome)
- Pixel-perfect Windows Forms layout

---

## Implementation stages

Ship as one milestone and one pre-1.0 checkpoint (version confirmed at Stage 4).
**Stage 1 must settle menu and session semantics before large UI or core rewrites.**

1. **Debugger probe and matrix** (next) — Extend IDE inventory for the Debugger only:
   confirm Debug menu items and enablement; dialogue controls; how Jacobs Run/Step/Stop/
   Reset/Quit and Speed behave; relationship between main-window Run and debug Run.
   Artefacts: extend [`testdata/ide/PROBE.md`](../../testdata/ide/PROBE.md) /
   [`MATRIX.md`](../../testdata/ide/MATRIX.md) and/or add
   [`testdata/ide/debugger/`](../../testdata/ide/debugger/) (`README.md`, probe notes).
   Little or no feature code. Lock Stage 2–3 implement lists and deliberate divergences
   (especially keeping top-level Run).

2. **Core stepping and debug session API** (after Stage 1) — Grow interpreter / hooks
   support for: single-step one instruction; continuous run with cooperative stop;
   reset of PC/accumulator/store/data pointer while keeping the loaded program;
   optional delay between instructions for Speed; expose enough state for Variables /
   Accumulator / Data / PC→source line (use instruction `lineNumber_` where present).
   Catch2 tests in `tests/core/` without Qt. Prefer expanding `ExecutionHooks` and
   small session helpers over blocking the GUI thread with a tight `run()` loop.

3. **Debugger UI and Debug menu** (after Stage 2) — Qt Debugger dialogue (Source
   highlight, Variables table, Accumulator, Speed slider, Data, Output, Run/Step/
   Stop/Reset/Quit); Debug menu to open it and Stop when appropriate. Wire to the
   Stage 2 API. Compile-gate before debug (same as Run: no execute on compile errors).
   Do not remove Milestone 8 Run. Native conventions; resizable dialogue allowed.

4. **Close-out** (last) — Manual debugger checklist (extend
   [`testdata/ide/CHECKLIST.md`](../../testdata/ide/CHECKLIST.md) or add
   `testdata/ide/debugger/CHECKLIST.md`); confirm version **`0.9.0`**; mark milestone
   completed; Next up → Milestone 10. List remaining divergences in the matrix.

Stage 2 depends on 1. Stage 3 depends on 2. Stage 4 depends on 2 and 3.

### Stage 1 settle-or-defer checklist

Inventory must classify at least:

| Topic | Working assumption (confirm in Stage 1) |
|-------|----------------------------------------|
| Debug menu | Add **Debug** with **Debugger…** (opens dialogue). **Stop** enabled only while a debug continuous-run is active (or always present but no-op when idle — probe Jacobs). |
| Main-window Run | **Keep** File/Edit/Build/**Run**/Help topology from M8 (**deliberate diverge** vs Jacobs Debug→Run-only). |
| Debug dialogue Run vs main Run | Separate debug session inside the dialogue; main Run stays fire-and-forget without opening Debugger. |
| Step | Execute exactly one instruction; refresh Variables / Accumulator / highlight. |
| Reset | Restore machine start state for the loaded program; clear debug Output as Jacobs does if observed. |
| Speed | Maps to delay between instructions during dialogue continuous Run (not main-window Run). |
| Source highlight | Highlight the source line for the current PC (instruction line metadata). |
| Variables | Show store Name / Value; update after each step/run slice. |
| Data | Show remaining or full data values as probe specifies. |
| Breakpoints | Jacobs Debugger has no rich breakpoint UI in the M8 inventory — **defer** extra breakpoint UX unless Stage 1 finds it. `shouldBreak_` may still support internal step-stop. |
| Dialogue size | Jacobs fixed-size (Release Notes); this project may use a **resizable** dialogue (**deliberate diverge**) for accessibility. |

---

## Deliverables

### Debugger probe and matrix

- Jacobs Debugger behaviour recorded (menus, buttons, Speed, reset semantics).
- Matrix rows for Debug menu + dialogue with **match** / **gap** / **deliberate diverge** /
  **defer**; Stage 2–3 actions explicit.

### Core debug session

- Step / run-with-stop / reset APIs testable from Catch2.
- Live read of accumulator, store, PC, and data position for the UI.
- PC associated with a source line for highlighting when line metadata exists.

### Debugger dialogue and menu

- Debug menu integrated with the Milestone 8 menu bar.
- Dialogue panes and buttons covering the essential Visual CESIL debug workflow.
- Compile-failed programs do not execute in the debugger.

### Verification

- Focused core tests for stepping and reset.
- Manual debugger checklist on macOS, Linux, and Windows (operator-ticked).
- No requirement for fragile pixel GUI automation.

---

## Done when

- Stages 1–4 complete; Debugger matrix has no blocking **not yet established** rows
  for the agreed Jacobs Debugger scope (divergences explicit).
- Debug menu and dialogue satisfy the reference checklist for Run / Step / Stop /
  Reset / Quit, Variables, Accumulator, Speed, Data, Output, and PC highlight.
- Core stepping tests green; manual checklist covers platform details.
- CLI and IDE report the Stage 4 checkpoint version (expected `0.9.0`).
- Milestone 10 can treat the Visual CESIL Debugger as in the `1.0.0` contract.
