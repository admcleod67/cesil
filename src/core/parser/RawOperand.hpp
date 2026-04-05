#pragma once

#include "runtime/Opcode.hpp"
#include "lexer/Token.hpp"

namespace cesil {

/// Operand token captured during syntax analysis; interpreted in the semantic pass.
///
/// When no operand is present (e.g. HALT), \p token_ has type \c TokenType::EndOfFile as a sentinel.
struct RawOperand {
    /// Mnemonic on this line (determines expected operand shape after resolution).
    OpCode opcodeContext_{OpCode::Halt};
    Token token_{};
};

}  // namespace cesil
