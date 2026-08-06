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
| [Milestone 8](08-ide-parity.md) | Main window: Source / Errors / Output; Compile; top-level Run; Help → About; **Debug** → Debugger… / Stop |
| [Milestone 6](06-language-parity.md) | Language-compatible parse + interpret; Jacobs runtime banners |
| `Interpreter` | `load` / `reset` / `run` / `step`; `accumulator()`, `programCounter()`, `store()`, `program()`, `data()`, `dataPointer()`, `sourceLineAtPc()` |
| `ExecutionHooks` | `beforeInstruction_`, `afterInstruction_`, `shouldBreak_`, `shouldStop_`, `betweenInstructions_` |
| [`DebuggerDialog`](../../src/ide/DebuggerDialog.hpp) | Non-modal dialogue: Source highlight, Variables, Accumulator, Speed, Data, Output; Run/Step/Stop/Reset/Quit |
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

1. **Debugger probe and matrix** (done) — Inventory under
   [`testdata/ide/debugger/`](../../testdata/ide/debugger/) (`README.md`, `PROBE.md`,
   `MATRIX.md`). Evidence: Debugger screenshot, UTF-16 UI strings, Release Notes 1.2,
   parent IDE PROBE. Settle-or-defer rows classified; Stage 2/3 implement lists locked.
   No Debugger UI or stepping API code in this stage.

2. **Core stepping and debug session API** (done) — Added `Interpreter::step()`,
   `data()` / `dataPointer()` / `sourceLineAtPc()`; extended `ExecutionHooks` with
   `shouldStop_` (cooperative early return from `run()`) and `betweenInstructions_`
   (Speed delay / smoke). `reset()` clears PC, accumulator, store, and data pointer
   while keeping the loaded program and data values; clearing debug Output is
   UI-only. Catch2 coverage in `tests/core/runtime/DebugSteppingTest.cpp`.

3. **Debugger UI and Debug menu** (done) — Added **Debug** menu (Debugger…, Stop when
   continuous Run active) and non-modal [`DebuggerDialog`](../../src/ide/DebuggerDialog.hpp):
   Source line highlight, Variables table, Accumulator, Speed slider (1–500 ms between
   steps), Data list, Output via `QtIoHost`; Run/Step/Stop/Reset/Quit wired to
   `Interpreter::step()` and `reset()`. Compile-gate before open (same as Run).
   Top-level Run unchanged; resizable dialogue (deliberate diverge vs Jacobs fixed-size).

4. **Close-out** (last) — Manual debugger checklist (extend
   [`testdata/ide/CHECKLIST.md`](../../testdata/ide/CHECKLIST.md) or add
   `testdata/ide/debugger/CHECKLIST.md`); confirm version **`0.9.0`**; mark milestone
   completed; Next up → Milestone 10. List remaining divergences in the matrix.

Stage 2 depends on 1. Stage 3 depends on 2. Stage 4 depends on 2 and 3.

### Stage 1 settle-or-defer checklist

Settled in [`testdata/ide/debugger/MATRIX.md`](../../testdata/ide/debugger/MATRIX.md).
Summary:

| Topic | Decision |
|-------|----------|
| Debug menu | Stage 3: **Debug** → Debugger…; Stop when continuous debug Run active |
| Main-window Run | **Keep** (deliberate diverge vs Jacobs) |
| Debug vs main Run | Separate dialogue session; main Run unchanged |
| Step / Run / Reset / Quit / Speed | Parity targets for Stages 2–3 (see MATRIX) |
| Source highlight / Variables / Accumulator / Data / Output | Stage 3 UI on Stage 2 state |
| Breakpoints | **Defer** extra UI (absent in Jacobs) |
| Dialogue size | Resizable OK (**deliberate diverge** vs Jacobs fixed-size) |

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
