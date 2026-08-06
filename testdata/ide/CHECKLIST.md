# Manual IDE parity checklist (Milestone 8)

Exercise on each supported platform (macOS, Linux, Windows). Tick when verified.
Automated tests cover helpers and status strings; this list covers MainWindow chrome.

See [Milestone 8](../../docs/milestones/08-ide-parity.md) and [`MATRIX.md`](MATRIX.md).

**Not in Milestone 8:** Debug menu or Debugger dialogue — [Milestone 9](../../docs/milestones/09-debugger.md).

## File and document

- [ ] New / Open / Save / Save As work; unsaved-change prompt appears when dirty
- [ ] Window title shows `CESIL IDE — {file}` (or Untitled) and a dirty ` *` marker
- [ ] Quit closes with the usual unsaved prompt when needed

## Compile and Errors

- [ ] Build → **Compile** (and toolbar) runs with **F7**
- [ ] Successful Compile selects the Errors tab; table empty; status `No compilation errors`
- [ ] Failed Compile shows Line / Description rows; status `N compilation error(s)`
- [ ] Activating an error row switches to Source and moves the cursor
- [ ] Editing source after Compile/Run sets status to `Ready`

## Run and Output

- [ ] Run (menu / toolbar / **F5**) clears Output, then runs
- [ ] Compile failure on Run stays on Errors (does not execute)
- [ ] Successful Run selects Output; Errors cleared; status `Program finished.`
- [ ] Runtime failure selects Errors; status `Run failed.`

## Help and About

- [ ] Help → About shows product name **CESIL IDE**, version matching `cesil --version`, MIT copyright
- [ ] About dismisses with OK
- [ ] On macOS, About appears under the application menu (AboutRole)

## Fonts and layout

- [ ] Source and Output use a fixed-pitch default font; CESIL columns remain readable

## Platform

| Platform | Operator / date | Notes |
|----------|-----------------|-------|
| macOS | | |
| Linux | | |
| Windows | | |
