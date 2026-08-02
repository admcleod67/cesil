# Jacobs probe log (Milestone 6 Stage 1)

Corpus-gate probe for Visual CESIL source forms (Q4–Q6). Treat results as
behavioural evidence for Stage 2, not exact UI string catalogs.

## Tool

| Field | Value |
|-------|--------|
| Tool | Visual CESIL 2.0 distribution (examples + `Release Notes.htm`) |
| Version / build date | 2.0 (Release Notes: 30-May-2010); earlier notes 24-May-2003 / 26-Mar-2003 |
| Host OS | Probe evidence gathered without running `Visual CESIL.exe` on this host (macOS; no Wine). Findings are from **shipped example sources** and release notes in a local Visual CESIL 2.0 tree. Live Check/Run on Windows remains recommended to confirm UI status text and to fill any `_pending_` runtime rows. |
| Operator / date | Corpus inspection / 2026-08-02 |

## Method

1. Map each gate fixture to patterns already present in Jacobs’ shipped `.ces` examples
   or stated in `Release Notes.htm`.
2. Record compile/run expectations for Stage 2.
3. External checklist: `Example 1.ces`, `Example 2.ces` (not vendored in this repo).

## Per-file observations (gate fixtures)

| File | Compile (expected / evidence) | Runtime notes | Implications |
|------|------------------------------|---------------|--------------|
| `gate-unsigned-constant.ces` | **Accept** — `Example 1.ces` uses `LOAD 0`; `Division.ces` uses unsigned literals such as `LOAD 8` | Expect `OUT` of `8` then newline | **Q4:** unsigned non-negative constants are Jacobs-legal |
| `gate-star-comment.ces` | **Accept** — `Example 1.ces` / `Example 2.ces` / `Division.ces` begin many lines with `*` comments | N/A if comments ignored | **Q5:** `*` full-line comments are Jacobs-legal (distinct from end-of-data `*`) |
| `gate-paren-comment.ces` | **Unclear from examples** — Wikipedia/classic use `(`; shipped Jacobs examples use `*` instead. Release notes do not document `(` | _pending live Check_ | Keep classic `(` as **specified** for this dialect if already supported; confirm Jacobs tolerance with Windows Check |
| `gate-data-no-star.ces` | **Accept** — `Example 1.ces` and `Example 2.ces` end after data values with **no** trailing `*` line | Examples are intended to run | **Q6:** EOF after data is Jacobs-legal |
| `gate-data-with-star.ces` | **Accept** (classic); Jacobs examples omit `*` but a trailing `*` as end-of-data is the classic marker and should remain valid | _pending live Check_ if `*` after data is misread as a comment | Stage 2 must not break classic `*` terminators while allowing EOF |
| `smoke-print-out.ces` | **Accept** (ordinary `PRINT`/`OUT`/`LINE`) | Capture exact bytes on Windows when convenient (**Q8**, Stage 3/4) | Non-blocking for Stage 1 |

## Stage 3 stubs (optional; not required for Stage 1)

| File | Notes |
|------|--------|
| `probe-unset-var.ces` | Not probed for Stage 1 close-out (**Q1** → Stage 3) |
| `probe-case-fold.ces` | Not probed for Stage 1 close-out (**Q2** → Stage 3). Release Notes 2.0: variables initialised to zero at start — related runtime note only |

## External example checklist

| External file | Observations (from source text) | Q4–Q6 |
|---------------|----------------------------------|-------|
| `Example 1.ces` | Leading `*` comment lines; `LOAD 0` (unsigned); `%` then data `1`/`2`/`3`/`-1`; **no** trailing `*` | Q4, Q5, Q6 all exercised |
| `Example 2.ces` | Same comment and data-terminator pattern; unsigned data values | Q5, Q6 |

## Release-notes support

- **1.2 (24-May-2003):** positive numbers may be prefixed with `+` (`+0` allowed, `-0` not); blank lines ignored; `IN` past end errors instead of crash.
- **2.0 (30-May-2010):** variables initialised to zero at execution start.

## Stage 1 conclusions (gate)

| Q | Conclusion for this dialect (pending Stage 2 implementation) |
|---|---------------------------------------------------------------|
| Q4 | **Accept unsigned** non-negative constants (and optional `+`) for corpus compatibility |
| Q5 | **Accept `*` full-line comments** in the code section for corpus compatibility; keep classic `(` comments |
| Q6 | **Accept EOF** as end of data (no trailing `*`); keep classic trailing `*` as valid |

Live Windows Check/Run of the in-repo fixtures should be recorded here when available; corpus evidence above is sufficient to unlock Stage 2 design for Q4–Q6.
