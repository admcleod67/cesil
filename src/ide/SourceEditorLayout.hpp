#pragma once

#include <QString>
#include <QStringView>

#include <array>
#include <algorithm>

/// CESIL source column layout for the IDE editor (Milestone 3).
///
/// 1-based visual columns for the conventional label / instruction / operand
/// fields, confirmed against examples/total.ces and examples/hello.ces with
/// tab width 8. Parsing does not require these columns; they guide Tab / Shift-Tab
/// behaviour and editor tests.
namespace sourceEditorLayout {

/// Visual width of a tab character, matching the core lexer diagnostic expansion.
inline constexpr int kVisualTabWidth = 8;

/// 1-based column where an optional line label begins.
inline constexpr int kLabelColumn = 1;

/// 1-based column where the instruction mnemonic begins.
inline constexpr int kInstructionColumn = 9;

/// 1-based column where the optional operand begins.
inline constexpr int kOperandColumn = 17;

/// Primary Tab stops for single-line CESIL editing. Further stops continue every
/// \ref kVisualTabWidth columns (25, 33, …).
inline constexpr std::array<int, 3> kSourceColumns = {
    kLabelColumn,
    kInstructionColumn,
    kOperandColumn,
};

static_assert(kSourceColumns.size() == 3);
static_assert(kInstructionColumn - kLabelColumn == kVisualTabWidth);
static_assert(kOperandColumn - kInstructionColumn == kVisualTabWidth);

/// 1-based visual column after \p linePrefix (cursor sitting past those characters).
inline int visualColumn(QStringView linePrefix) {
    int column = kLabelColumn;
    for (const QChar ch : linePrefix) {
        if (ch == QLatin1Char('\t')) {
            column += kVisualTabWidth - ((column - 1) % kVisualTabWidth);
        } else {
            ++column;
        }
    }
    return column;
}

/// Smallest tab stop strictly greater than \p visualCol (stops are 1 + 8n).
inline int nextTabStop(int visualCol) {
    const int column = std::max(visualCol, kLabelColumn);
    const int offset = column - kLabelColumn;
    const int nextOffset = ((offset / kVisualTabWidth) + 1) * kVisualTabWidth;
    return kLabelColumn + nextOffset;
}

/// Largest tab stop strictly less than \p visualCol, floored at label column 1.
inline int previousTabStop(int visualCol) {
    if (visualCol <= kLabelColumn) {
        return kLabelColumn;
    }
    const int offset = visualCol - kLabelColumn;
    const int prevOffset = ((offset - 1) / kVisualTabWidth) * kVisualTabWidth;
    return kLabelColumn + prevOffset;
}

/// Number of spaces to insert so a caret at \p visualCol advances to the next stop.
inline int spacesToNextStop(int visualCol) {
    return nextTabStop(visualCol) - visualCol;
}

/// Index of the first non-space/non-tab character, or \p line.size() if none.
inline int leadingWhitespaceLength(QStringView line) {
    int index = 0;
    while (index < line.size()) {
        const QChar ch = line[index];
        if (ch != QLatin1Char(' ') && ch != QLatin1Char('\t')) {
            break;
        }
        ++index;
    }
    return index;
}

/// Visual column where line content begins (or past all-whitespace lines).
inline int contentStartColumn(QStringView line) {
    return visualColumn(line.left(leadingWhitespaceLength(line)));
}

/// Characters of leading whitespace to remove to shed up to one tab stop of width.
inline int leadingCharsToRemoveForOneStop(QStringView line) {
    int column = kLabelColumn;
    int index = 0;
    int removedVisual = 0;
    while (index < line.size() && removedVisual < kVisualTabWidth) {
        const QChar ch = line[index];
        if (ch != QLatin1Char(' ') && ch != QLatin1Char('\t')) {
            break;
        }
        const int before = column;
        if (ch == QLatin1Char('\t')) {
            column += kVisualTabWidth - ((column - 1) % kVisualTabWidth);
        } else {
            ++column;
        }
        removedVisual += column - before;
        ++index;
    }
    return index;
}

/// Leading indent made of spaces so content starts at \p targetColumn (1-based).
inline QString leadingSpacesForColumn(int targetColumn) {
    const int width = std::max(0, targetColumn - kLabelColumn);
    return QString(width, QLatin1Char(' '));
}

}  // namespace sourceEditorLayout
