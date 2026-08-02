# Compatibility matrix stub (Milestone 6 Stage 1)

Gate topics only. Status values: **match** (this engine already agrees with settled
rule), **gap** (engine must change in Stage 2), **Jacobs-observed** / **specified**
(see language reference), **not yet established**.

Evidence: [`PROBE.md`](PROBE.md) (Visual CESIL 2.0 distribution examples + release notes).

| Topic | Q | Fixture(s) | Rule status | Engine vs rule | Stage 2 action |
|-------|---|------------|-------------|----------------|----------------|
| Unsigned numeric constants | Q4 | `gate-unsigned-constant.ces` | **specified** (accept) | **gap** if engine rejects unsigned forms; else **match** | Ensure unsigned literals parse in code and data |
| `*` full-line comments | Q5 | `gate-star-comment.ces` | **specified** (accept in code) | **gap** (today `*` is end-of-data token only) | Recognise start-of-line `*` comments without breaking data `*` |
| `(` line comments | Q5 | `gate-paren-comment.ces` | **specified** (classic; keep) | **match** if `(` already skipped | Preserve; confirm Jacobs with live Check if desired |
| Data ends at EOF (no `*`) | Q6 | `gate-data-no-star.ces` | **specified** (accept) | **gap** if engine requires trailing `*` | Allow EOF to end data section |
| Data ends with `*` | Q6 | `gate-data-with-star.ces` | **specified** (classic; keep) | **match** if already accepted | Keep working |

Broader matrix rows (instructions, runtime edges) land in later Milestone 6 stages.
