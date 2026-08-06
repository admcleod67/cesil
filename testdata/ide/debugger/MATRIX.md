# Debugger compatibility matrix (Milestone 9)

Evidence: [`PROBE.md`](PROBE.md). Ours today: **Debug** menu (**Debugger…**, **Stop**)
and non-modal [`DebuggerDialog`](../../../src/ide/DebuggerDialog.hpp) wired to
[`Interpreter::step()`](../../../src/core/runtime/Interpreter.hpp); top-level **Run**
kept (M8 deliberate diverge). Resizable dialogue (deliberate diverge vs Jacobs
fixed-size).

Status: **match** | **gap** | **deliberate diverge** | **defer**.

## Menu and session

| Topic | Jacobs | Ours today | Status | Notes |
|-------|--------|------------|--------|-------|
| Debug menu | Debug → Run, Stop, Debugger | **Debug** → Debugger…, Stop | **match** (subset) / **deliberate diverge** | No Debug → Run (M8 Run kept). Stop enabled during dialogue continuous Run only. |
| Main-window Run | Under Debug only | Top-level **Run** (M8) | **deliberate diverge** | Kept. |
| Debugger vs main Run | DebugForm has its own Run | Dialogue Run + main Run | **match** | Dialogue uses `QTimer` + `step()`; main Run unchanged. |
| Menu Stop (idle) | Present; dialogue Stop disabled at idle | Debug → Stop disabled at idle | **match** | Enabled during paced dialogue Run. |

## Dialogue controls

| Topic | Jacobs | Ours today | Status | Notes |
|-------|--------|------------|--------|-------|
| Step | One instruction; refresh state / highlight | Dialogue **Step** → `step()` | **match** | |
| Dialogue Run | Continuous, Speed-paced; Stop stops it | **Run** + `QTimer`/`step()` | **match** | Speed slider 1–500 ms between steps. |
| Reset | Start-state restore; clear debug Output | **Reset** → `reset()` + clear Output | **match** | |
| Quit | Closes dialogue | **Quit** closes dialogue | **match** | |
| Speed | Slow–Fast delay on dialogue Run | Vertical slider Slow (top) … Fast (bottom) | **match** | Step unaffected. |
| Source highlight | Current instruction line | Blue line highlight via `sourceLineAtPc()` | **match** | |
| Variables | Name / Value store table | `QTableWidget` from `store()` | **match** | |
| Accumulator | Single value field | Read-only field; refreshed after step/run | **match** | |
| Data | `dataList` of program data | `QListWidget` from parse `data()` at open | **match** | Static list; does not shrink on `IN`. |
| Output | Debug stdout | `QtIoHost` → dialogue Output | **match** | |
| Breakpoint UI | Absent | Absent | **match** / **defer** extras | No breakpoint UI in M9. |
| Dialogue size | Fixed (Release Notes) | Resizable `QDialog` | **deliberate diverge** | |
| Compile gate | No execute if compile fails | Parse gate before open (same as Run) | **match** | |

## Explicit deferrals

| Item | Until |
|------|--------|
| Rich breakpoint / watch UI | Post-M9 / never unless Jacobs evidence appears |
| Removing top-level Run | Never for M9 (deliberate diverge) |
| Main-window mnemonic gutter | Post-M8 teaching extra |
| Pixel-perfect fixed-size DebugForm | Deliberate diverge (resizable) |

---

## Stage 2 implement list (core) — done

1. ~~Public or testable **single-step** one instruction (update PC, accumulator, store, dataPtr).~~ — `Interpreter::step()`
2. ~~**Cooperative continuous run** with stop flag; optional **inter-instruction delay** for Speed.~~ — `shouldStop_`, `betweenInstructions_`
3. ~~**Reset** loaded program to start state (PC, accumulator, store, dataPtr); document Output clear as UI responsibility.~~ — `reset()`; Output clear remains UI-only
4. ~~Helpers or accessors for **PC → source line** (`Instruction::lineNumber_`), store snapshot, data values / dataPtr.~~ — `sourceLineAtPc()`, `store()`, `data()`, `dataPointer()`
5. ~~Catch2 coverage for step, reset, stop-during-run, and delay hook smoke (no Qt).~~ — `tests/core/runtime/DebugSteppingTest.cpp`

Prefer extending `ExecutionHooks` / a small debug-session helper over blocking GUI `run()` loops.

## Stage 3 implement list (UI) — done

1. ~~**Debug** menu: **Debugger…** opens dialogue; **Stop** enabled only during debug continuous Run.~~ — [`MainWindow`](../../../src/ide/MainWindow.cpp)
2. ~~Debugger dialogue: Source (highlight), Variables, Accumulator, Speed, Data, Output; Run / Step / Stop / Reset / Quit.~~ — [`DebuggerDialog`](../../../src/ide/DebuggerDialog.hpp)
3. ~~Wire controls to Stage 2 API; IoHost into dialogue Output; compile-gate before load/run.~~
4. ~~Keep Milestone 8 File/Edit/Build/**Run**/Help; do not move main Run under Debug.~~
5. ~~Resizable dialogue allowed; no breakpoint UI.~~
