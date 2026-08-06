# Visual CESIL IDE probe log (Milestone 8 Stage 1)

Main-window UI and workflow observations for Visual CESIL 2.0. Treat results as
behavioural reference for presentation and transitions — not pixel decoration and
not Jacobs diagnostic *wording* to clone.

## Tool

| Field | Value |
|-------|--------|
| Tool | Visual CESIL 2.0 |
| Evidence | Operator screenshots (2026-08-01 diagnostics session; 2026-08-02 About / main window / Debugger); UTF-16 UI strings from local `Visual CESIL.exe` (non-repo distribution); [`Release Notes.htm`](../../docs/language/compatibility-corpus.md) (1.2 notes on error double-click, About/Debugger fixed size) |
| Host OS | Windows (live UI); string extract on macOS from the same binary |
| Operator / date | Stage 1 inventory lock 2026-08-06 |

Absolute paths to the local distribution are intentionally omitted.

## Menu bar

| Menu | Jacobs items (labels) | Notes |
|------|----------------------|--------|
| **File** | New, Open..., Save, Save As..., Exit | Exit (not Quit). Filter: `CESIL Files (*.ces)\|*.ces\|All files (*.*)\|*.*` |
| **Edit** | Undo, Cut, Copy, Paste, Delete, Select All | No Redo; no Find / Replace in binary UI strings |
| **Build** | Compile | Label is **Compile**, not “Check syntax” |
| **Debug** | Run, Stop, Debugger | **Run** and **Stop** live here; **Debugger** opens the debug dialogue (M9) |
| **Help** | About... | Opens About dialogue |

No separate top-level **Run** menu. No **Print** menu item in extracted UI strings.

Toolbar icons exist for several File / Compile / Run actions (image resources on those
menu items); exact toolbar button set not required for Stage 1 beyond noting Jacobs
exposes Compile and Run from menus (and typically toolbar).

## Tabs

| Tab | Present |
|-----|---------|
| Source | Yes |
| Errors | Yes (`errorPage` / `errorList`) |
| Output | Yes |

## Errors panel

| Item | Jacobs |
|------|--------|
| Columns | **Line**, **Description** (`lineColumnHeader`, `descriptionColumnHeader`) |
| Navigation | Double-click positions source on the line (Release Notes 1.2; `errorList_DoubleClick`) |
| Empty success | Table empty; status **No compilation errors** (no trailing period in binary string) |
| Multi-error status | `{n} compilation error(s)` (binary fragment ` compilation error(s)`; live M4 probe used forms like `2 compilation error(s)`) |
| Message text | Jacobs wording (e.g. `Unknown operation '…'`) — **not** cloned by this project |

## Status bar

| Situation | Jacobs string (evidence) |
|-----------|--------------------------|
| Idle / after edit | **Ready** |
| Successful compile | **No compilation errors** |
| Failed compile | **`N` compilation error(s)** |
| Unsaved prompt | `{name} file has changed` / `Do you want to save changes?` |
| Successful / failed program run (main window) | No stable status-bar literals in the UTF-16 extract (`Program finished` / `Run failed` absent). Stage 2 keeps this project’s `Program finished.` / `Run failed.` as **deliberate diverge**. |

## Window title

| State | Jacobs |
|-------|--------|
| Pattern | `{filename} - Visual CESIL` (binary suffix ` - Visual CESIL`) |
| Dirty | Trailing `*` on the filename portion observed in screenshots (e.g. `mixed-recover-then-valid.ces* - Visual CESIL`) |

## Check / Run workflow (main window)

| Action | Jacobs behaviour |
|--------|------------------|
| Compile (Build → Compile) | Compiles; Errors tab used for diagnostics; status as above |
| Run (Debug → Run) | Runs if compile OK; Output shows program stdout; compile failure stays on Errors |
| Output | Cleared/replaced on run (confirm exact clear timing in Stage 2 if needed) |
| Edit after result | Status returns toward **Ready** when source changes (aligns with M2 design goal; Jacobs TextChanged → status update) |

## Source presentation

| Item | Jacobs |
|------|--------|
| Column layout | TAB/space columns (Obelisk docs; M3 locked 1/9/17 for this project) |
| Mnemonic gutter | Left margin shows single-letter mnemonic cues (screenshot of Debugger source; main Source similar teaching chrome) |
| Syntax colouring | Not established as essential; no strong colouring requirement from Stage 1 |
| Monospace | Fixed-pitch source expected for column alignment |

## Help → About

| Field | Jacobs (screenshot + binary) |
|-------|------------------------------|
| Title | About |
| Product | Visual CESIL 2.0 |
| Copyright | Copyright (C)2002-2010 Andrew J Jacobs |
| Link line | Visit http://www.obelisk.demon.co.uk/cesil for more information |
| Body | Scrollable license / disclaimer text |
| Dismiss | OK |
| Resize | Fixed size (Release Notes 1.2) |

## Debugger (Milestone 9)

Detailed probe and matrix: [`debugger/`](debugger/). Opened via Debug → Debugger.
Fixed-size in Jacobs (Release Notes). Contains Source (PC highlight), Variables,
Accumulator, Speed, Data, Output, and Run / Step / Stop / Reset / Quit.

## Find / Print

No Find or Print menu items located in UTF-16 UI strings. Treat as **absent** from
the essential Visual CESIL 2.0 main-window workflow unless a later live probe finds
them under a non-menu path.
