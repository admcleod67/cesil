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

## Runtime probes (Milestone 6 Stage 3)

Fixtures under `runtime-*.ces`. Engine behaviour locked by Catch2
(`RuntimeSemanticsTest`) against classic Wikipedia banners / toward-zero division
and prior live Stage 1 notes. **Live Visual CESIL Run of these fixtures on Windows
is still recommended** to confirm Q3/Q7/Q8 multi-digit / Q11; until then Stage 3
settles the dialect as below (host-width arithmetic; classic max-6 labels).

| File | Compile (this engine) | Runtime / output (this engine + classic) | Dialect conclusion |
|------|----------------------|------------------------------------------|--------------------|
| `runtime-divide-neg.ces` | Accept | `-3\n` (`-7/2` toward zero) | **Q7 specified** — toward-zero integer division (Wikipedia/classic; C++ `/=`) |
| `runtime-outdigits.ces` | Accept | `n=42 m=-3\n` (no padding, no auto spaces) | **Q8 specified** — bare decimal `OUT`; concatenate with `PRINT` |
| `runtime-no-halt.ces` | Accept | `5\n`, run succeeds | **Q9 specified** — fall off end is successful termination |
| `runtime-in-exhaust.ces` | Accept | stdout contains `*** PROGRAM REQUIRES MORE DATA ***` | **Q10 specified** — classic banner text |
| `runtime-divzero.ces` | Accept | stdout contains `*** DIVISION BY ZERO ***` | **Q10 specified** — classic banner text |
| `runtime-overflow.ces` | Accept | `16777214\n` (8388607×2 on host `int`) | **Q3 specified** — host-width `int`, no 24-bit clamp/trap; historical 24-bit is non-enforced classic background. Live Jacobs overflow behaviour deferred to Stage 4 if needed |
| `runtime-long-label.ces` | Reject (invalid jump target / label) | N/A | **Q11 specified** — max 6 characters (classic). Jacobs may accept longer names (M4 probe used `NOWHERE`); this dialect keeps classic max-6 (**deliberate diverge** if Jacobs is laxer) |

| Q | Stage 3 status |
|---|----------------|
| Q1 | **specified** — unset store reads as `0`; not a compile error (2026-08-03 live + tests) |
| Q2 | **specified** — case-sensitive stores (unchanged) |
| Q3 | **specified** — host `int`, no overflow diagnostic (live Jacobs overflow Run deferred) |
| Q7 | **specified** — toward-zero `DIVIDE` |
| Q8 | **specified** — no auto space; bare decimal digits (incl. multi-digit / negative) |
| Q9 | **specified** — fall off end succeeds |
| Q10 | **specified** — exact classic `*** … ***` banners |
| Q11 | **specified** — identifiers ≤6 chars (classic) |

Live gate Check of the in-repo fixtures is recorded above. Stage 2 implemented
unsigned constants, code-section `*` comments, and EOF-or-`*` data termination without
breaking existing `(` comment support.
