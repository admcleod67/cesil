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

Blank lines (spaces/tabs only) are ignored. **Jacobs-observed** (Visual CESIL 1.1 release
notes); treat as **specified** for this dialect.

## Identifiers

Used for line labels and store names:

| Rule | Status |
|------|--------|
| Begin with a letter | **specified** |
| Continue with letters or digits | **specified** |
| Maximum length **six** characters | **specified** (classic / cesil.org / Wikipedia) |
| Case of mnemonics | Instructions are recognised case-insensitively | **specified** (practical dialect; matches common implementations) |
| Case of labels and store names | Whether `FOO` and `foo` are the same name | **open** |

## Numeric constants

| Rule | Status |
|------|--------|
| Classic CESIL often writes signed constants (`+0`, `-1`) | **specified** (classic materials / Wikipedia) |
| Visual CESIL allows unsigned non-negative constants (`0`, `8`) and optional `+` | **Jacobs-observed** (examples; release notes allow `+`) |
| Whether unsigned forms are accepted in this dialect | **open** (lean toward accepting them for Visual CESIL example compatibility) |
| `-0` | Disallowed in Visual CESIL 1.2 notes | **Jacobs-observed** |

## Strings

`PRINT` takes a double-quoted string operand. Escape conventions beyond a basic
quoted payload are **open**.

## Comments

| Form | Status |
|------|--------|
| Line beginning with `(` — remainder of line is a comment (classic / Wikipedia) | **specified** (classic); this engine already recognises start-of-line `(` |
| Line beginning with `*` as a comment (Visual CESIL examples) | **Jacobs-observed**; conflicts with `*` as end-of-data — **open** for this dialect |
| Whether both forms will be accepted | **open** (needed for Visual CESIL example corpus compatibility) |

Do not invent a third comment syntax in this milestone.

## Mnemonics

The fourteen instruction spellings are listed under [Instructions](instructions.md).
Unknown mnemonics are compile-time errors. Extra tokens after a complete instruction
line are rejected in this project (**deliberate diverge** vs Jacobs, who often ignores
trailing junk — see [Diagnostics](diagnostics.md)).
