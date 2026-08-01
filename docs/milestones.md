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
- [Milestone 6 — Visual CESIL language parity (planned landmark)](milestones/06-language-parity.md)
- [Milestone 8 — Visual CESIL IDE parity (planned landmark)](milestones/08-ide-parity.md)
- [Milestone 10 — Visual CESIL parity and 1.0 release (planned landmark)](milestones/10-version-1-release.md)

**Release checkpoint:** Milestone 4 is complete. The CLI and IDE report `0.4.0`
(`${PROJECT_VERSION}` from CMake). Cut git tag `v0.4.0` when ready. Milestone 5
(language reference) is documentation-complete with no version bump.

**Next up:** Milestone 6 (language parity against
[`docs/language/`](language/README.md) and Visual CESIL examples).

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
- **Milestone 6:** language and runtime parity (engine + Visual CESIL corpus)
- **Milestone 8:** IDE workflow and presentation parity
- **Milestone 10 / `1.0.0`:** integrated parity validation and release

Milestones 7 and 9 remain deliberately unallocated. They can be used to turn later
thought documents into smaller, testable increments as compatibility research
reveals the actual work. Their pre-1.0 release versions will be assigned when those
milestones are defined.

Features that deliberately extend Visual CESIL belong after `1.0.0`. This includes
new language features, richer debugging, IR visualisation, and hardware-oriented
experiments unless a reference audit establishes that they are required for parity.

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
  Milestone 5 reference.
- **Milestones 8 and 10:** IDE workflow and cross-platform checklists; automate
  stable state transitions where practical, keep fragile visual checks manual.

Pause for a dedicated coverage pass only if regressions appear that current tests
miss. Otherwise keep next work as Milestone 6 and grow tests with the feature.
