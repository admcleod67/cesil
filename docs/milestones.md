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
- [Milestone 8 — Visual CESIL IDE parity (completed)](milestones/08-ide-parity.md)
- [Milestone 9 — Visual CESIL Debugger (planned)](milestones/09-debugger.md)
- [Milestone 10 — Visual CESIL parity and 1.0 release (planned landmark)](milestones/10-version-1-release.md)

**Release checkpoint:** Milestone 8 is complete. The CLI and IDE report `0.8.0`
(`${PROJECT_VERSION}` from CMake). Cut git tag `v0.8.0` when ready.

**Next up:** Milestone 9 (Visual CESIL Debugger — staged; see
[`milestones/09-debugger.md`](milestones/09-debugger.md)). Expected checkpoint `0.9.0`.

Language reference: **[`docs/language/`](language/README.md)**.

## Path to version 1.0.0

Version `1.0.0` means parity with Andrew Jacobs' Visual CESIL 2.0 in two
user-visible areas:

- CESIL source accepted by Visual CESIL has the same documented meaning, output,
  and error conditions in this project.
- The Qt application provides the same essential edit → check → run workflow and
  presents the same useful information, while following native conventions on
  macOS, Linux, and Windows — including the Debugger dialogue (Milestone 9).

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
- **Milestone 8 / `0.8.0`:** main-window IDE workflow and presentation parity (completed)
- **Milestone 9 / `0.9.0`:** Visual CESIL Debugger dialogue (planned; staged —
  probe → core stepping → UI → close-out)
- **Milestone 10 / `1.0.0`:** integrated parity validation and release

Milestone 7 remains deliberately unallocated. Pre-1.0 release versions for Milestone 9
are assigned when that work is staged.

Features that deliberately extend Visual CESIL belong after `1.0.0`. This includes
new language features, IR visualisation, and hardware-oriented experiments unless a
reference audit establishes that they are required for parity. The Visual CESIL
Debugger dialogue is planned **before** `1.0.0` (Milestone 9), not as a post-1.0
extra.

## Testing strategy

Automated tests grow with each milestone rather than through a separate coverage
project. Today the suite is intentionally light:

- **Core:** Catch2 smoke coverage for lexer, parser, semantic/diagnostic cases, and
  runtime/golden parity paths.
- **IDE:** Qt Test coverage for diagnostic helpers, Edit actions, layout/indent, and
  compilation status summaries. MainWindow workflows use a manual checklist under
  [`testdata/ide/CHECKLIST.md`](../testdata/ide/CHECKLIST.md).

Planned growth:

- **Milestone 8:** main-window IDE parity — inventory, workflow/Errors, shell chrome,
  manual checklist (completed).
- **Milestone 9:** Debugger dialogue — probe/matrix, core stepping API, Qt Debug
  menu and dialogue, manual checklist; expected `0.9.0` (planned).
- **Milestone 10:** cross-platform release checklists; automate stable state
  transitions where practical, keep fragile visual checks manual.

Pause for a dedicated coverage pass only if regressions appear that current tests
miss. Otherwise keep next work as Milestone 9 and grow tests with the feature.
