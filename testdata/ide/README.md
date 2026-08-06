# IDE parity inventory (Milestone 8)

Main-window UI inventory and close-out artefacts for Visual CESIL 2.0 parity
(Stages 1–4). Checkpoint **`0.8.0`**.

See [Milestone 8](../../docs/milestones/08-ide-parity.md), [`PROBE.md`](PROBE.md),
[`MATRIX.md`](MATRIX.md), and [`CHECKLIST.md`](CHECKLIST.md).

Diagnostic *message text* remains this project’s own wording
([Milestone 4](../../docs/milestones/04-diagnostic-accumulation.md)).

## Layout

| File | Role |
|------|------|
| [`PROBE.md`](PROBE.md) | Jacobs observations and evidence |
| [`MATRIX.md`](MATRIX.md) | Ours vs Jacobs → status / deferrals |
| [`CHECKLIST.md`](CHECKLIST.md) | Manual cross-platform parity checklist (Stage 4) |

No copyrighted Jacobs `.ces` trees or the Visual CESIL binary are vendored here.
Use an external Visual CESIL 2.0 distribution (see
[`docs/language/compatibility-corpus.md`](../../docs/language/compatibility-corpus.md)).

## Probe history

Stage 1 evidence (2026-08): screenshots, UTF-16 UI strings from a local Visual CESIL
2.0 binary (not vendored), diagnostic-status probes. Stage 2 aligned compilation
status helpers. Stage 3 added Help → About, Compile, monospace fonts. Stage 4
closed the milestone at `0.8.0` with this checklist.

Do not commit absolute host paths to the local distribution.

## Scope

- **Milestone 8 (done):** main window — Source / Errors / Output, Compile and Run
  workflows, Help → About, status and document lifecycle.
- **Milestone 9:** Debugger dialogue and Debug-menu actions — Stage 1 inventory under
  [`debugger/`](debugger/).
