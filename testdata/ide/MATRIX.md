# IDE compatibility matrix (Milestone 8 Stage 1)

Evidence: [`PROBE.md`](PROBE.md). Ours baseline: [`src/ide/MainWindow.cpp`](../../src/ide/MainWindow.cpp),
[`src/ide/DiagnosticUtils.cpp`](../../src/ide/DiagnosticUtils.cpp).

Status values: **match** | **gap** | **deliberate diverge** | **defer** (Milestone 9 or later).

## Menus and actions

| Topic | Jacobs (probe) | Ours today | Status | Stage 2/3 action |
|-------|----------------|------------|--------|------------------|
| Top-level menus | File, Edit, Build, Debug, Help | File, Edit, Build, Run | **deliberate diverge** | Stage 3: add **Help**; keep top-level **Run** (do not add Debug menu in M8). Document diverge vs Jacobs Debug grouping. |
| Compile / Check | Build → **Compile** | Build → **Check syntax** (F7) | **gap** (label) | Stage 3: rename to **Compile** (keep F7 or match Jacobs shortcut if probed). Same action semantics. |
| Run placement | Debug → **Run** | Run → **Run** (F5) | **deliberate diverge** | Stage 3: keep Run menu (or toolbar) for main-window run; Debug→Run parity waits for M9 menu work. |
| Stop (main) | Debug → **Stop** | None | **defer** | Milestone 9 (tied to debug/run-control). Not required for M8 edit→check→run. |
| Debugger command | Debug → **Debugger** | None | **defer** | Milestone 9. |
| Help → About | About... → product/license/OK | Missing | **gap** | Stage 3: implement Help → About (this product’s name, `${PROJECT_VERSION}`, own copyright/license, optional URL). |
| File lifecycle | New/Open/Save/Save As/Exit | New/Open/Save/Save As/Quit | **match** (role) / **deliberate diverge** (Quit vs Exit wording) | Stage 3: optional Exit wording on Windows; keep native Quit on macOS. |
| Edit menu | Undo, Cut, Copy, Paste, Delete, Select All (no Redo) | Undo, Redo, Cut, Copy, Paste, Select All (no Delete) | **deliberate diverge** | Stage 3: keep Redo (useful native diverge). Add Delete only if inventory prioritises it — **defer** Delete unless Stage 3 wants quick win. |
| Find / Replace | Not present | Not present | **match** (absent) | None. |
| Print | Not present | Not present | **match** (absent) | None. |

## Errors and status

| Topic | Jacobs (probe) | Ours today | Status | Stage 2/3 action |
|-------|----------------|------------|--------|------------------|
| Errors columns | Line, Description | Line, Description | **match** | None (keep). |
| Error navigation | Double-click → source line | Activate → source line/column | **match** (role) | Stage 2: confirm double-click vs activate feels right; keep column navigation (exceedance OK). |
| Diagnostic message text | Jacobs strings | Own `cesil-core` strings | **deliberate diverge** | None (policy). |
| Success status | `No compilation errors` | `No compilation errors.` | **gap** (period) | Stage 2: drop trailing period to match Jacobs, or accept micro-diverge — **prefer match** (remove period). |
| Error count status | `N compilation error(s)` | `1 compilation error` / `N compilation errors` | **gap** (plural form) | Stage 2: align to Jacobs `N compilation error(s)` **or** keep grammatical English as deliberate diverge — **prefer match** Jacobs form for status bar only. |
| Ready / stale | Ready; updates on edit | Ready on contentsChanged | **match** | Stage 2: verify no stale “Program finished.” / error counts after edit (already intended). |
| Check always selects Errors | Compile → Errors | Check always selects Errors tab (even on success) | **match** (likely) | Stage 2: confirm Jacobs always focuses Errors after Compile; keep or tweak if probe differs. |
| Run failure → Errors | Yes | Yes | **match** | None. |
| Run success → Output | Yes (expected) | Yes | **match** | Stage 2: confirm focus/clear behaviour against live Run if needed. |
| Run success status | Not string-locked in Stage 1 | `Program finished.` | **gap** / settle in Stage 2 | Stage 2: live-confirm Jacobs post-run status; match if a stable string exists. |
| Run failed status | Not string-locked in Stage 1 | `Run failed.` | **gap** / settle in Stage 2 | Stage 2: live-confirm; match if stable. |

## Document and window

| Topic | Jacobs (probe) | Ours today | Status | Stage 2/3 action |
|-------|----------------|------------|--------|------------------|
| Title pattern | `{file} - Visual CESIL` | `CESIL IDE — {file}` | **deliberate diverge** | Stage 3: keep product-named title (`CESIL IDE`); optional ` - ` separator polish. Do not brand as Visual CESIL. |
| Dirty marker | `*` on filename | ` *` suffix on title | **match** (role) | None. |
| Unsaved prompt | `{name} file has changed`… | Save changes to “{name}”? | **deliberate diverge** | Stage 3: optional wording closer to Jacobs; not blocking. |
| Tabs | Source, Errors, Output | Source, Errors, Output | **match** | None. |

## Source presentation

| Topic | Jacobs (probe) | Ours today | Status | Stage 2/3 action |
|-------|----------------|------------|--------|------------------|
| CESIL columns / Tab | TAB columns; teaching layout | Spaces to columns 1/9/17 (M3) | **deliberate diverge** (spaces vs tabs) | None in M8 (M3 contract). |
| Monospace / tab stops | Fixed pitch expected | `SourceEditor` tab stops for visual width 8 | **match** (role) | Stage 3: ensure default editor font is monospace if not already. |
| Mnemonic gutter | Present (screenshot) | Absent | **defer** / **deliberate diverge** | **Defer** past M8 (nice-to-have teaching chrome, not required for check/run). |
| Syntax colouring | Not required | None | **match** (absent) / **defer** if desired later | No colouring in M8. |

## About

| Topic | Jacobs (probe) | Ours today | Status | Stage 2/3 action |
|-------|----------------|------------|--------|------------------|
| About dialogue | Product, copyright, URL, license, OK | Missing | **gap** | Stage 3: role parity with **this** project’s branding/license (not Jacobs text). |

## Debugger (Milestone 9)

| Topic | Jacobs (probe) | Ours today | Status | Stage 2/3 action |
|-------|----------------|------------|--------|------------------|
| Debugger dialogue | Full DebugForm (Source, Variables, Accumulator, Speed, Data, Output, Run/Step/Stop/Reset/Quit) | None | **defer** | Milestone 9 only. |

---

## Stage 2 implement list (workflow / Errors)

1. Align compilation status strings with Jacobs (`No compilation errors`, `N compilation error(s)`) unless explicitly kept as diverge in implementation notes.
2. Verify/fix Ready-after-edit; Check/Run tab selection; Output clear on Run; post-run status strings via short live Run if needed.
3. Confirm Errors empty-on-success and navigation; no diagnostic text cloning.
4. Add focused IDE tests for status/tab transitions touched above.

## Stage 3 implement list (shell chrome)

1. Help menu + About dialogue (own identity/version/license).
2. Rename **Check syntax** → **Compile** (and toolbar tooltip/text).
3. Keep File/Edit/Build/Run topology; document Debug-menu deferral.
4. Optional: monospace default font; Exit vs Quit wording; unsaved-prompt polish.
5. Do **not** implement Debugger, mnemonic gutter, Find, Print, or Debug → Stop.

## Explicit deferrals

| Item | Until |
|------|--------|
| Debug menu, Debugger dialogue, Step/Stop/Reset/speed/variables | Milestone 9 |
| Mnemonic gutter / syntax colouring | Post-M8 or never (teaching extra) |
| Edit → Delete (Jacobs has it; we lack it) | Optional later; not blocking M8 |
| Exact post-run status literals if unseen in Stage 1 extract | Stage 2 live confirm |
