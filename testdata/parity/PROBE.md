# Jacobs probe log (Milestone 6 Stage 1)

Corpus-gate probe for Visual CESIL source forms (Q4–Q6), plus early Stage 3 stub
and smoke observations. Treat results as behavioural evidence, not exact UI string
catalogs.

## Tool

| Field | Value |
|-------|--------|
| Tool | Visual CESIL 2.0 |
| Version / build date | 2.0 (Release Notes: 30-May-2010); earlier notes 24-May-2003 / 26-Mar-2003 |
| Host OS | Live Check/Run on Windows (operator); earlier corpus inspection also used the local Visual CESIL 2.0 tree on macOS |
| Operator / date | Live probes / 2026-08-03 (corpus inspection / 2026-08-02) |

## Method

1. Map each gate fixture to patterns in Jacobs’ shipped `.ces` examples or
   `Release Notes.htm`.
2. Confirm with **live Visual CESIL Check** (and Run where noted) on in-repo fixtures.
3. Record compile/run expectations for Stage 2 / Stage 3.
4. External checklist: `Example 1.ces`, `Example 2.ces` (not vendored in this repo).

## Per-file observations (gate fixtures)

| File | Compile (evidence) | Runtime notes | Implications |
|------|--------------------|---------------|--------------|
| `gate-unsigned-constant.ces` | **Accept** — live Check: no error. Also `Example 1.ces` / `Division.ces` use unsigned literals | Expect `OUT` of `8` then newline | **Q4:** unsigned non-negative constants are Jacobs-legal |
| `gate-star-comment.ces` | **Accept** — live Check: no error. Also Example 1/2 / `Division.ces` use leading `*` comments | N/A if comments ignored | **Q5:** `*` full-line comments in code are Jacobs-legal (distinct from end-of-data `*`) |
| `gate-paren-comment.ces` | **Reject** — live Check: error on L1, wording ~`Unrecognised instruction(s)` (Jacobs treats `(` as a bad mnemonic, not a comment) | N/A | **Q5:** Visual CESIL does **not** accept classic `(` comments. This dialect keeps `(` as a **deliberate diverge** (classic / Wikipedia) |
| `gate-data-no-star.ces` | **Accept** — live Check: no error. Also Example 1/2 end data at EOF with no trailing `*` | Examples are intended to run | **Q6:** EOF after data is Jacobs-legal |
| `gate-data-with-star.ces` | **Accept** — live Check: no error (trailing `*` not misread as a code comment after `%`) | _pending_ stdout if Run used | **Q6:** classic trailing `*` remains Jacobs-legal alongside EOF |
| `smoke-print-out.ces` | **Accept** — live Check: no error | Live Run output: `Hi1` (then newline from `LINE`) | **Q8 seed:** `PRINT` then `OUT` concatenate with **no** intervening space; single-digit `OUT` is bare `1` |

## Stage 3 stubs (early live Check / Run)

| File | Compile | Runtime / notes | Implications |
|------|---------|-----------------|--------------|
| `probe-unset-var.ces` | **Accept** — live Check: no error (`LOAD UNSET`) | Reinforced by case-fold Run: distinct unset name `FOO` read as `0` | **Q1:** never-stored names are **not** compile errors; unset reads as **0** at runtime (**Jacobs-observed**) |
| `probe-case-fold.ces` | **Accept** — live Check: no error | Live Run of distinctive probe (`LOAD +42` / `STORE Foo` / `LOAD FOO` / `OUT`): output **`0`** | **Q2 specified:** store names are **case-sensitive** (`Foo` ≠ `FOO`). Fixture updated to the `+42` form |

## External example checklist

| External file | Observations (from source text) | Q4–Q6 |
|---------------|----------------------------------|-------|
| `Example 1.ces` | Leading `*` comment lines; `LOAD 0` (unsigned); `%` then data `1`/`2`/`3`/`-1`; **no** trailing `*` | Q4, Q5, Q6 all exercised |
| `Example 2.ces` | Same comment and data-terminator pattern; unsigned data values | Q5, Q6 |

## Release-notes support

- **1.2 (24-May-2003):** positive numbers may be prefixed with `+` (`+0` allowed, `-0` not); blank lines ignored; `IN` past end errors instead of crash.
- **2.0 (30-May-2010):** variables initialised to zero at execution start.

## Stage 1 conclusions (gate)

| Q | Conclusion for this dialect (pending Stage 2 implementation where noted) |
|---|--------------------------------------------------------------------------|
| Q4 | **Accept unsigned** non-negative constants (and optional `+`) — Jacobs live Check + corpus |
| Q5 | **Accept `*` full-line comments** in the code section (Jacobs live Check + corpus). **Keep classic `(` comments** as a deliberate diverge — Jacobs rejects them. Data-section `*` remains the end-of-data marker |
| Q6 | **Accept EOF** as end of data (no trailing `*`); **keep** classic trailing `*` — both confirmed by live Check |

## Early Stage 3 notes (not Stage 1 blockers)

| Q | Status after 2026-08-03 live probes |
|---|-------------------------------------|
| Q1 | **Jacobs-observed:** compile accepts unset `LOAD`; runtime read of never-stored name is `0` (seen via `LOAD FOO` after `STORE Foo` with 42) |
| Q2 | **specified:** store names are case-sensitive (`STORE Foo` / `LOAD FOO` → `OUT` `0`, not `42`) |
| Q8 | **Jacobs-observed (partial):** `PRINT "Hi"` + `OUT` of `+1` → `Hi1` (no auto space). Wider golden formatting still open |

Live gate Check of the in-repo fixtures is recorded above. Stage 2 must implement
unsigned constants, code-section `*` comments, and EOF-or-`*` data termination without
breaking existing `(` comment support.
