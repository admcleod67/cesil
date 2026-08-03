← [Project milestones index](../milestones.md)

## Milestone 6 — Visual CESIL language parity (planned)

This milestone brings the CESIL parser and runtime into line with the project
[language reference](05-language-reference.md) and with Andrew Jacobs' Visual CESIL
2.0 example corpus. The engine already implements the classic instruction set; the
work is to resolve open questions, close gaps, and pin behaviour with tests rather
than reimplement every instruction.

It complements:

- [Project milestones](../milestones.md)
- [Milestone 1 — IDE edit → check → run](01-ide-mvp.md)
- [Milestone 4 — Diagnostic accumulation and parser recovery](04-diagnostic-accumulation.md)
- [Milestone 5 — CESIL language reference](05-language-reference.md)
- [Milestone 8 — Visual CESIL IDE parity](08-ide-parity.md)

### Goals

- Verify the implementation against the Milestone 5 language reference.
- Run Visual CESIL examples with matching control flow and program output.
- Resolve **open** / **Jacobs-observed** items from the reference via probe, then
  update the reference status and the compatibility matrix.
- Turn settled behaviour into automated regression tests.
- Record compatibility findings so later IDE work rests on a stable engine.

### Starting point

Milestones 1–4 provide all documented instruction opcodes, parsing and validation,
case-normalised identifiers, a data section, structured multi-error diagnostics, and
an interpreter used by both the CLI and IDE. Milestone 5 provides the written
language reference under [`docs/language/`](../language/README.md), source citations,
and the [open-questions](../language/open-questions.md) list. Existing behaviour must
be audited against that reference before being treated as a gap.

Core tests before this milestone remain mostly smoke-level plus multi-error recovery
from Milestone 4. This milestone is where coverage becomes deep: the language
reference is the specification; Visual CESIL is the example-compatibility oracle
(local corpus notes in
[`compatibility-corpus.md`](../language/compatibility-corpus.md)); the golden corpus
pins language and runtime behaviour.

### Out of scope for Milestone 6

- Authoring the language reference from scratch (owned by Milestone 5)
- IDE layout or error-panel presentation
- Pixel-level reproduction of Visual CESIL
- New instructions or language extensions not in the Milestone 5 classic scope
  (including CESIL “Plus”)
- Rich debugger and stepping UI
- Packaging / installers

### Implementation stages

Ship as one milestone and one pre-1.0 checkpoint (version assigned at close-out).
Work lands in ordered stages. Probe results must settle dialect rules before large
engine or golden-suite changes — several
[open questions](../language/open-questions.md) (especially `*` comments, unsigned
constants, and optional trailing `*`) block running the Visual CESIL example corpus.

1. **Corpus gate and dialect probes** (done) — In-repo gate fixtures and probe log live
   under [`testdata/parity/`](../../testdata/parity/). Evidence for Q4–Q6 comes from the
   Visual CESIL 2.0 distribution examples / `Release Notes.htm` and a **live Check** of
   the gate fixtures (2026-08-03; see [`PROBE.md`](../../testdata/parity/PROBE.md) and
   [`MATRIX.md`](../../testdata/parity/MATRIX.md)). Notable live finding: Jacobs rejects
   classic `(` comments (this dialect keeps them as a deliberate diverge) while accepting
   `*` comments, unsigned constants, and EOF-or-`*` data termination. Language-reference
   statuses for the gate items were updated so Stage 2 has an implement list. No engine
   changes in this stage.
2. **Parser and source compatibility** (next) — Implement the settled source-form rules so
   the Visual CESIL examples compile (comments, constants, data terminator, and any
   related case/identifier decisions from stage 1). Smoke that the example set parses;
   keep Milestone 4 deliberate diagnostic divergences unless a probe forces a rethink.
3. **Runtime semantics and matrix** — Probe and settle remaining open questions
   (undefined variables, identifier case, overflow, `DIVIDE` rounding, I/O bytes,
   runtime banners, fall-off without `HALT`, and similar). Correct interpreter gaps;
   keep the compatibility matrix and language-reference statuses current.
4. **Golden suite and close-out** — End-to-end fixtures from the Visual CESIL corpus
   with output compared after normalising host line endings where required; focused
   per-instruction and edge-case tests; negative fixtures; CLI-facing path coverage.
   Defer any leftover **open** items explicitly post-1.0 or mark them **specified** /
   **deliberate diverge**. Assign the release checkpoint version and mark the
   milestone completed.

Stage 2 depends on stage 1. Stage 3 can overlap lightly with late stage 2 once
examples parse, but runtime golden expectations should wait for stage 3 probes.
Stage 4 depends on settled matrix entries from stages 1–3.

**Out of scope reminder:** undefined *variables* (never-stored names) remain a probe
item (stage 3), distinct from undefined *labels*. Do not add a compile-time undefined
variable diagnostic from fixtures alone.

---

## Deliverables

### Compatibility matrix against the language reference

- Consume [`docs/language/`](../language/README.md) and its
  [open-questions](../language/open-questions.md) / conflicts table.
- Collect the Visual CESIL 2.0 documentation and example programs used as the
  compatibility corpus (see
  [`compatibility-corpus.md`](../language/compatibility-corpus.md) when online
  docs are unavailable).
- Record each documented lexical rule, instruction, operand form, and runtime edge
  case as **match**, **gap**, or **not yet established** (and promote
  **Jacobs-observed** / **open** reference statuses as probes complete).
- Capture expected output and error conditions from the reference implementation.
- Keep uncertain or undocumented behaviour explicit; do not infer it from the
  current implementation. Update `docs/language/` when a probe settles a rule.

### Instruction and machine semantics

Verify and, where necessary, correct:

- `LOAD`, `STORE`, `IN`, `ADD`, `SUBTRACT`, `MULTIPLY`, and `DIVIDE`
- `JUMP`, `JIZERO`, and `JINEG`
- `PRINT` with its documented quoted-string operand
- `OUT`, `LINE`, and `HALT`
- Initial accumulator and variable-store state
- Literal and variable resolution, identifier case, and negative integers
- Integer range / overflow and division semantics

#### Undefined variables and identifier case

Do **not** confuse undefined *labels* (compile-time errors in Visual CESIL and in
the Milestone 4 fixtures) with undefined *variables* (never-stored names).

Live evidence ([`testdata/parity/PROBE.md`](../../testdata/parity/PROBE.md)):

- [`probe-unset-var.ces`](../../testdata/parity/probe-unset-var.ces): **no compile error**
  for `LOAD UNSET`.
- Distinctive case probe (`LOAD +42` / `STORE Foo` / `LOAD FOO` / `OUT`): output **`0`**
  — store names are **case-sensitive**, and the unset name `FOO` reads as zero.

Do not invent a compile-time undefined-variable diagnostic. Preserve identifier case in
the store (and likely labels). Mnemonics remain case-insensitive separately.

See also Milestone 4 [`PROBE.md`](../../testdata/diagnostics/PROBE.md) open questions.

### Source, data, and labels

Verify and, where necessary, correct:

- Code/data separation and `%` data-section syntax
- Sequential `IN` consumption and data-exhaustion behaviour
- Integer parsing in the data section
- Case-sensitive labels and variables (Jacobs store names; labels assumed same)
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

- The Milestone 5 language reference has no remaining **open** items that block the
  documented classic scope (or each leftover is explicitly deferred post-1.0).
- The compatibility matrix has no **not yet established** entries for that settled
  scope, including Visual CESIL example compatibility.
- Every documented instruction and operand form is covered by an automated test.
- The reference example corpus produces matching output.
- Established syntax, semantic, and runtime error conditions are covered by tests.
- There are no known behavioural gaps within the documented classic CESIL scope that
  Milestone 5 and this milestone agreed to cover for 1.0.
