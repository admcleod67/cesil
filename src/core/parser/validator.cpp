#include "parser/validator.hpp"

#include "errors/diagnostic.hpp"
#include "lexer/token_utils.hpp"
#include "parser/instruction_rules.hpp"

namespace cesil {

bool SemanticValidator::resolveOperand(const RawOperand& raw, Operand& out, std::vector<Diagnostic>& diags) {
    const OpCode op = raw.opcodeContext_;
    const Token& tok = raw.token_;

    if (!instructionRules::needsOperand(op)) {
        out = Operand{};
        return true;
    }

    if (tok.type_ == TokenType::EndOfFile) {
        pushDiagnostic(diags, DiagnosticSeverity::Error, "missing operand", tok.line_, tok.column_);
        return false;
    }

    if (op == OpCode::Print) {
        if (tok.type_ != TokenType::String) {
            pushDiagnostic(diags, DiagnosticSeverity::Error, "PRINT operand must be a quoted string", tok.line_,
                           tok.column_);
            return false;
        }
        out.kind_ = OperandKind::Symbolic;
        out.symbol_ = tok.text_;
        return true;
    }

    if (instructionRules::operandMustBeLabel(op)) {
        if (tok.type_ != TokenType::Identifier) {
            pushDiagnostic(diags, DiagnosticSeverity::Error, "jump target must be a valid label", tok.line_,
                           tok.column_);
            return false;
        }
        if (!lexer::isValidIdentifier(tok.text_)) {
            pushDiagnostic(diags, DiagnosticSeverity::Error, "jump target must be a valid label", tok.line_,
                           tok.column_);
            return false;
        }
        out.kind_ = OperandKind::Symbolic;
        out.symbol_ = tok.text_;
        return true;
    }

    if (op == OpCode::Store) {
        if (tok.type_ != TokenType::Identifier) {
            pushDiagnostic(diags, DiagnosticSeverity::Error, "STORE target must be a valid store name", tok.line_,
                           tok.column_);
            return false;
        }
        if (!lexer::isValidIdentifier(tok.text_)) {
            pushDiagnostic(diags, DiagnosticSeverity::Error, "STORE target must be a valid store name", tok.line_,
                           tok.column_);
            return false;
        }
        out.kind_ = OperandKind::Symbolic;
        out.symbol_ = tok.text_;
        return true;
    }

    if (tok.type_ == TokenType::Number) {
        int value = 0;
        if (!lexer::parseSignedInteger(tok.text_, value)) {
            pushDiagnostic(diags, DiagnosticSeverity::Error, "invalid numeric constant", tok.line_, tok.column_);
            return false;
        }
        out.kind_ = OperandKind::Immediate;
        out.immediate_ = value;
        out.symbol_.clear();
        return true;
    }

    if (tok.type_ == TokenType::Identifier) {
        if (!lexer::isValidIdentifier(tok.text_)) {
            pushDiagnostic(diags, DiagnosticSeverity::Error, "invalid operand", tok.line_, tok.column_);
            return false;
        }
        out.kind_ = OperandKind::Symbolic;
        out.symbol_ = tok.text_;
        return true;
    }

    pushDiagnostic(diags, DiagnosticSeverity::Error, "invalid operand for this instruction", tok.line_, tok.column_);
    return false;
}

bool SemanticValidator::validateLabelNames(const ParseResult& result, std::vector<Diagnostic>& diags) {
    for (const auto& entry : result.labelDefineLines_) {
        const std::string& name = entry.first;
        const int line = entry.second;
        if (!lexer::isValidIdentifier(name)) {
            pushDiagnostic(diags, DiagnosticSeverity::Error, "invalid label", line, 0);
            return false;
        }
    }
    return true;
}

bool SemanticValidator::validateJumpTargets(const ParseResult& result, std::vector<Diagnostic>& diags) {
    for (const Instruction& inst : result.instructions_) {
        if (!instructionRules::operandMustBeLabel(inst.opcode_)) {
            continue;
        }
        if (inst.operand_.kind_ != OperandKind::Symbolic) {
            continue;
        }
        if (result.labelIndices_.count(inst.operand_.symbol_) == 0) {
            // TODO: forward references / multi-pass label resolution for assembler.
            pushDiagnostic(diags, DiagnosticSeverity::Error,
                           "undefined label '" + inst.operand_.symbol_ + "'", inst.lineNumber_, 0);
            return false;
        }
    }
    return true;
}

bool SemanticValidator::run(ParseResult& result) const {
    result.semanticOk_ = false;
    result.data_.clear();

    if (result.instructions_.size() != result.rawOperands_.size()) {
        pushDiagnostic(result.diagnostics_, DiagnosticSeverity::Error, "internal parse state mismatch", 0, 0);
        return false;
    }

    if (!validateLabelNames(result, result.diagnostics_)) {
        return false;
    }

    for (std::size_t i = 0; i < result.instructions_.size(); ++i) {
        // TODO: relocatable symbols / assembler fixups in resolveOperand path.
        if (!resolveOperand(result.rawOperands_[i], result.instructions_[i].operand_, result.diagnostics_)) {
            return false;
        }
    }

    for (const Token& t : result.rawDataTokens_) {
        int value = 0;
        if (!lexer::parseSignedInteger(t.text_, value)) {
            pushDiagnostic(result.diagnostics_, DiagnosticSeverity::Error, "invalid data section integer", t.line_,
                           t.column_);
            return false;
        }
        result.data_.push_back(value);
    }

    if (!validateJumpTargets(result, result.diagnostics_)) {
        return false;
    }

    result.semanticOk_ = true;
    return true;
}

}  // namespace cesil
