#pragma once

#include "parser/Parser.hpp"

#include <vector>

namespace cesil {

/// Semantic analysis: operand resolution, data validation, jump target checks.
class SemanticValidator {
   public:
    /// Consumes \p result.rawOperands_ and \p result.rawDataTokens_; fills \p result.instructions_ operands
    /// and \p result.data_; appends diagnostics. Sets \p result.semanticOk_.
    ///
    /// Preconditions: \p result.syntaxOk_ and populated syntax fields from \ref Parser.
    bool run(ParseResult& result) const;

   private:
    static bool resolveOperand(const RawOperand& raw, Operand& out, std::vector<Diagnostic>& diags);
    static bool validateJumpTargets(const ParseResult& result, std::vector<Diagnostic>& diags);
    static bool validateLabelNames(const ParseResult& result, std::vector<Diagnostic>& diags);
};

}  // namespace cesil
