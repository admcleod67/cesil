← [Project milestones index](../milestones.md)

## Milestone 5 — CESIL language reference (planned)

This milestone publishes a project language reference for classic CESIL: the machine
model, source form, instructions, data section, and runtime rules this engine aims
to implement. It synthesises historical and modern sources, records conflicts and
open questions explicitly, and leaves implementation and Visual CESIL example
verification to [Milestone 6](06-language-parity.md).

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
conventions, multi-error diagnostics, and a small example/fixture set. There is no
single in-repo language reference yet. Behaviour that matters for 1.0 is currently
split across code, milestone docs, and informal knowledge of Jacobs / ICL CESIL.

Useful external sources (non-exhaustive):

- ICL CES Computer Studies materials and the
  [ICL-CES CESIL/BASIC reference card](https://iclces.uk/articles/icl_cesil_basic_reference_card.html)
  (scans of the original card pages)
- [cesil.org](https://cesil.org) — modern overview of classic CESIL architecture,
  structure, and the 14 instructions (exclude CESIL “Plus” extensions)
- Wikipedia's CESIL overview and classic examples
- Andrew Jacobs' Visual CESIL 2.0 documentation and example programs (compatibility
  corpus; probe for edges the card does not settle)

### Out of scope for Milestone 5

- Changing parser, validator, or interpreter behaviour
- Visual CESIL golden corpus or deep Catch2 parity suites (owned by Milestone 6)
- IDE layout, menus, or Errors-table presentation (owned by Milestones 2–3 and 8)
- CESIL “Plus” or other post-classic extensions
- Word-for-word reproduction of any vendor or third-party text
- Resolving every open question by probing Visual CESIL (that probe work belongs to
  Milestone 6; this milestone only catalogues what must be probed)

### Version

Documentation milestone. No required CLI/IDE version bump; the product may remain
`0.4.0` until Milestone 6. Assign `0.5.0` only if a docs checkpoint tag is wanted.

### Implementation stages

Ship as one documentation milestone. Suggested stages:

1. **Source inventory and status vocabulary** — List primary sources and how each is
   used (historical floor, modern restatement, Jacobs compatibility, project
   decision). Define status labels for every normative claim, for example:
   **specified**, **Jacobs-observed**, **open**, **deliberate diverge**.
2. **Draft the reference** — Write the language pages (machine model, source layout,
   identifiers, instructions, data section, runtime, diagnostics overview) under
   `docs/language/`, with citations and links back to milestones where this project
   has already decided (e.g. Milestone 4 diagnostic exceedances).
3. **Conflicts, open questions, and close-out** — Publish a conflict / open-questions
   table; link it from the milestones index and from Milestone 6; update README
   roadmap blurb; mark this milestone completed.

---

## Deliverables

### Language reference (`docs/language/`)

A short, navigable reference (index plus focused pages or clearly headed sections):

- **Machine model** — Single accumulator; named scalar variables; no user stack;
  no subroutines/functions in classic CESIL; non-interactive data via the program
  data section.
- **Source form** — Label / instruction / operand columns; optional label; operand
  forms (integer, name, quoted string for `PRINT`); whitespace and layout notes
  aligned with project editor columns where applicable.
- **Identifiers** — Alphanumeric labels/names, length limit, leading letter, case
  folding as adopted by this project.
- **Instructions** — The classic fourteen mnemonics with operand requirements and
  brief semantics (`LOAD`, `STORE`, `IN`, `ADD`, `SUBTRACT`, `MULTIPLY`, `DIVIDE`,
  `JUMP`, `JIZERO`, `JINEG`, `PRINT`, `OUT`, `LINE`, `HALT`).
- **Program structure** — Code then `%` data then `*`; sequential `IN` consumption;
  `HALT` / end-of-program termination.
- **Runtime** — Initial accumulator and store state; arithmetic and jump conditions;
  division by zero and data exhaustion as error *conditions* (exact wording may
  differ).
- **Diagnostics** — Compile-time vs runtime; pointer to Milestone 4 accumulation /
  ordering / limit rules; no requirement to clone Jacobs message text.
- **Integer range** — Document the historical 24-bit signed range cited by classic
  sources; mark enforcement in this engine / Visual CESIL as **open** until probed.

### Source roles and citations

- Prefer the ICL reference card and classic CES materials for the instruction set and
  coding-sheet shaped language.
- Use [cesil.org](https://cesil.org) as a clear modern restatement of classic CESIL
  only; explicitly exclude CESIL “Plus” from the 1.0 language.
- Use Wikipedia for orientation and well-known examples, not as the sole authority
  for contested edges.
- Use Visual CESIL docs and examples as the **compatibility corpus**, with
  Jacobs-only edges labelled **Jacobs-observed** until promoted to **specified** for
  this dialect.

### Status, conflicts, and open questions

- Every normative statement in the reference carries or inherits a status
  (**specified** / **Jacobs-observed** / **open** / **deliberate diverge**).
- A dedicated conflicts / open-questions page or section lists disagreements among
  ICL, cesil.org, Visual CESIL, and the current engine — including at least:
  undefined *variables* (never-stored names) vs undefined *labels*; overflow;
  identifier length and case; strictness of trailing junk / operand shapes;
  `PRINT` / `OUT` / `LINE` spacing.
- Do not close an **open** item by copying current `cesil-core` behaviour; Milestone 6
  probes and then updates both the matrix and the reference.

### Project wiring

- Link the language reference from [`docs/milestones.md`](../milestones.md) and the
  README roadmap.
- Retarget [Milestone 6](06-language-parity.md) so it consumes this reference: verify
  and fix the engine against the settled rules, run the Visual CESIL example corpus,
  and resolve **open** / **Jacobs-observed** entries.
- Keep deliberate Milestone 4 diagnostic exceedances documented as **deliberate
  diverge** where they remain in force.

### Verification

This milestone is documentation-first. Acceptance is editorial and structural:

- `docs/language/` is complete enough that a reader can write and reason about classic
  CESIL programs without reading the C++ sources.
- Status labels are applied consistently; open questions are listed, not buried.
- CESIL “Plus” and other extensions are clearly out of scope for 1.0.
- Milestone 6's starting point and deliverables refer to this reference instead of
  redefining the language from scratch.
- No requirement for new automated tests in this milestone (tests land with
  Milestone 6 once behaviour is settled).

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
