#include "cesil/semantics/validator.hpp"

#include "cesil/diagnostic.hpp"
#include "cesil/lexer/token_utils.hpp"
#include "cesil/parser/instruction_rules.hpp"

namespace cesil {

bool SemanticValidator::resolve_operand(const RawOperand& raw, Operand& out, std::vector<Diagnostic>& diags) {
    const OpCode op = raw.opcode_context;
    const Token& tok = raw.token;

    if (!instruction_rules::needs_operand(op)) {
        out = Operand{};
        return true;
    }

    if (tok.type == TokenType::EndOfFile) {
        push_diagnostic(diags, DiagnosticSeverity::Error, "missing operand", tok.line, tok.column);
        return false;
    }

    if (op == OpCode::Print) {
        if (tok.type != TokenType::String) {
            push_diagnostic(diags, DiagnosticSeverity::Error, "PRINT operand must be a quoted string", tok.line,
                            tok.column);
            return false;
        }
        out.kind = OperandKind::Symbolic;
        out.symbol = tok.text;
        return true;
    }

    if (instruction_rules::operand_must_be_label(op)) {
        if (tok.type != TokenType::Identifier) {
            push_diagnostic(diags, DiagnosticSeverity::Error, "jump target must be a valid label", tok.line,
                            tok.column);
            return false;
        }
        if (!lexer::is_valid_identifier(tok.text)) {
            push_diagnostic(diags, DiagnosticSeverity::Error, "jump target must be a valid label", tok.line,
                            tok.column);
            return false;
        }
        out.kind = OperandKind::Symbolic;
        out.symbol = tok.text;
        return true;
    }

    if (op == OpCode::Store) {
        if (tok.type != TokenType::Identifier) {
            push_diagnostic(diags, DiagnosticSeverity::Error, "STORE target must be a valid store name", tok.line,
                            tok.column);
            return false;
        }
        if (!lexer::is_valid_identifier(tok.text)) {
            push_diagnostic(diags, DiagnosticSeverity::Error, "STORE target must be a valid store name", tok.line,
                            tok.column);
            return false;
        }
        out.kind = OperandKind::Symbolic;
        out.symbol = tok.text;
        return true;
    }

    if (tok.type == TokenType::Number) {
        int value = 0;
        if (!lexer::parse_signed_integer(tok.text, value)) {
            push_diagnostic(diags, DiagnosticSeverity::Error, "invalid numeric constant", tok.line, tok.column);
            return false;
        }
        out.kind = OperandKind::Immediate;
        out.immediate = value;
        out.symbol.clear();
        return true;
    }

    if (tok.type == TokenType::Identifier) {
        if (!lexer::is_valid_identifier(tok.text)) {
            push_diagnostic(diags, DiagnosticSeverity::Error, "invalid operand", tok.line, tok.column);
            return false;
        }
        out.kind = OperandKind::Symbolic;
        out.symbol = tok.text;
        return true;
    }

    push_diagnostic(diags, DiagnosticSeverity::Error, "invalid operand for this instruction", tok.line, tok.column);
    return false;
}

bool SemanticValidator::validate_label_names(const ParseResult& result, std::vector<Diagnostic>& diags) {
    for (const auto& entry : result.label_define_lines) {
        const std::string& name = entry.first;
        const int line = entry.second;
        if (!lexer::is_valid_identifier(name)) {
            push_diagnostic(diags, DiagnosticSeverity::Error, "invalid label", line, 0);
            return false;
        }
    }
    return true;
}

bool SemanticValidator::validate_jump_targets(const ParseResult& result, std::vector<Diagnostic>& diags) {
    for (const Instruction& inst : result.instructions) {
        if (!instruction_rules::operand_must_be_label(inst.opcode)) {
            continue;
        }
        if (inst.operand.kind != OperandKind::Symbolic) {
            continue;
        }
        if (result.label_indices.count(inst.operand.symbol) == 0) {
            // TODO: forward references / multi-pass label resolution for assembler.
            push_diagnostic(diags, DiagnosticSeverity::Error,
                            "undefined label '" + inst.operand.symbol + "'", inst.lineNumber, 0);
            return false;
        }
    }
    return true;
}

bool SemanticValidator::run(ParseResult& result) const {
    result.semantic_ok = false;
    result.data.clear();

    if (result.instructions.size() != result.raw_operands.size()) {
        push_diagnostic(result.diagnostics, DiagnosticSeverity::Error, "internal parse state mismatch", 0, 0);
        return false;
    }

    if (!validate_label_names(result, result.diagnostics)) {
        return false;
    }

    for (std::size_t i = 0; i < result.instructions.size(); ++i) {
        // TODO: relocatable symbols / assembler fixups in resolve_operand path.
        if (!resolve_operand(result.raw_operands[i], result.instructions[i].operand, result.diagnostics)) {
            return false;
        }
    }

    for (const Token& t : result.raw_data_tokens) {
        int value = 0;
        if (!lexer::parse_signed_integer(t.text, value)) {
            push_diagnostic(result.diagnostics, DiagnosticSeverity::Error, "invalid data section integer", t.line,
                            t.column);
            return false;
        }
        result.data.push_back(value);
    }

    if (!validate_jump_targets(result, result.diagnostics)) {
        return false;
    }

    result.semantic_ok = true;
    return true;
}

}  // namespace cesil
