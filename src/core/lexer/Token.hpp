#pragma once

#include <string>

namespace cesil {

/// Lexical token type for CESIL source (no opcode or semantic meaning).
///
/// TODO: debug_metadata (source spans, original label names) can extend \ref Token later.
enum class TokenType {
    Identifier,
    Number,
    Label,
    String,
    Newline,
    Percent,
    Star,
    EndOfFile,
    Unknown,
};

/// Single token from the lexer with exact source text and location.
struct Token {
    TokenType type_{TokenType::Unknown};
    std::string text_{};
    /// 1-based source line at token start.
    int line_{1};
    /// 1-based column at token start.
    int column_{1};
};

}  // namespace cesil
