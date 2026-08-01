← [Project milestones index](../milestones.md)

## Milestone 6 — Visual CESIL language parity (planned)

This milestone brings the CESIL parser and runtime into behavioural parity with
Andrew Jacobs' Visual CESIL 2.0. The current engine already implements the classic
instruction set, so the work is to establish a reliable reference baseline, verify
the implementation, and close observed gaps rather than reimplement every
instruction.

It complements:

- [Project milestones](../milestones.md)
- [Milestone 1 — IDE edit → check → run](01-ide-mvp.md)
- [Milestone 4 — Diagnostic accumulation and parser recovery](04-diagnostic-accumulation.md)
- [Milestone 8 — Visual CESIL IDE parity](08-ide-parity.md)

### Goals

- Define the documented Visual CESIL language and runtime behaviour precisely.
- Run Visual CESIL examples with matching control flow and program output.
- Turn reference behaviour into automated regression tests.
- Record compatibility findings so later IDE work rests on a stable engine.

### Starting point

Milestones 1–4 provide all documented instruction opcodes, parsing and validation,
case-normalised identifiers, a data section, structured multi-error diagnostics, and
an interpreter used by both the CLI and IDE. Existing behaviour must be audited
before being treated as a gap.

Core tests before this milestone remain mostly smoke-level plus multi-error recovery
from Milestone 4. This milestone is where coverage becomes deep: Visual CESIL is the
oracle, and the golden corpus pins language and runtime behaviour.

### Out of scope for Milestone 6

- IDE layout or error-panel presentation
- Pixel-level reproduction of Visual CESIL
- New instructions or language extensions not supported by Visual CESIL 2.0
- Rich debugger and stepping UI
- Packaging / installers

---

## Deliverables

### Reference baseline and compatibility matrix

- Collect the Visual CESIL 2.0 documentation and example programs used as the
  compatibility reference.
- Record each documented lexical rule, instruction, operand form, and runtime edge
  case as **match**, **gap**, or **not yet established**.
- Capture expected output and error conditions from the reference implementation.
- Keep uncertain or undocumented behaviour explicit; do not infer it from the
  current implementation.

### Instruction and machine semantics

Verify and, where necessary, correct:

- `LOAD`, `STORE`, `IN`, `ADD`, `SUBTRACT`, `MULTIPLY`, and `DIVIDE`
- `JUMP`, `JIZERO`, and `JINEG`
- `PRINT` with its documented quoted-string operand
- `OUT`, `LINE`, and `HALT`
- Initial accumulator and variable-store state
- Literal and variable resolution, identifier case, and negative integers
- Integer range / overflow and division semantics

#### Undefined variables (explicit probe)

Do **not** confuse undefined *labels* (compile-time errors in Visual CESIL and in
the Milestone 4 fixtures) with undefined *variables* (never-stored names). Classic
CESIL often allows `LOAD` / `ADD` / … of an unset name and reads it as `0`; this
project's interpreter currently does the same. Treat the rule as **not yet
established** until Visual CESIL is probed, for example:

- `LOAD UNSET` then `OUT` with no prior `STORE` — compile success? output `0`?
- `LOAD +0` then `ADD UNSET` — same questions
- `STORE Foo` then `LOAD FOO` — case folding of store names

Only after that probe should the compatibility matrix mark “unset variable” as
match, gap, or deliberate post-1.0 extension. Milestone 4 must not invent a
compile-time undefined-variable diagnostic from the diagnostic fixtures alone
(see [`PROBE.md`](../../testdata/diagnostics/PROBE.md) open questions).

### Source, data, and labels

Verify and, where necessary, correct:

- Code/data separation and `%` data-section syntax
- Sequential `IN` consumption and data-exhaustion behaviour
- Integer parsing in the data section
- Case-insensitive labels and variables
- Forward label references
- Duplicate and missing label diagnostics
- Comment, whitespace, and column-layout rules

### Output and errors

- Match Visual CESIL's spacing and line-ending behaviour for `PRINT`, `OUT`, and
  `LINE`.
- Match runtime error conditions such as division by zero and exhausted data.
- Match syntax and semantic error conditions for invalid instructions, operands,
  labels, and data.
- Separate diagnostic meaning from IDE presentation; Milestone 8 owns the visual
  error-panel layout.

### Parity regression suite

This is the project's main language-coverage expansion (see the testing strategy in
the [project milestones index](../milestones.md)). Prefer golden fixtures derived
from Visual CESIL over inventing independent edge cases ahead of the audit.

- Add end-to-end fixtures for Visual CESIL example programs.
- Compare captured program output byte for byte after normalising only host-native
  line endings where required.
- Add focused tests for every instruction and each established edge case
  (`OUT` formatting, divide-by-zero, `IN` exhaustion, overflow, and similar).
- Add negative fixtures for parser, semantic, and runtime failures.
- Exercise the same core behaviour through the CLI-facing execution path.
- Record each matrix entry's covering test so gaps stay visible.

---

## Done when

- The compatibility matrix has no **not yet established** entries for the documented
  Visual CESIL language.
- Every documented instruction and operand form is covered by an automated test.
- The reference example corpus produces matching output.
- Established syntax, semantic, and runtime error conditions are covered by tests.
- There are no known behavioural gaps within the documented Visual CESIL 2.0
  language scope.
