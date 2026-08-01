# CESIL language reference

This directory is the project’s description of **classic CESIL** as this engine aims
to implement it for the 1.0 path. It is not a copy of any vendor manual.

**Milestone:** [Milestone 5 — CESIL language reference](../milestones/05-language-reference.md)  
**Parity work:** [Milestone 6 — Visual CESIL language parity](../milestones/06-language-parity.md)

## How to read status labels

Normative claims use one of these statuses:

| Status | Meaning |
|--------|---------|
| **specified** | Settled for this project’s dialect (classic sources and/or an explicit project decision). |
| **Jacobs-observed** | Seen in Visual CESIL materials or examples; not yet promoted to **specified**. |
| **open** | Contested, unprobed, or deliberately unsettled. Do not treat current `cesil-core` behaviour as the rule. |
| **deliberate diverge** | This project knowingly differs from Jacobs or a classic source; documented on purpose. |

Where a whole section is mostly settled, individual bullets still call out **open** or
**Jacobs-observed** edges.

## Pages

| Page | Contents |
|------|----------|
| [Sources and status](sources-and-status.md) | Primary sources and how each is used |
| [Machine model](machine-model.md) | Accumulator, stores, integers, non-interactive model |
| [Source form](source-form.md) | Columns, identifiers, constants, comments, mnemonics |
| [Instructions](instructions.md) | The classic fourteen instructions |
| [Program structure](program-structure.md) | Code, `%` data, `*`, `IN` consumption |
| [Runtime](runtime.md) | Start state, arithmetic, jumps, I/O, runtime errors |
| [Diagnostics](diagnostics.md) | Compile vs run; Milestone 4 accumulation rules |
| [Open questions](open-questions.md) | Conflicts and Milestone 6 probe list |
| [Compatibility corpus](compatibility-corpus.md) | Visual CESIL examples (local tree) |

## Out of scope for 1.0

- **CESIL “Plus”** and other unofficial extensions ([cesil.org](https://cesil.org))
- Interactive input, stacks, subroutines, character I/O beyond `PRINT` text
- Matching Jacobs diagnostic *wording* or IDE chrome

## Quick example

Classic total-until-negative pattern (also in [`examples/total.ces`](../../examples/total.ces)
and [Wikipedia](https://en.wikipedia.org/wiki/CESIL)):

```text
        LOAD    +0
LOOP    STORE   TOTAL
        IN
        JINEG   DONE
        ADD     TOTAL
        JUMP    LOOP

DONE    PRINT   "The total is: "
        LOAD    TOTAL
        OUT
        LINE
        HALT

%
1
2
3
-1
*
```
