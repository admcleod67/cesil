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
- [Milestone 3 — Editor ergonomics (planned)](milestones/03-editor-ergonomics.md)
- [Milestone 6 — Visual CESIL language parity (planned landmark)](milestones/06-language-parity.md)
- [Milestone 8 — Visual CESIL IDE parity (planned landmark)](milestones/08-ide-parity.md)
- [Milestone 10 — Visual CESIL parity and 1.0 release (planned landmark)](milestones/10-version-1-release.md)

**Release checkpoint:** Milestone 2 is complete. The CLI and IDE report `0.2.0`
(`${PROJECT_VERSION}` from CMake). Cut git tag `v0.2.0` when ready.

**Next up:** Milestone 3 establishes baseline editor commands and CESIL-aware
indentation.

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
- **Milestone 3 / `0.3.0`:** editor commands and indentation
- **Milestone 6:** language and runtime parity
- **Milestone 8:** IDE workflow and presentation parity
- **Milestone 10 / `1.0.0`:** integrated parity validation and release

Milestones 4, 5, 7, and 9 remain deliberately unallocated. They can be used to turn
the later thought documents into smaller, testable increments as compatibility
research reveals the actual work. Their pre-1.0 release versions will be assigned
when those milestones are defined.

Features that deliberately extend Visual CESIL belong after `1.0.0`. This includes
new language features, richer debugging, IR visualisation, and hardware-oriented
experiments unless a reference audit establishes that they are required for parity.
