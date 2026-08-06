# Visual CESIL Debugger probe log (Milestone 9 Stage 1)

Debugger-specific observations for Visual CESIL 2.0. Main-window IDE probe remains
in [`../PROBE.md`](../PROBE.md).

## Tool

| Field | Value |
|-------|--------|
| Tool | Visual CESIL 2.0 |
| Evidence | Operator Debugger screenshot (2026-08-02); UTF-16 UI strings from local `Visual CESIL.exe` (non-repo); Release Notes 1.2 (About/Debugger fixed size; Speed slider; debugger source scrolls with execution); parent IDE PROBE Debug menu rows |
| Host | Windows (live UI / screenshots); string extract on macOS from the same binary |
| Operator / date | Stage 1 inventory lock 2026-08-06 |

Absolute paths to the local distribution are intentionally omitted.

## Debug menu (main window)

| Item | Label (binary) | Role |
|------|----------------|------|
| Menu | `&Debug` | Top-level menu (Jacobs has no separate top-level Run) |
| Run | `&Run` | Runs the program (main-window path under Debug in Jacobs) |
| Stop | `&Stop` | Stops an in-progress run |
| Debugger | `Debugger` | Opens `DebugForm` |

No breakpoint / watch menu items appear in UTF-16 UI strings.

## Debugger dialogue (`DebugForm`)

Release Notes 1.2: About and Debugger windows **cannot be resized** (Jacobs).

### Layout (screenshot + binary)

| Pane / control | Evidence |
|----------------|----------|
| **Source:** | `sourceLabel` / source text; current instruction highlighted (blue on screenshot); scrolls with execution (Release Notes 1.2) |
| **Variables:** | `variablesLabel` / `variablesList` — Name / Value columns (screenshot); empty before any `STORE` |
| **Accumulator:** | `accumulatorLabel` / `accumulatorTextBox` — shows `0` at start |
| **Speed:** | `speedLabel` / `speedTrackBar`; scale **Slow** (top) … **Fast** (bottom) |
| **Data:** | `dataLabel` / `dataList` — list of program data values (screenshot empty when data is only `%` / `*`) |
| **Output:** | `outputLabel` — program stdout during debug Run/Step |
| Buttons | **Run**, **Step**, **Stop**, **Reset**, **Quit** (`runButton`, `stepButton`, `stopButton`, `resetButton`, `quitButton`) |

Mnemonic gutter letters on debugger Source (screenshot) are teaching chrome; main-window
gutter remains out of M8/M9 scope unless revisited later.

### Button semantics (settled for Stage 2–3)

| Control | Jacobs behaviour | Settlement |
|---------|------------------|------------|
| **Step** | Execute one instruction; advance PC highlight; refresh Accumulator / Variables | **specified** for parity |
| **Run** | Continuous execution in the dialogue; paced by Speed; Stop becomes available | **specified** |
| **Stop** | Halts continuous Run; screenshot shows **Stop disabled** at idle (before Run) | **specified** — enable while continuous debug Run is active; disabled when idle / after Step-only |
| **Reset** | Return to program start state (PC, accumulator, stores, data pointer) with loaded program kept | **specified**; clear debug Output on Reset (teaching IDE expectation; consistent with empty Output at open) |
| **Quit** | Close the Debugger dialogue | **specified** |

### Speed

| Topic | Settlement |
|-------|------------|
| Effect | Delay between instructions during dialogue **Run** (Slow = longer delay, Fast = shorter). Does **not** apply to main-window Run. |
| Step | Unaffected by Speed (one instruction per click). |

Release Notes 1.2: Speed slider added to help watch code run.

### Data pane

| Topic | Settlement |
|-------|------------|
| Contents | Shows the program’s data values (`dataList`), not a live “next IN only” single cell. Empty when the data section has no integers. |
| During `IN` | Values remain listed; consumption is reflected by machine state / further `IN` errors, not necessarily by removing list rows (no contrary evidence in strings). Stage 3 may show full static data list from parse. |

### Compile gate

Debugger assumes a successfully compiled program (same as main Run). Opening Debugger
with compile errors should refuse to execute (align with main-window Run gate). Exact
Jacobs error toast not string-locked; Stage 3 matches “no execute on compile failure”.

### Breakpoints

No breakpoint UI strings or screenshot controls. Extra breakpoint UX is **out of
Milestone 9** unless a later live probe finds it. Core may still use
`shouldBreak_` internally for step-stop.

### Main-window Run vs Debugger Run

Jacobs places Run under **Debug**. This project keeps Milestone 8 top-level **Run**
for fire-and-forget execution and adds Debugger for inspect/step sessions
(**deliberate diverge** on menu topology; dialogue behaviour still matches Jacobs).
