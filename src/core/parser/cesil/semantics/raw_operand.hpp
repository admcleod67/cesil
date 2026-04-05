#pragma once

#include "cesil/opcode.hpp"
#include "../lexer/token.hpp"

namespace cesil {

/// Operand token captured during syntax analysis; interpreted in the semantic pass.
///
/// When no operand is present (e.g. HALT), \p token has type \c TokenType::EndOfFile as a sentinel.
struct RawOperand {
    /// Mnemonic on this line (determines expected operand shape after resolution).
    OpCode opcode_context{OpCode::Halt};
    Token token{};
};

}  // namespace cesil
