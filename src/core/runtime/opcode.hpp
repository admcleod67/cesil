#pragma once

#include <cstdint>

namespace cesil {

/// Machine-level operation codes for CESIL instructions.
///
/// Values are stable for hypothetical binary encoding on a CESIL CPU.
enum class OpCode : std::uint8_t {
    Load = 0,
    Store = 1,
    In = 2,
    Add = 3,
    Subtract = 4,
    Multiply = 5,
    Divide = 6,
    Jump = 7,
    JiZero = 8,
    JiNeg = 9,
    Out = 10,
    Line = 11,
    Print = 12,
    Halt = 13,
};

/// \return Human-readable name for \p op (suitable for diagnostics).
const char* toString(OpCode op) noexcept;

}  // namespace cesil
