← [Language reference index](README.md)

# Program structure

## Sections

A classic CESIL program has:

1. **Code** — instruction lines (and comments), executed from the top.
2. **Data marker** — a line whose significant token is `%`, ending the code section and
   starting data. **specified**
3. **Data** — integer values consumed by `IN`, in order. **specified**
4. **End of data** — either a line whose significant token is `*`, **or** end-of-file
   after the last data value. **specified** (classic `*`; Visual CESIL corpus also
   omits `*` and ends at EOF — Stage 1 gate, [`testdata/parity/PROBE.md`](../../testdata/parity/PROBE.md))

Do not treat data lines as instructions or vice versa after `%`. In the **code**
section, a line starting with `*` is a comment ([Source form](source-form.md)), not
end-of-data.

## Data values

| Rule | Status |
|------|--------|
| Each data item is an integer (signed or unsigned non-negative forms as in [Source form](source-form.md)) | **specified** |
| Invalid data tokens are compile-time (or load-time) errors | **specified** |
| Valid integers between invalid rows should still be collected when recovery allows | **specified** (this project; Jacobs floor for multi-invalid data) |

## `IN` and exhaustion

`IN` reads the next unread data value into the accumulator. Reading past the last
value is a **runtime** error. Visual CESIL banner:
`** ERROR: Attempt to read more data than was provided`. **specified**

Visual CESIL 1.2 release notes: reading past end generates an error instead of
crashing. **Jacobs-observed**

## Labels

- A label may appear only once in the code section; duplicates are errors. **specified**
- Forward references (jump to a label defined later) are allowed. **specified**
- Jumping to a never-defined label is a compile-time error. **specified**

## Minimal programs

A program with `HALT` and an empty data section ( `%` then `*` ) is a valid control
baseline. **specified** (project fixtures; Jacobs accepted similar minimal cases)
