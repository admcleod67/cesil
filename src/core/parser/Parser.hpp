#pragma once

#include "errors/Diagnostic.hpp"
#include "runtime/Instruction.hpp"
#include "parser/RawOperand.hpp"
#include "lexer/Token.hpp"

#include <cstddef>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace cesil {

class Lexer;

/// Result of parsing CESIL source into IR (syntax + semantic passes).
struct ParseResult {
    /// Decoded instructions in execution order (operands filled after semantic pass).
    std::vector<Instruction> instructions_{};
    /// Integer runtime data (filled after semantic pass).
    std::vector<int> data_{};
    /// Line label to instruction index (0-based), recorded during syntax.
    std::unordered_map<std::string, std::size_t> labelIndices_{};
    /// Source line where each label in \p labelIndices_ was defined (for diagnostics).
    std::unordered_map<std::string, int> labelDefineLines_{};
    /// Operand tokens per instruction (same length as \p instructions_ after successful syntax).
    std::vector<RawOperand> rawOperands_{};
    /// Raw numeric tokens from the data section (before semantic validation).
    std::vector<Token> rawDataTokens_{};
    std::vector<Diagnostic> diagnostics_{};
    bool syntaxOk_{false};
    bool semanticOk_{false};
    /// True when both syntax and semantic passes succeeded.
    bool ok_{false};
};

/// Converts CESIL source text into a sequence of \ref Instruction values.
class Parser {
   public:
    /// Full pipeline: lexer, syntax, and semantic validation.
    ParseResult parse(std::string_view source) const;

    /// Same as \ref parse(std::string_view) but uses an existing lexer (must be at start of input).
    ParseResult parse(Lexer& lexer) const;

    /// Syntax only: fills \p result without running semantics (for testing).
    /// Sets \p result.syntaxOk_; leaves operands and \p data_ empty unless caller runs \ref SemanticValidator.
    bool parseSyntax(Lexer& lexer, ParseResult& result) const;
};

}  // namespace cesil
