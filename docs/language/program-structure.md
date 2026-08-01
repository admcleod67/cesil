← [Language reference index](README.md)

# Program structure

## Sections

A classic CESIL program has:

1. **Code** — instruction lines (and comments), executed from the top.
2. **Data marker** — a line whose significant token is `%`, ending the code section and
   starting data. **specified**
3. **Data** — integer values consumed by `IN`, in order. **specified**
4. **End of data** — classic materials use `*` to mark the end of data. **specified**
   (classic / Wikipedia)

Visual CESIL examples often omit a trailing `*` and end the file after the last data
value (**Jacobs-observed**). Whether EOF alone ends the data section in this dialect
is **open** (needed for corpus compatibility).

Do not treat data lines as instructions or vice versa after `%`.

## Data values

| Rule | Status |
|------|--------|
| Each data item is an integer (same constant forms as code, subject to the signed/unsigned **open** item) | **specified** |
| Invalid data tokens are compile-time (or load-time) errors | **specified** |
| Valid integers between invalid rows should still be collected when recovery allows | **specified** (this project; Jacobs floor for multi-invalid data) |

## `IN` and exhaustion

`IN` reads the next unread data value into the accumulator. Reading past the last
value is a **runtime** error condition. Classic / Wikipedia cite the banner
`*** PROGRAM REQUIRES MORE DATA ***`. **specified** (condition); exact text **open**
to match or paraphrase.

Visual CESIL 1.2 release notes: reading past end generates an error instead of
crashing. **Jacobs-observed**

## Labels

- A label may appear only once in the code section; duplicates are errors. **specified**
- Forward references (jump to a label defined later) are allowed. **specified**
- Jumping to a never-defined label is a compile-time error. **specified**

## Minimal programs

A program with `HALT` and an empty data section ( `%` then `*` ) is a valid control
baseline. **specified** (project fixtures; Jacobs accepted similar minimal cases)
