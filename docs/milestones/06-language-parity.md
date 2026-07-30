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
- [Milestone 8 — Visual CESIL IDE parity](08-ide-parity.md)

### Goals

- Define the documented Visual CESIL language and runtime behaviour precisely.
- Run Visual CESIL examples with matching control flow and program output.
- Turn reference behaviour into automated regression tests.
- Record compatibility findings so later IDE work rests on a stable engine.

### Starting point

Milestone 1 provides all documented instruction opcodes, parsing and validation,
case-normalised identifiers, a data section, structured diagnostics, and an
interpreter used by both the CLI and IDE. Existing behaviour must be audited before
being treated as a gap.

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

- Add end-to-end fixtures for Visual CESIL example programs.
- Compare captured program output byte for byte after normalising only host-native
  line endings where required.
- Add focused tests for every instruction and each established edge case.
- Add negative fixtures for parser, semantic, and runtime failures.
- Exercise the same core behaviour through the CLI-facing execution path.

---

## Done when

- The compatibility matrix has no **not yet established** entries for the documented
  Visual CESIL language.
- Every documented instruction and operand form is covered by an automated test.
- The reference example corpus produces matching output.
- Established syntax, semantic, and runtime error conditions are covered by tests.
- There are no known behavioural gaps within the documented Visual CESIL 2.0
  language scope.
