← [Language reference index](README.md)

# Sources and status

## Primary sources

| Source | Role | Notes |
|--------|------|--------|
| [ICL-CES CESIL/BASIC reference card](https://iclces.uk/articles/icl_cesil_basic_reference_card.html) | Historical floor for the instruction set and coding-sheet language | Page scans of the original CESIL card; prefer for mnemonic list and classic shape |
| [cesil.org](https://cesil.org) | Clear modern restatement of **classic** CESIL | Architecture, 14 instructions, 6-character labels, non-interactive data. **Ignore CESIL “Plus”** for 1.0 |
| [Wikipedia — CESIL](https://en.wikipedia.org/wiki/CESIL) | Orientation, instruction summaries, classic example | Useful footnotes on Visual CESIL differences; not sole authority on edges |
| Visual CESIL 2.0 (Andrew Jacobs) | **Example-compatibility** corpus | Online pages may be unavailable; local examples and `Release Notes.htm` under a Visual CESIL 2.0 tree (see [Compatibility corpus](compatibility-corpus.md)) |
| This repository | Project decisions already shipped | Milestone docs (especially [M4 diagnostics](../milestones/04-diagnostic-accumulation.md)), `examples/`, `testdata/diagnostics/` |

## Compatibility vs definition

- **Classic sources** (ICL card, cesil.org classic overview, Wikipedia structure) define what CESIL *is* for the happy path.
- **Visual CESIL** defines which *programs* we intend to run for parity: its examples and observed edges. Jacobs is not the only historical definition of CESIL.
- When sources conflict, the claim stays **open** (or is marked **deliberate diverge**) until [Milestone 6](../milestones/06-language-parity.md) probes and settles it.

## Project decisions already in force

These are **specified** or **deliberate diverge** elsewhere and are restated in the
reference pages:

- Multi-error compilation with line recovery, source-ordered diagnostics, dedupe, and
  a cap of 100 ([Milestone 4](../milestones/04-diagnostic-accumulation.md)).
- Stricter diagnostics than Jacobs on trailing junk, bare `STORE`, some operand
  shapes, and syntax-then-semantic pairs (`testdata/diagnostics/`).
- IDE source columns **1 / 9 / 17** with visual tab width **8**
  ([Milestone 3](../milestones/03-editor-ergonomics.md)) — editing convention; the
  parser accepts whitespace-delimited fields, not fixed punch columns only.

## What this reference is not

- A transcript of the ICL card or Jacobs help text
- A commitment that today’s `cesil-core` behaviour is correct for every **open** item
- A licence to implement CESIL “Plus” before 1.0
