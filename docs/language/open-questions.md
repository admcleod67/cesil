← [Language reference index](README.md)

# Open questions and conflicts

Evidence: [`testdata/parity/PROBE.md`](../../testdata/parity/PROBE.md).
Remaining Stage 4 work: example goldens and closing engine **gaps** (Q9/Q10).

## Probe checklist

| ID | Topic | Status |
|----|--------|--------|
| Q1 | Undefined variables (never-stored) | **specified** — compile OK; read as `0` |
| Q2 | Store-name case | **specified** — case-sensitive |
| Q3 | Integer range / overflow | **specified** — no 24-bit clamp/trap (live Jacobs: `8388607×2` → `16777214`) |
| Q4 | Unsigned constants | **specified** |
| Q5 | `*` / `(` comments | **specified** (`*`); `(` **deliberate diverge** |
| Q6 | Data EOF or `*` | **specified** |
| Q7 | `DIVIDE` rounding | **specified** — toward zero (live: `-7/2` → `-3`) |
| Q8 | `OUT` / `PRINT` / `LINE` | **specified** — no auto space; bare decimal `OUT` (live: `n=42 m=-3`) |
| Q9 | Fall off end without `HALT` | **specified** — Jacobs requires `HALT` (`** ERROR: No HALT at end of program`). Engine still allows fall-off → **gap** |
| Q10 | Runtime banner text | **specified** — Jacobs live strings: `** ERROR: Attempt to read more data than was provided`; `** ERROR: Attempted division by zero`. Engine classic `*** … ***` → **gap** |
| Q11 | Label length > 6 | **deliberate diverge** — keep classic max 6; Jacobs accepts longer (`NOWHERE` → undefined-label error) |

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
| Integer width | 24-bit cited | Live: no clamp (`16777214`) | Host `int`, no trap |
| Unset variables | Often `0` | Live: `0` | `0` |
| Store case | Unclear | Case-sensitive | Case-sensitive |
| End without `HALT` | Often OK | Error | Match Jacobs (engine **gap**) |
| Runtime banners | `*** … ***` | `** ERROR: …` | Match Jacobs (engine **gap**) |
| Label length | Max 6 | Longer OK | Max 6 (diverge) |
