← [Language reference index](README.md)

# Source form

## Line layout

A code line has up to three logical fields:

```text
[LABEL]  INSTRUCTION  [OPERAND]
```

| Field | Role | Status |
|-------|------|--------|
| Label | Optional name for this instruction (jump target) | **specified** |
| Instruction | One of the fourteen mnemonics | **specified** |
| Operand | Immediate, store/label name, or quoted string (`PRINT` only), when required | **specified** |

Whitespace (spaces and tabs) separates fields. Fixed punch-card columns are historical;
this project’s **editor** uses visual columns **1 / 9 / 17** with tab width **8** as a
writing convention ([Milestone 3](../milestones/03-editor-ergonomics.md)). Parsing does
not require those columns. **specified** (project editing convention); classic coding
sheets are the historical inspiration.

Blank lines (spaces/tabs only) are ignored. **specified** (Visual CESIL 1.1 release
notes; blank lines ignored).

## Identifiers

Used for line labels and store names:

| Rule | Status |
|------|--------|
| Begin with a letter | **specified** |
| Continue with letters or digits | **specified** |
| Maximum length **six** characters | **specified** (classic / cesil.org / Wikipedia) |
| Case of mnemonics | Instructions are recognised case-insensitively | **specified** (practical dialect; matches common implementations) |
| Case of labels and store names | Whether `FOO` and `foo` are the same name | **open** (Stage 3; `probe-case-fold.ces`) |

## Numeric constants

| Rule | Status |
|------|--------|
| Signed constants (`+0`, `-1`) are accepted | **specified** (classic materials / Wikipedia) |
| Unsigned non-negative constants (`0`, `8`) are accepted | **specified** (Visual CESIL corpus / Stage 1 gate; see [`testdata/parity/PROBE.md`](../../testdata/parity/PROBE.md)) |
| Optional `+` on positive constants | **specified** (Jacobs 1.2 notes + corpus) |
| `-0` | Disallowed in Visual CESIL 1.2 notes | **Jacobs-observed** |

## Strings

`PRINT` takes a double-quoted string operand. Escape conventions beyond a basic
quoted payload are **open**.

## Comments

| Form | Status |
|------|--------|
| Line beginning with `(` — remainder of line is a comment (classic / Wikipedia) | **specified** |
| Line beginning with `*` in the **code** section — remainder of line is a comment (Visual CESIL examples) | **specified** (Stage 1 corpus gate) |
| `*` after `%` as end-of-data marker | **specified** (classic); must remain distinct from code-section `*` comments |

Both comment forms are accepted in this dialect. Stage 2 must implement `*` comments
without breaking data-section `*`. See [Program structure](program-structure.md).

## Mnemonics

The fourteen instruction spellings are listed under [Instructions](instructions.md).
Unknown mnemonics are compile-time errors. Extra tokens after a complete instruction
line are rejected in this project (**deliberate diverge** vs Jacobs, who often ignores
trailing junk — see [Diagnostics](diagnostics.md)).
