# Jacobs probe log

Fill this in after running the fixtures on Windows. Treat results as a **minimum**
floor for Milestone 4, not exact message text to clone.

## Tool

| Field | Value |
|-------|--------|
| Tool | _pending_ (Visual CESIL 2.0 **or** Jacobs Java CLI) |
| Version / build date | _pending_ |
| Host OS | _pending_ |
| Operator / date | _pending_ |

## Per-file observations

For each file: list error *conditions* in order (paraphrase is fine), and note
whether later lines still appeared to be checked after an earlier error.

| File | Jacobs conditions (ordered) | Recovery / later lines? | Notes |
|------|-----------------------------|-------------------------|-------|
| `syntax-multi-unknown.ces` | _pending_ | _pending_ | |
| `syntax-missing-operands.ces` | _pending_ | _pending_ | |
| `syntax-junk-after-mnemonic.ces` | _pending_ | _pending_ | |
| `semantic-multi-undefined-jump.ces` | _pending_ | _pending_ | |
| `semantic-duplicate-labels.ces` | _pending_ | _pending_ | |
| `semantic-bad-operands.ces` | _pending_ | _pending_ | |
| `data-multi-invalid.ces` | _pending_ | _pending_ | |
| `mixed-syntax-then-semantic.ces` | _pending_ | _pending_ | |
| `mixed-recover-then-valid.ces` | _pending_ | _pending_ | |
| `structural-percent-boundary.ces` | _pending_ | _pending_ | |
| `structural-empty-ish.ces` | _pending_ | _pending_ | |

## Deliberate differences (this project)

After the probe, list where cesil will intentionally report **more** than Jacobs,
suppress cascades more tightly, or otherwise diverge. Leave blank until the probe
is done.

- _pending_
