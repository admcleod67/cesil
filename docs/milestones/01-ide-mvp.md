← [Project milestones index](../milestones.md)

## Milestone 1 — IDE edit → check → run (completed)

This document defines **Milestone 1**: the first product-useful CESIL IDE — edit
source, check syntax, run programs, and surface diagnostics and output — plus shared
`0.1.0` version reporting for the IDE and CLI.

**Release note:** Milestone 1 is complete. The CLI and IDE report `0.1.0`
(`${PROJECT_VERSION}` from CMake). Cut git tag `v0.1.0` when ready.

It complements:

- [Project milestones](../milestones.md)
- [Milestone 0 — Core library and CLI](00-core-and-cli.md)
- Visual CESIL by Andrew Jacobs (UX north star for later milestones)

### Goals

- Prove the Qt shell + in-process `cesil-core` model (edit → check → run).
- Deliver a complete single-buffer teaching workflow with Errors and Output panels.
- Report a single project version from both `cesil` and `cesil-ide`.

### Starting point

Milestone 0 provides `cesil-core`, the CLI, tests, and examples. Milestone 1 adds the
optional Qt IDE (`-DCESIL_BUILD_IDE=ON`) on top of that library.

### Out of scope for Milestone 1

- Visual CESIL–style error panel restyle
- Syntax highlighting
- Stepping / visual debugger UI
- Packaging / installers

---

## Deliverables

### IDE shell

- Optional Qt 6 Widgets application `cesil-ide`
- Tabs: Source, Errors, Output
- File New / Open / Save / Save As (with unsaved-change prompts)
- Build → Check syntax (F7)
- Run → Run (F5), using the in-process interpreter and a Qt `IoHost`
- Fixed main toolbar: New, Open, Save | Check syntax, Run
- Status bar: action messages plus permanent Ln/Col indicator

### Version reporting

- Root CMake `project(cesil VERSION 0.1.0 …)`
- `CESIL_VERSION` compile definition on `cesil-ide` and `cesil-cli`
- IDE sets `QApplication` name, organisation, and version
- CLI supports `--version` / `-v` (prints `cesil 0.1.0`)

---

## Done when

- `cesil-ide` builds with `-DCESIL_BUILD_IDE=ON`
- Check syntax and Run populate Errors / Output as documented
- `cesil --version` and the IDE application version both report `0.1.0`
