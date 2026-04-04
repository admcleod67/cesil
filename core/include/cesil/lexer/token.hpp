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
    TokenType type{TokenType::Unknown};
    std::string text{};
    int line{1};
    int column{1};
};

}  // namespace cesil
