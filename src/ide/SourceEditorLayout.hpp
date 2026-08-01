#pragma once

#include <array>

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

}  // namespace sourceEditorLayout
