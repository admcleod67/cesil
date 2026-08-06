← [Project milestones index](../milestones.md)

## Milestone 9 — Visual CESIL Debugger (planned)

This milestone adds the Visual CESIL **Debugger** dialogue and related Debug-menu
actions so the pre-1.0 IDE matches the reference app’s stepping and inspection
workflow. It builds on the main-window parity from
[Milestone 8](08-ide-parity.md) and the language engine from
[Milestone 6](06-language-parity.md).

**Release checkpoint:** assign when the milestone is defined in detail (after M8’s
`0.8.0`).

It complements:

- [Project milestones](../milestones.md)
- [Milestone 8 — Visual CESIL IDE parity](08-ide-parity.md)
- [Milestone 10 — Visual CESIL parity and 1.0 release](10-version-1-release.md)
- Inventory notes in [`testdata/ide/PROBE.md`](../../testdata/ide/PROBE.md) (Debugger section)

### Goals

- Debug menu actions comparable to Visual CESIL (Run / Stop / Debugger as inventoried).
- Debugger dialogue: Source with PC highlight, Variables, Accumulator, Speed, Data,
  Output; Run / Step / Stop / Reset / Quit.
- Wire stepping through existing `cesil-core` `ExecutionHooks` where practical.
- Keep native Qt conventions; do not require pixel-perfect Jacobs chrome.

### Starting point

Milestone 8 provides the main-window IDE without a Debug menu. The interpreter already
exposes optional [`ExecutionHooks`](../../src/core/runtime/Hooks.hpp) for before/after
instruction and break callbacks.

### Out of scope for Milestone 9

- New language features or CESIL “Plus”
- Packaging / installers
- Post-1.0 extras (IR visualisation, hardware CESIL, richer breakpoints beyond Jacobs)

### Implementation stages

To be detailed when work starts. Expect inventory confirmation from Stage 8 PROBE,
core stepping/state plumbing, then the Qt dialogue and menu wiring, then close-out.

---

## Done when

- Debugger dialogue covers the essential Visual CESIL debug workflow
- Debug menu actions open and drive that dialogue appropriately
- Automated and/or manual checks cover step/run/stop/reset and live accumulator/store
  display
- Milestone 10 can treat Debugger as in the `1.0.0` parity contract
