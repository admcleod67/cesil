#pragma once

#include "lexer/Token.hpp"

#include <optional>
#include <string>
#include <string_view>

namespace cesil {

/// Character-level tokenizer for CESIL source.
///
/// Emits a stream of \ref Token values with line/column (1-based column at token start).
/// Does not interpret mnemonics, labels vs variables, or the data section—only lexical rules
/// (whitespace, newline, comments, punctuation, numbers, identifiers, strings).
class Lexer {
   public:
    /// \param source Full program text; copied internally for stable token text lifetime.
    explicit Lexer(std::string_view source);

    /// Advance and return the next token.
    Token getNextToken();

    /// Look at the next token without consuming it.
    Token peekToken();

   private:
    Token readTokenImpl();

    void skipHorizontalSpace();
    void advanceOne();
    bool atEnd() const;
    char peekChar() const;
    Token makeToken(TokenType type, std::string text, int line, int col) const;

    std::string buffer_;
    std::size_t pos_{0};
    int line_{1};
    int column_{1};
    /// True until the first non-comment token on the current logical line is emitted.
    bool lineHasStartedToken_{false};

    std::optional<Token> peekBuffer_{};
};

}  // namespace cesil
