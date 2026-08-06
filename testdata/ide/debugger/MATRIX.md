# Debugger compatibility matrix (Milestone 9)

Evidence: [`PROBE.md`](PROBE.md). Ours today: no Debug menu or Debugger dialogue
([`MainWindow`](../../../src/ide/MainWindow.cpp) File/Edit/Build/Run/Help only).
Core: [`Interpreter`](../../../src/core/runtime/Interpreter.hpp) +
[`ExecutionHooks`](../../../src/core/runtime/Hooks.hpp) — Stage 2: `step()`,
`data()` / `dataPointer()` / `sourceLineAtPc()`, `shouldStop_`,
`betweenInstructions_`; `reset()` clears PC/accumulator/store/dataPtr (Output clear
is UI-only).

Status: **match** | **gap** | **deliberate diverge** | **defer**.

## Menu and session

| Topic | Jacobs | Ours today | Status | Stage 2/3 action |
|-------|--------|------------|--------|------------------|
| Debug menu | Debug → Run, Stop, Debugger | Missing | **gap** | Stage 3: add **Debug** with **Debugger…**; **Stop** when debug continuous-run active. |
| Main-window Run | Under Debug only | Top-level **Run** (M8) | **deliberate diverge** | **Keep** M8 Run; do not remove. |
| Debugger vs main Run | DebugForm has its own Run | Main Run only | **gap** | Stage 3: dialogue Run uses Stage 2 session API; main Run unchanged. |
| Menu Stop (idle) | Present; dialogue Stop disabled at idle (screenshot) | None | **gap** | Stage 3: disable Stop when idle; enable during paced Run. |

## Dialogue controls

| Topic | Jacobs | Ours today | Status | Stage 2/3 action |
|-------|--------|------------|--------|------------------|
| Step | One instruction; refresh state / highlight | `Interpreter::step()` | **match** (core) | Stage 3: wire button. |
| Dialogue Run | Continuous, Speed-paced; Stop stops it | `run()` + `shouldStop_` / `betweenInstructions_` | **match** (core) | Stage 3: wire. |
| Reset | Start-state restore; clear debug Output | `Interpreter::reset()` (PC/acc/store/dataPtr) | **gap** (UI) / **match** (core) | Stage 3: button + clear Output. |
| Quit | Closes dialogue | None | **gap** | Stage 3: close dialogue. |
| Speed | Slow–Fast delay on dialogue Run | `betweenInstructions_` hook | **match** (core) | Stage 3: slider. |
| Source highlight | Current instruction line | `sourceLineAtPc()` | **match** (core) | Stage 3: highlight. |
| Variables | Name / Value store table | `store()` | **match** (core) | Stage 3: table. |
| Accumulator | Single value field | Readable via `accumulator()` | **gap** (UI) | Stage 3: display; refresh after step/run. |
| Data | `dataList` of program data | `data()` / `dataPointer()` | **match** (core) | Stage 3: list UI. |
| Output | Debug stdout | None in dialogue | **gap** | Stage 3: capture via IoHost into dialogue Output. |
| Breakpoint UI | Absent | Absent | **match** (absent) / **defer** extras | No breakpoint UI in M9. |
| Dialogue size | Fixed (Release Notes) | N/A | **deliberate diverge** (planned) | Stage 3: resizable Qt dialogue OK. |
| Compile gate | No execute if compile fails | Main Run already gates | **gap** (debug path) | Stage 3: same gate before debug session load. |

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

## Stage 3 implement list (UI) — locked

1. **Debug** menu: **Debugger…** opens dialogue; **Stop** enabled only during debug continuous Run.
2. Debugger dialogue: Source (highlight), Variables, Accumulator, Speed, Data, Output; Run / Step / Stop / Reset / Quit.
3. Wire controls to Stage 2 API; IoHost into dialogue Output; compile-gate before load/run.
4. Keep Milestone 8 File/Edit/Build/**Run**/Help; do not move main Run under Debug.
5. Resizable dialogue allowed; no breakpoint UI.
