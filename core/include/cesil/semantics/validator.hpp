#pragma once

#include "../parser/parser.hpp"

#include <vector>

namespace cesil {

/// Semantic analysis: operand resolution, data validation, jump target checks.
class SemanticValidator {
   public:
    /// Consumes \p result.raw_operands and \p result.raw_data_tokens; fills \p result.instruction operands
    /// and \p result.data; appends diagnostics. Sets \p result.semantic_ok.
    ///
    /// Preconditions: \p result.syntax_ok and populated syntax fields from \ref Parser.
    bool run(ParseResult& result) const;

   private:
    static bool resolve_operand(const RawOperand& raw, Operand& out, std::vector<Diagnostic>& diags);
    static bool validate_jump_targets(const ParseResult& result, std::vector<Diagnostic>& diags);
    static bool validate_label_names(const ParseResult& result, std::vector<Diagnostic>& diags);
};

}  // namespace cesil
