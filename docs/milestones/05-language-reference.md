← [Project milestones index](../milestones.md)

## Milestone 5 — CESIL language reference (completed)

This milestone publishes a project language reference for classic CESIL: the machine
model, source form, instructions, data section, and runtime rules this engine aims
to implement. It synthesises historical and modern sources, records conflicts and
open questions explicitly, and leaves implementation and Visual CESIL example
verification to [Milestone 6](06-language-parity.md).

**Release note:** Milestone 5 is complete. Documentation only — CLI/IDE remain
`0.4.0` unless a separate docs checkpoint tag is cut. Language pages live under
[`docs/language/`](../language/README.md).

It complements:

- [Project milestones](../milestones.md)
- [Milestone 4 — Diagnostic accumulation and parser recovery](04-diagnostic-accumulation.md)
- [Milestone 6 — Visual CESIL language parity](06-language-parity.md)
- [Milestone 8 — Visual CESIL IDE parity](08-ide-parity.md)

### Goals

- Produce a durable `docs/language/` reference that describes *this* project's CESIL
  dialect without relying on memory or scattered milestone notes.
- Ground the happy-path language in classic CESIL sources (ICL materials and clear
  modern restatements), not solely in Visual CESIL.
- Treat Visual CESIL as the **example-compatibility** target: programs that run there
  should be runnable here within the documented scope.
- Mark every contested or unprobed edge as open rather than inventing behaviour from
  the current implementation.
- Give Milestone 6 a settled specification and an explicit probe list.

### Starting point

Milestones 0–4 provide a working classic instruction set, column-oriented editing
conventions, multi-error diagnostics, and a small example/fixture set. There was no
single in-repo language reference before this milestone.

Useful external sources (non-exhaustive):

- ICL CES Computer Studies materials and the
  [ICL-CES CESIL/BASIC reference card](https://iclces.uk/articles/icl_cesil_basic_reference_card.html)
  (scans of the original card pages)
- [cesil.org](https://cesil.org) — modern overview of classic CESIL architecture,
  structure, and the 14 instructions (exclude CESIL “Plus” extensions)
- Wikipedia's CESIL overview and classic examples
- Andrew Jacobs' Visual CESIL 2.0 — online pages may be unavailable; example programs
  and release notes from a local 2.0 tree (see
  [`docs/language/compatibility-corpus.md`](../language/compatibility-corpus.md))

### Out of scope for Milestone 5

- Changing parser, validator, or interpreter behaviour
- Visual CESIL golden corpus or deep Catch2 parity suites (owned by Milestone 6)
- IDE layout, menus, or Errors-table presentation (owned by Milestones 2–3 and 8)
- CESIL “Plus” or other post-classic extensions
- Word-for-word reproduction of any vendor or third-party text
- Resolving every open question by probing Visual CESIL (that probe work belongs to
  Milestone 6; this milestone only catalogues what must be probed)

### Version

Documentation milestone. No required CLI/IDE version bump; the product remains
`0.4.0` until Milestone 6 (or until a docs-only `0.5.0` tag is chosen).

### Implementation stages

Shipped as one documentation milestone:

1. **Source inventory and status vocabulary** (done) — Primary sources and roles;
   status labels **specified** / **Jacobs-observed** / **open** /
   **deliberate diverge** in [`docs/language/`](../language/README.md).
2. **Draft the reference** (done) — Machine model, source form, instructions,
   program structure, runtime, and diagnostics pages with citations.
3. **Conflicts, open questions, and close-out** (done) — Probe checklist and
   conflict summary; compatibility corpus notes for the local Visual CESIL tree;
   milestone index / README / Milestone 6 wired to the reference.

---

## Deliverables

### Language reference (`docs/language/`)

Published under [`docs/language/README.md`](../language/README.md):

- Machine model, source form, identifiers, instructions, program structure,
  runtime, diagnostics overview
- Integer range documented as historical 24-bit with enforcement **open**
- Status labels on normative claims
- Open-questions / conflicts table for Milestone 6
- Compatibility corpus notes (local Visual CESIL examples; not vendored)

### Source roles and citations

- ICL reference card and classic CES materials for the instruction set shape
- [cesil.org](https://cesil.org) for classic CESIL only (CESIL “Plus” excluded)
- Wikipedia for orientation and the classic total example
- Visual CESIL as the **compatibility corpus** (examples + release notes)

### Project wiring

- Linked from [`docs/milestones.md`](../milestones.md) and the README roadmap
- [Milestone 6](06-language-parity.md) consumes this reference
- Milestone 4 diagnostic exceedances recorded as **deliberate diverge**

### Verification

- `docs/language/` is navigable without reading C++ sources
- Open questions are explicit; CESIL “Plus” is out of scope for 1.0
- No implementation changes in this milestone

---

## Done when

- A project CESIL language reference exists under `docs/language/` with citations to
  the primary sources above.
- Classic instruction set, source/data structure, and runtime outline are documented.
- Conflicts and open questions are explicit and ready for Milestone 6 probes.
- Visual CESIL is documented as the example-compatibility target, not the sole
  historical definition of CESIL.
- Milestone index / README / Milestone 6 point at the reference.
- No implementation changes were required to complete this milestone (fixes belong to
  Milestone 6).
