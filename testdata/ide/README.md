# IDE parity inventory (Milestone 8 Stage 1)

Main-window UI inventory for Visual CESIL 2.0 parity. Stage 1 locks what
Stages 2–3 implement; it does **not** change the Qt IDE.

See [Milestone 8](../../docs/milestones/08-ide-parity.md), [`PROBE.md`](PROBE.md),
and [`MATRIX.md`](MATRIX.md).

**Do not treat current `cesil-ide` behaviour as the oracle** for unsettled rows.
Prefer Visual CESIL 2.0 (live UI, screenshots, or binary string evidence) for
Jacobs columns. Diagnostic *message text* remains this project’s own wording
([Milestone 4](../../docs/milestones/04-diagnostic-accumulation.md)).

## Layout

| File | Role |
|------|------|
| [`PROBE.md`](PROBE.md) | Jacobs observations and evidence |
| [`MATRIX.md`](MATRIX.md) | Ours vs Jacobs → Stage 2/3 action or defer |

No copyrighted Jacobs `.ces` trees or the Visual CESIL binary are vendored here.
Use an external Visual CESIL 2.0 distribution (see
[`docs/language/compatibility-corpus.md`](../../docs/language/compatibility-corpus.md)).

## Windows / reference probe

1. Prefer **Visual CESIL 2.0** on Windows for live Check / Run / menu inspection.
2. Record exact status strings, menu labels, and tab transitions in [`PROBE.md`](PROBE.md).
3. Update [`MATRIX.md`](MATRIX.md) classifications and Stage 2/3 actions.
4. Do not commit absolute host paths to the local distribution.

Stage 1 evidence (2026-08): live diagnostic/status probes, operator screenshots
(About, main window, Debugger), and UTF-16 UI string extraction from
`Visual CESIL.exe` in a local (non-repo) 2.0 tree. Stage 2 (2026-08) aligned
compilation status helpers and settled post-run status diverge. Stage 3 (2026-08)
added Help → About, Compile label, and monospace Source/Output fonts. See
[`PROBE.md`](PROBE.md) and [`MATRIX.md`](MATRIX.md).

## Scope reminder

- **In Milestone 8:** main window — Source / Errors / Output, Check/Compile and Run
  workflows, Help → About, status and document lifecycle.
- **Deferred to Milestone 9:** Debugger dialogue and Debug-menu actions that open or
  drive it (`Debugger`, in-debugger Step / Stop / Reset, live variables, speed).
