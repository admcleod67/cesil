#pragma once

#include "cesil/diagnostic.hpp"
#include "cesil/instruction.hpp"
#include "cesil/semantics/raw_operand.hpp"
#include "../lexer/token.hpp"

#include <cstddef>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace cesil {

class Lexer;

/// Result of parsing CESIL source into IR (syntax + semantic passes).
struct ParseResult {
    /// Decoded instructions in execution order (operands filled after semantic pass).
    std::vector<Instruction> instructions{};
    /// Integer runtime data (filled after semantic pass).
    std::vector<int> data{};
    /// Line label to instruction index (0-based), recorded during syntax.
    std::unordered_map<std::string, std::size_t> label_indices{};
    /// Source line where each label in \p label_indices was defined (for diagnostics).
    std::unordered_map<std::string, int> label_define_lines{};
    /// Operand tokens per instruction (same length as \p instructions after successful syntax).
    std::vector<RawOperand> raw_operands{};
    /// Raw numeric tokens from the data section (before semantic validation).
    std::vector<Token> raw_data_tokens{};
    std::vector<Diagnostic> diagnostics{};
    bool syntax_ok{false};
    bool semantic_ok{false};
    /// True when both syntax and semantic passes succeeded.
    bool ok{false};
};

/// Converts CESIL source text into a sequence of \ref Instruction values.
class Parser {
   public:
    /// Full pipeline: lexer, syntax, and semantic validation.
    ParseResult parse(std::string_view source) const;

    /// Same as \ref parse(std::string_view) but uses an existing lexer (must be at start of input).
    ParseResult parse(Lexer& lexer) const;

    /// Syntax only: fills \p result without running semantics (for testing).
    /// Sets \p result.syntax_ok; leaves operands and \p data empty unless caller runs \ref SemanticValidator.
    bool parseSyntax(Lexer& lexer, ParseResult& result) const;
};

}  // namespace cesil
