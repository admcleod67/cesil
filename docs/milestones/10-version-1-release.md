← [Project milestones index](../milestones.md)

## Milestone 10 — Visual CESIL parity and 1.0 release (planned)

This milestone integrates and validates the language and IDE parity work, resolves
cross-platform release blockers, and publishes the first stable release. It is a
release-hardening milestone, not an opportunity to add features beyond Visual CESIL.

**Release checkpoint:** `1.0.0`.

It complements:

- [Project milestones](../milestones.md)
- [Milestone 6 — Visual CESIL language parity](06-language-parity.md)
- [Milestone 8 — Visual CESIL IDE parity](08-ide-parity.md)

### Version 1.0.0 contract

Version `1.0.0` is a cross-platform recreation of Visual CESIL 2.0:

- Visual CESIL programs within the documented language scope parse and execute with
  matching user-visible behaviour.
- The IDE provides the same essential single-program edit → check → run workflow and
  information.
- The CLI and IDE use the same tested `cesil-core` implementation.
- macOS, Linux, and Windows builds pass the agreed parity checks.

The contract does not require the original implementation technology, historical
Windows decoration, or undocumented bugs that are not needed to run compatible
programs.

### Out of scope for Milestone 10

- New language instructions or extensions
- Project / workspace support
- Richer debugging or visualisation beyond the established parity contract
- Refactors that do not remove a release blocker
- Post-1.0 experiments such as IR visualisation or a hardware CESIL CPU

---

## Deliverables

### Integrated parity validation

- Run the complete language fixture corpus and IDE workflow checklist.
- Resolve every remaining compatibility-matrix gap in the 1.0 scope.
- Verify that CLI and IDE output and error conditions remain consistent.
- Record the Visual CESIL reference version and the final compatibility scope.

### Cross-platform validation

- Configure, build, and test the core, CLI, and IDE on macOS, Linux, and Windows.
- Validate file dialogs, paths, line endings, fonts, menus, shortcuts, and process
  exit behaviour on each host.
- Document supported compilers, CMake, and Qt versions.
- Ensure a clean build does not depend on developer-machine paths or untracked
  assets.

### User documentation

- Provide concise build and run instructions for each supported platform.
- Document the CESIL language and the Visual CESIL compatibility contract.
- Include representative example programs and expected output.
- Record any platform presentation differences that do not affect parity.
- Update the milestone index and README to identify `1.0.0` as the stable parity
  release.

### Release preparation

- Set `PROJECT_VERSION` to `1.0.0`; confirm `cesil --version` and the Qt application
  version agree.
- Run the full automated test suite and manual IDE checklist from clean builds.
- Prepare release notes summarising parity scope, supported platforms, and known
  limitations.
- Create annotated tag `v1.0.0` only after all release criteria pass.

---

## Done when

- Milestones 6 and 8 are complete and their compatibility matrices contain no gaps
  in the agreed 1.0 scope.
- All automated tests pass from clean macOS, Linux, and Windows builds.
- The manual IDE parity checklist passes on all supported platforms.
- User and compatibility documentation describe the shipped behaviour accurately.
- The CLI and IDE report `1.0.0`.
- The `v1.0.0` release notes and annotated tag are ready to publish.

## After 1.0.0

Improvements beyond Andrew Jacobs' Visual CESIL begin after this release. They should
be planned as explicit extensions so compatibility remains testable and users can
distinguish faithful CESIL behaviour from new capabilities.
