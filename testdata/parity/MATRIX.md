# Compatibility matrix stub (Milestone 6 Stage 1–2)

Gate topics. Status values: **match** (engine agrees with settled rule), **gap**,
**Jacobs-observed** / **specified** / **deliberate diverge**.

Evidence: [`PROBE.md`](PROBE.md). Stage 2 implementation: code-section `*` comments
in [`Parser::parseSyntax`](../../src/core/parser/Parser.cpp); unsigned literals and
EOF data end confirmed by Catch2 (`SourceCompatibilityTest`).

| Topic | Q | Fixture(s) | Rule status | Engine vs rule | Stage 2 action |
|-------|---|------------|-------------|----------------|----------------|
| Unsigned numeric constants | Q4 | `gate-unsigned-constant.ces` | **specified** | **match** | Done — lexer/parser already accepted; locked by test |
| `*` full-line comments | Q5 | `gate-star-comment.ces` | **specified** | **match** | Done — skip `Star` lines in code section only |
| `(` line comments | Q5 | `gate-paren-comment.ces` | **deliberate diverge** | **match** | Preserved (lexer); Jacobs rejects |
| Data ends at EOF (no `*`) | Q6 | `gate-data-no-star.ces` | **specified** | **match** | Done — `parseDataSection` already allowed EOF; locked by test |
| Data ends with `*` | Q6 | `gate-data-with-star.ces` | **specified** | **match** | Preserved |

## Early Stage 3 observations (not Stage 1 gate)

| Topic | Q | Fixture(s) | Rule status | Notes |
|-------|---|------------|-------------|-------|
| Never-stored variable (`LOAD`) | Q1 | `probe-unset-var.ces` | **Jacobs-observed** | Live Check: no error. Unset read as `0` confirmed via case-fold Run |
| Store-name case | Q2 | `probe-case-fold.ces` | **specified** (case-sensitive) | Stage 2 smoke: parse + run → `0\n` (`STORE Foo` / `LOAD FOO`) |
| `PRINT`/`OUT` adjacency | Q8 | `smoke-print-out.ces` | **Jacobs-observed (partial)** | Live Run: `Hi1` — no auto space between string and digit |
