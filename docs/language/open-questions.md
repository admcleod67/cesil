← [Language reference index](README.md)

# Open questions and conflicts

Stage 1–3 evidence: [`testdata/parity/PROBE.md`](../../testdata/parity/PROBE.md).
Remaining work for full example goldens belongs to Milestone 6 Stage 4.

## Probe checklist

| ID | Topic | Status |
|----|--------|--------|
| Q1 | Undefined variables (never-stored) | **specified** — compile OK; read as `0` |
| Q2 | Store-name case | **specified** — case-sensitive |
| Q3 | Integer range / overflow | **specified** — host `int`, no 24-bit clamp/trap; historical 24-bit non-enforced. Live Jacobs overflow Run deferred to Stage 4 if needed |
| Q4 | Unsigned constants | **specified** |
| Q5 | `*` / `(` comments | **specified** (`*`); `(` **deliberate diverge** |
| Q6 | Data EOF or `*` | **specified** |
| Q7 | `DIVIDE` rounding | **specified** — toward zero |
| Q8 | `OUT` / `PRINT` / `LINE` | **specified** — no auto space; bare decimal `OUT` |
| Q9 | Fall off end without `HALT` | **specified** — successful termination |
| Q10 | Runtime banner text | **specified** — classic `*** PROGRAM REQUIRES MORE DATA ***` / `*** DIVISION BY ZERO ***` |
| Q11 | Label length > 6 | **specified** — max 6 (classic). Jacobs may accept longer; keep classic limit |

## Settled divergences (not open)

| Topic | Status |
|-------|--------|
| Multi-error recovery beyond Jacobs fail-fast | **deliberate diverge** / **specified** (M4) |
| Trailing junk after zero-operand mnemonics diagnosed | **deliberate diverge** |
| Bare `STORE` and some operand shapes diagnosed | **deliberate diverge** |
| Classic `(` line comments (Jacobs rejects; uses `*` instead) | **deliberate diverge** |
| Identifier max length 6 if Jacobs allows longer | **deliberate diverge** / classic **specified** |
| Own diagnostic message text (banners may still match classic) | **specified** |
| CESIL “Plus” excluded from 1.0 | **specified** |

## Source disagreements (summary)

| Topic | Classic | Visual CESIL | This dialect |
|-------|---------|--------------|--------------|
| Comments | `(` | `*` (rejects `(`) | Both (`(` diverge) |
| Constants | Often signed | Unsigned OK | Both |
| Data terminator | `*` | EOF or `*` | Either |
| Integer width | 24-bit cited | Unclear / deferred live | Host `int`, no trap |
| Unset variables | Often `0` | Live: `0` | `0` |
| Store case | Unclear | Case-sensitive | Case-sensitive |
| Label length | Max 6 | May allow longer | Max 6 |
