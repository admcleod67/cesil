← [Language reference index](README.md)

# Open questions and conflicts

Evidence: [`testdata/parity/PROBE.md`](../../testdata/parity/PROBE.md).
Milestone 6 Stages 1–4 settle the checklist below for the 0.6.0 language-parity
checkpoint. Deferred: vendoring full Jacobs Example trees; relaxing max-6 labels.

## Probe checklist

| ID | Topic | Status |
|----|--------|--------|
| Q1 | Undefined variables (never-stored) | **specified** — compile OK; read as `0` |
| Q2 | Store-name case | **specified** — case-sensitive |
| Q3 | Integer range / overflow | **specified** — no 24-bit clamp/trap |
| Q4 | Unsigned constants | **specified** |
| Q5 | `*` / `(` comments | **specified** (`*`); `(` **deliberate diverge** |
| Q6 | Data EOF or `*` | **specified** |
| Q7 | `DIVIDE` rounding | **specified** — toward zero |
| Q8 | `OUT` / `PRINT` / `LINE` | **specified** — no auto space; bare decimal `OUT` |
| Q9 | Fall off end without `HALT` | **specified** — require `HALT`; Jacobs banner |
| Q10 | Runtime banner text | **specified** — Jacobs `** ERROR: …` strings |
| Q11 | Label length > 6 | **deliberate diverge** — keep classic max 6 |

## Settled divergences (not open)

| Topic | Status |
|-------|--------|
| Multi-error recovery beyond Jacobs fail-fast | **deliberate diverge** / **specified** (M4) |
| Trailing junk after zero-operand mnemonics diagnosed | **deliberate diverge** |
| Bare `STORE` and some operand shapes diagnosed | **deliberate diverge** |
| Classic `(` line comments (Jacobs rejects; uses `*` instead) | **deliberate diverge** |
| Identifier max length 6 (Jacobs allows longer) | **deliberate diverge** |
| Own compile diagnostic message text | **specified** (may differ from Jacobs wording) |
| CESIL “Plus” excluded from 1.0 | **specified** |

## Source disagreements (summary)

| Topic | Classic | Visual CESIL | This dialect |
|-------|---------|--------------|--------------|
| Comments | `(` | `*` (rejects `(`) | Both (`(` diverge) |
| Constants | Often signed | Unsigned OK | Both |
| Data terminator | `*` | EOF or `*` | Either |
| Integer width | 24-bit cited | No clamp | Host `int`, no trap |
| Unset variables | Often `0` | `0` | `0` |
| Store case | Unclear | Case-sensitive | Case-sensitive |
| End without `HALT` | Often OK | Error | Match Jacobs |
| Runtime banners | `*** … ***` | `** ERROR: …` | Match Jacobs |
| Label length | Max 6 | Longer OK | Max 6 (diverge) |
