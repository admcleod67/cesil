# Project milestones

This document is an **orienting roadmap**. Priorities and scope may change as the
CESIL core, CLI, and IDE evolve toward Visual CESIL parity on macOS, Linux, and
Windows.

## Milestone index

Detailed scope for each milestone is in [`docs/milestones/`](milestones/) (two-digit
prefixes sort lexicographically).

- [Milestone 0 — Core library and CLI (completed)](milestones/00-core-and-cli.md)
- [Milestone 1 — IDE edit → check → run (completed)](milestones/01-ide-mvp.md)
- [Milestone 2 — Diagnostic workflow (completed)](milestones/02-diagnostic-workflow.md)
- [Milestone 3 — Editor ergonomics (completed)](milestones/03-editor-ergonomics.md)
- [Milestone 4 — Diagnostic accumulation and parser recovery (completed)](milestones/04-diagnostic-accumulation.md)
- [Milestone 5 — CESIL language reference (completed)](milestones/05-language-reference.md)
- [Milestone 6 — Visual CESIL language parity (completed)](milestones/06-language-parity.md)
- [Milestone 8 — Visual CESIL IDE parity (planned landmark)](milestones/08-ide-parity.md)
- [Milestone 10 — Visual CESIL parity and 1.0 release (planned landmark)](milestones/10-version-1-release.md)

**Release checkpoint:** Milestone 6 is complete. The CLI and IDE report `0.6.0`
(`${PROJECT_VERSION}` from CMake). Cut git tag `v0.6.0` when ready.

**Next up:** Milestone 8 (main-window IDE parity; staged — see
[`milestones/08-ide-parity.md`](milestones/08-ide-parity.md)). Debugger dialogue is
planned as Milestone 9 before `1.0.0`.

Language reference: **[`docs/language/`](language/README.md)**.

## Path to version 1.0.0

Version `1.0.0` means parity with Andrew Jacobs' Visual CESIL 2.0 in two
user-visible areas:

- CESIL source accepted by Visual CESIL has the same documented meaning, output,
  and error conditions in this project.
- The Qt application provides the same essential edit → check → run workflow and
  presents the same useful information, while following native conventions on
  macOS, Linux, and Windows.

Parity is behavioural rather than an attempt to reproduce Visual CESIL's original
Java or C# implementation or its Windows-specific appearance pixel for pixel.
Compatibility findings and reference cases will be captured as tests and milestone
documentation instead of relying on memory.

The planned landmarks are:

- **Milestone 2 / `0.2.0`:** diagnostic presentation and source navigation (completed)
- **Milestone 3 / `0.3.0`:** editor commands and indentation (completed)
- **Milestone 4 / `0.4.0`:** diagnostic accumulation and parser recovery (completed)
- **Milestone 5:** CESIL language reference (completed; documentation; no version bump)
- **Milestone 6 / `0.6.0`:** language and runtime parity (engine + golden corpus) (completed)
- **Milestone 8:** main-window IDE workflow and presentation parity (planned; About in;
  Debugger out)
- **Milestone 9:** Visual CESIL Debugger dialogue (planned; before `1.0.0`)
- **Milestone 10 / `1.0.0`:** integrated parity validation and release

Milestone 7 remains deliberately unallocated. Milestone 9 is reserved for the Debugger
once Milestone 8's main-window scope is underway or complete. Other spare slots can
still absorb smaller increments as compatibility research reveals the work. Pre-1.0
release versions are assigned when those milestones are defined.

Features that deliberately extend Visual CESIL belong after `1.0.0`. This includes
new language features, IR visualisation, and hardware-oriented experiments unless a
reference audit establishes that they are required for parity. The Visual CESIL
Debugger dialogue is planned **before** `1.0.0` (Milestone 9), not as a post-1.0
extra.

## Testing strategy

Automated tests grow with each milestone rather than through a separate coverage
project. Today the suite is intentionally light:

- **Core:** Catch2 smoke coverage for lexer, parser, a few semantic cases, and a
  small runtime/integration path (including the classic sum example).
- **IDE:** Qt Test coverage for Milestone 2 diagnostic-model helpers and Milestone 3
  Edit actions, layout column math, and Tab/Shift-Tab indent behaviour. MainWindow
  workflows remain largely manual.

That baseline is enough to protect basic parse/run and Errors-table helpers. It is
not enough to pin Visual CESIL behaviour or full IDE workflows. Expanding tests in
isolation risks inventing cases that later parity work will redefine.

Planned growth:

- **Milestone 3:** focused automated tests for Edit actions and Tab/Shift-Tab
  behaviour, plus a short manual checklist for native menus and shortcuts
  (completed).
- **Milestone 4:** multi-error recovery, cascade suppression, ordering, and
  diagnostic-limit tests in the core suite, with CLI/IDE checks that the full set
  is presented (completed).
- **Milestone 5:** documentation-first language reference and open-questions list;
  no new automated language suite required (completed).
- **Milestone 6:** the deep layer — a Visual CESIL golden corpus and per-instruction
  / edge-case regression suite that makes language parity testable against the
  Milestone 5 reference (completed).
- **Milestone 8:** main-window IDE parity — inventory/matrix under `testdata/ide/`,
  then workflow/Errors and shell chrome (including About); automate stable state
  transitions; keep fragile visual checks manual (planned).
- **Milestone 9:** Debugger dialogue parity and stepping coverage (planned; before
  `1.0.0`).
- **Milestone 10:** cross-platform release checklists; automate stable state
  transitions where practical, keep fragile visual checks manual.

Pause for a dedicated coverage pass only if regressions appear that current tests
miss. Otherwise keep next work as Milestone 8 Stage 1 and grow tests with the feature.
