# Jacobs probe log (Milestone 6)

Corpus-gate and runtime probes for Visual CESIL 2.0. Treat results as behavioural
evidence for dialect rules and engine gaps, not exact UI string catalogs unless
quoted from a live Run.

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
3. Record compile/run expectations for Stage 2 / Stage 3 / Stage 4 gaps.
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
| `probe-unset-var.ces` | **Accept** — live Check: no error (`LOAD UNSET`) | Reinforced by case-fold Run: distinct unset name `FOO` read as `0` | **Q1:** never-stored names are **not** compile errors; unset reads as **0** at runtime (**specified**) |
| `probe-case-fold.ces` | **Accept** — live Check: no error | Live Run (`LOAD +42` / `STORE Foo` / `LOAD FOO` / `OUT`): output **`0`** | **Q2 specified:** store names are **case-sensitive** (`Foo` ≠ `FOO`) |

## External example checklist

| External file | Observations (from source text) | Q4–Q6 |
|---------------|----------------------------------|-------|
| `Example 1.ces` | Leading `*` comment lines; `LOAD 0` (unsigned); `%` then data `1`/`2`/`3`/`-1`; **no** trailing `*` | Q4, Q5, Q6 all exercised |
| `Example 2.ces` | Same comment and data-terminator pattern; unsigned data values | Q5, Q6 |

## Release-notes support

- **1.2 (24-May-2003):** positive numbers may be prefixed with `+` (`+0` allowed, `-0` not); blank lines ignored; `IN` past end errors instead of crash.
- **2.0 (30-May-2010):** variables initialised to zero at execution start.

## Stage 1 conclusions (gate)

| Q | Conclusion for this dialect |
|---|------------------------------|
| Q4 | **Accept unsigned** non-negative constants (and optional `+`) — Jacobs live Check + corpus |
| Q5 | **Accept `*` full-line comments** in the code section. **Keep classic `(` comments** as a deliberate diverge — Jacobs rejects them |
| Q6 | **Accept EOF** as end of data (no trailing `*`); **keep** classic trailing `*` — both confirmed by live Check |

## Runtime probes (Milestone 6 Stage 3) — live Visual CESIL 2026-08-03

Live Windows Run/Check of `runtime-*.ces`. Earlier Stage 3 notes that assumed classic
Wikipedia banners and fall-off-without-`HALT` are **superseded** where Jacobs differs.

| File | Jacobs compile | Jacobs runtime / output | Dialect conclusion |
|------|----------------|-------------------------|--------------------|
| `runtime-divide-neg.ces` | Accept | `-3` | **Q7 specified** — toward-zero (`-7/2`); engine **match** |
| `runtime-outdigits.ces` | Accept | `n=42 m=-3` | **Q8 specified** — bare decimal `OUT`; no auto spaces; engine **match** |
| `runtime-overflow.ces` | Accept | `16777214` (`8388607×2`) | **Q3 specified** — no 24-bit clamp/trap; engine **match** |
| `runtime-no-halt.ces` | Accept | `** ERROR: No HALT at end of program` | **Q9 specified** — programs must end with `HALT` (Jacobs). Engine currently allows fall-off success → **gap** |
| `runtime-in-exhaust.ces` | Accept | `** ERROR: Attempt to read more data than was provided` | **Q10 specified** — match Jacobs banner text for Visual CESIL parity. Engine still prints classic `*** PROGRAM REQUIRES MORE DATA ***` → **gap** |
| `runtime-divzero.ces` | Accept | `** ERROR: Attempted division by zero` | **Q10 specified** — match Jacobs banner. Engine still prints classic `*** DIVISION BY ZERO ***` → **gap** |
| `runtime-long-label.ces` | Reject — undefined label (`JUMP NOWHERE`) | N/A | **Q11:** Jacobs accepts 7-char `NOWHERE` as a jump spelling then fails lookup. This dialect keeps classic **max 6** identifiers → **deliberate diverge** |

| Q | Status after live runtime probe |
|---|----------------------------------|
| Q1 | **specified** — unset store reads as `0`; not a compile error |
| Q2 | **specified** — case-sensitive stores |
| Q3 | **specified** — host-width / no 24-bit clamp (live: `16777214`) |
| Q7 | **specified** — toward-zero `DIVIDE` (live: `-3`) |
| Q8 | **specified** — no auto space; bare digits (live: `n=42 m=-3`) |
| Q9 | **specified** — require trailing `HALT`; missing `HALT` is an error (Jacobs). Engine **gap** |
| Q10 | **specified** — Jacobs `** ERROR: …` banners (exact strings above). Engine classic `*** … ***` **gap** |
| Q11 | **deliberate diverge** — keep max length 6; Jacobs allows longer names |

Stage 2 source-form work (unsigned constants, `*` comments, EOF-or-`*` data) remains
done. Stage 4 should close Q9/Q10 engine gaps for Visual CESIL output parity and keep
Q11 as a documented diverge.
