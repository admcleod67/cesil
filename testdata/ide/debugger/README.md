# Debugger inventory (Milestone 9)

Probe and matrix for Visual CESIL 2.0 **Debugger** behaviour. Stage 1 locked the
scope; Stage 2 delivered the core stepping API (`step()`, cooperative
`shouldStop_` / `betweenInstructions_`, data/line accessors). Stage 3 adds the
Debug menu and dialogue UI.

See [Milestone 9](../../../docs/milestones/09-debugger.md) and the parent
[IDE inventory](../README.md).

**Jacobs Debugger is the oracle** for dialogue semantics. Do not invent stepping
rules from `cesil-ide` alone. Main-window Run remains a Milestone 8
**deliberate diverge** (top-level Run kept).

## Layout

| File | Role |
|------|------|
| [`PROBE.md`](PROBE.md) | Jacobs Debugger observations |
| [`MATRIX.md`](MATRIX.md) | Ours vs Jacobs → Stage 2 done / Stage 3 actions |

No Visual CESIL binary or copyrighted examples are vendored here. Use an external
2.0 distribution (see
[`docs/language/compatibility-corpus.md`](../../../docs/language/compatibility-corpus.md)).
Do not commit absolute host paths.

## Probe procedure

1. Prefer **Visual CESIL 2.0** on Windows: Debug → Debugger; exercise Run / Step /
   Stop / Reset / Quit and Speed.
2. Record enablement, Output/Data behaviour, and Speed effect in [`PROBE.md`](PROBE.md).
3. Update [`MATRIX.md`](MATRIX.md) classifications and Stage 2/3 lists.
