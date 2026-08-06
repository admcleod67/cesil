# IDE compatibility matrix (Milestone 8 Stages 1–4)

Evidence: [`PROBE.md`](PROBE.md). Manual close-out: [`CHECKLIST.md`](CHECKLIST.md).
Ours: [`src/ide/MainWindow.cpp`](../../src/ide/MainWindow.cpp),
[`src/ide/DiagnosticUtils.cpp`](../../src/ide/DiagnosticUtils.cpp),
[`src/ide/SourceEditor.cpp`](../../src/ide/SourceEditor.cpp).

Status values: **match** | **gap** | **deliberate diverge** | **defer** (Milestone 9 or later).

No **not yet established** or blocking **gap** rows remain for the Milestone 8
main-window scope. Remaining non-match rows are **deliberate diverge** or **defer**.

## Menus and actions

| Topic | Jacobs (probe) | Ours today | Status | Stage 2/3 action |
|-------|----------------|------------|--------|------------------|
| Top-level menus | File, Edit, Build, Debug, Help | File, Edit, Build, Run, Help | **deliberate diverge** | Stage 3 done: Help added; Run kept; no Debug (M9). |
| Compile / Check | Build → **Compile** | Build → **Compile** (F7) | **match** (label) | Done Stage 3. |
| Run placement | Debug → **Run** | Run → **Run** (F5) | **deliberate diverge** | Keep (Stage 3). |
| Stop (main) | Debug → **Stop** | None | **defer** | Milestone 9. |
| Debugger command | Debug → **Debugger** | None | **defer** | Milestone 9. |
| Help → About | About... → product/license/OK | Help → About CESIL IDE… | **match** (role) | Done Stage 3 (own branding/license). |
| File lifecycle | New/Open/Save/Save As/Exit | New/Open/Save/Save As/Quit | **match** (role) / **deliberate diverge** (Quit vs Exit) | Keep Quit (Stage 3; no Exit rename). |
| Edit menu | Undo, Cut, Copy, Paste, Delete, Select All (no Redo) | Undo, Redo, Cut, Copy, Paste, Select All (no Delete) | **deliberate diverge** | Keep Redo; Delete deferred. |
| Find / Replace | Not present | Not present | **match** (absent) | None. |
| Print | Not present | Not present | **match** (absent) | None. |

## Errors and status

| Topic | Jacobs (probe) | Ours today | Status | Stage 2/3 action |
|-------|----------------|------------|--------|------------------|
| Errors columns | Line, Description | Line, Description | **match** | None (keep). |
| Error navigation | Double-click → source line | Activate → source line/column | **match** (role) | Confirmed Stage 2. |
| Diagnostic message text | Jacobs strings | Own `cesil-core` strings | **deliberate diverge** | None (policy). |
| Success status | `No compilation errors` | `No compilation errors` | **match** | Done Stage 2. |
| Error count status | `N compilation error(s)` | `N compilation error(s)` | **match** | Done Stage 2. |
| Ready / stale | Ready; updates on edit | Ready on contentsChanged | **match** | Confirmed Stage 2. |
| Check always selects Errors | Compile → Errors | Compile always selects Errors tab | **match** | Confirmed Stage 2. |
| Run failure → Errors | Yes | Yes | **match** | Confirmed Stage 2. |
| Run success → Output | Yes | Clears Output first; Output tab; Errors cleared | **match** | Confirmed Stage 2. |
| Run success status | No stable UI literal in binary | `Program finished.` | **deliberate diverge** | Keep ours (Stage 2). |
| Run failed status | No stable UI literal in binary | `Run failed.` | **deliberate diverge** | Keep ours (Stage 2). |

## Document and window

| Topic | Jacobs (probe) | Ours today | Status | Stage 2/3 action |
|-------|----------------|------------|--------|------------------|
| Title pattern | `{file} - Visual CESIL` | `CESIL IDE — {file}` | **deliberate diverge** | Keep product-named title (Stage 3). |
| Dirty marker | `*` on filename | ` *` suffix on title | **match** (role) | None. |
| Unsaved prompt | `{name} file has changed`… | Save changes to “{name}”? | **deliberate diverge** | Keep (Stage 3; no Jacobs clone). |
| Tabs | Source, Errors, Output | Source, Errors, Output | **match** | None. |

## Source presentation

| Topic | Jacobs (probe) | Ours today | Status | Stage 2/3 action |
|-------|----------------|------------|--------|------------------|
| CESIL columns / Tab | TAB columns; teaching layout | Spaces to columns 1/9/17 (M3) | **deliberate diverge** (spaces vs tabs) | None in M8 (M3 contract). |
| Monospace / tab stops | Fixed pitch expected | System fixed font on Source + Output; tab width 8 | **match** | Done Stage 3. |
| Mnemonic gutter | Present (screenshot) | Absent | **defer** / **deliberate diverge** | **Defer** past M8. |
| Syntax colouring | Not required | None | **match** (absent) | No colouring in M8. |

## About

| Topic | Jacobs (probe) | Ours today | Status | Stage 2/3 action |
|-------|----------------|------------|--------|------------------|
| About dialogue | Product, copyright, URL, license, OK | CESIL IDE, version, own copyright, MIT, OK | **match** (role) | Done Stage 3 (`QMessageBox::about`; AboutRole). |

## Debugger (Milestone 9)

| Topic | Jacobs (probe) | Ours today | Status | Stage 2/3 action |
|-------|----------------|------------|--------|------------------|
| Debugger dialogue | Full DebugForm | None | **defer** | Milestone 9 only. |

---

## Stage 2 implement list (workflow / Errors) — done

1. Aligned compilation status strings with Jacobs via `compilationErrorSummary` + `DiagnosticUtilsTest`.
2. Audited Ready-after-edit, Check/Run tab selection, Output clear on Run — **match**.
3. Errors empty-on-success and navigation confirmed.
4. Post-run status settled as **deliberate diverge**.

## Stage 3 implement list (shell chrome) — done

1. Help menu + About dialogue (CESIL IDE / version / MIT copyright).
2. Renamed Check syntax → **Compile** (F7 unchanged).
3. Kept File/Edit/Build/Run; Help added; Debug deferred to M9.
4. System fixed font on Source and Output; Quit and unsaved-prompt wording left as diverge.
5. Did not implement Debugger, mnemonic gutter, Find, Print, or Debug → Stop.

## Explicit deferrals

| Item | Until |
|------|--------|
| Debug menu, Debugger dialogue, Step/Stop/Reset/speed/variables | Milestone 9 |
| Mnemonic gutter / syntax colouring | Post-M8 or never (teaching extra) |
| Edit → Delete (Jacobs has it; we lack it) | Optional later; not blocking M8 |
| Post-run status string literals | **deliberate diverge** (Stage 2) |
| Exit vs Quit / Jacobs unsaved-prompt wording | **deliberate diverge** (Stage 3) |
