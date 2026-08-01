← [Language reference index](README.md)

# Diagnostics

This page describes *conditions* and project policy. Message text is this project’s
own wording unless a golden test later pins a banner string.

## Compile-time vs runtime

| Class | When | Examples |
|-------|------|----------|
| Compile-time | Parse / validate before run | Unknown mnemonic, missing operand, bad operand shape, duplicate/undefined label, invalid data row |
| Runtime | During interpretation | `IN` past end of data, division by zero |

Any compile-time diagnostic ⇒ unsuccessful parse; IR must not run. **specified**

## Accumulation and ordering

From [Milestone 4](../milestones/04-diagnostic-accumulation.md):

| Rule | Status |
|------|--------|
| Multiple independent compile-time errors in one pass | **specified** |
| Line-oriented recovery after a bad instruction line | **specified** |
| Diagnostics finalized: source order, exact dedupe, cap **100** with suppression message | **specified** |
| Jacobs message text not cloned | **specified** |

## Deliberate divergences from Visual CESIL (diagnostics)

Documented in `testdata/diagnostics/` and the Milestone 4 probe log. **deliberate diverge**

| Topic | Jacobs (floor) | This project |
|-------|----------------|--------------|
| Trailing tokens after `LINE` / `HALT` | Often accepted | Diagnose unexpected trailing tokens |
| Bare `STORE` | May miss | Missing-operand error |
| `PRINT` / `STORE` wrong shapes | Often only flags bad `JUMP` | Shape errors for unquoted `PRINT`, numeric `STORE` |
| Unknown mnemonic then undefined jump | May stop after first | Report both when independently checkable |

Undefined *variables* remain **open** — not a Milestone 4 invent-a-diagnostic item.

## Runtime banners

Classic / Wikipedia cite:

- `*** PROGRAM REQUIRES MORE DATA ***`
- `*** DIVISION BY ZERO ***`

Matching these strings exactly is **open** until Milestone 6 golden tests decide;
the *conditions* are **specified**.
