#include "../../include/cesil/parser/parser.hpp"

#include "../../include/cesil/lexer/lexer.hpp"
#include "cesil/lexer/token_utils.hpp"
#include "cesil/parser/instruction_rules.hpp"
#include "cesil/semantics/validator.hpp"

#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace cesil {

namespace {

void skip_newlines(Lexer& lexer) {
    while (lexer.peekToken().type == TokenType::Newline) {
        lexer.getNextToken();
    }
}

void discard_rest_of_line(Lexer& lexer) {
    while (true) {
        const Token t = lexer.peekToken();
        if (t.type == TokenType::Newline || t.type == TokenType::EndOfFile) {
            break;
        }
        lexer.getNextToken();
    }
}

bool expect_end_of_line(Lexer& lexer, std::vector<Diagnostic>& diags) {
    const Token t = lexer.peekToken();
    if (t.type == TokenType::Newline) {
        lexer.getNextToken();
        return true;
    }
    if (t.type == TokenType::EndOfFile) {
        return true;
    }
    push_diagnostic(diags, DiagnosticSeverity::Error, "unexpected tokens at end of line", t.line, t.column);
    return false;
}

bool read_raw_operand(Lexer& lexer, OpCode op, int mnemonic_line, int mnemonic_column, RawOperand& out,
                      std::vector<Diagnostic>& diags) {
    using cesil::instruction_rules::needs_operand;
    if (!needs_operand(op)) {
        out = RawOperand{op, Token{TokenType::EndOfFile, "", mnemonic_line, mnemonic_column}};
        return true;
    }

    const Token peeked = lexer.peekToken();
    if (peeked.type == TokenType::Newline || peeked.type == TokenType::EndOfFile) {
        push_diagnostic(diags, DiagnosticSeverity::Error, "missing operand", mnemonic_line, mnemonic_column);
        return false;
    }

    out.opcode_context = op;
    out.token = lexer.getNextToken();
    return true;
}

bool parse_data_section(Lexer& lexer, ParseResult& result) {
    for (;;) {
        skip_newlines(lexer);
        Token t = lexer.peekToken();
        if (t.type == TokenType::EndOfFile) {
            return true;
        }
        if (t.type == TokenType::Star) {
            lexer.getNextToken();
            discard_rest_of_line(lexer);
            return true;
        }
        if (t.type != TokenType::Number) {
            push_diagnostic(result.diagnostics, DiagnosticSeverity::Error, "invalid data section integer", t.line,
                            t.column);
            return false;
        }
        lexer.getNextToken();
        result.raw_data_tokens.push_back(t);
        if (!expect_end_of_line(lexer, result.diagnostics)) {
            return false;
        }
    }
}

bool parse_instruction_line(Lexer& lexer, ParseResult& result) {
    using cesil::instruction_rules::lookup_opcode;
    using cesil::instruction_rules::needs_operand;

    std::optional<std::string> label_name;
    int label_line = 0;
    int label_column = 0;
    OpCode opcode{};
    int mnemonic_line = 0;
    int mnemonic_column = 0;

    Token first = lexer.getNextToken();

    if (first.type == TokenType::Label) {
        label_name = first.text;
        label_line = first.line;
        label_column = first.column;
        Token mn = lexer.getNextToken();
        if (mn.type != TokenType::Identifier) {
            push_diagnostic(result.diagnostics, DiagnosticSeverity::Error, "missing instruction", mn.line, mn.column);
            return false;
        }
        if (!lookup_opcode(lexer::to_upper_copy(mn.text), opcode)) {
            push_diagnostic(result.diagnostics, DiagnosticSeverity::Error, "unknown instruction", mn.line, mn.column);
            return false;
        }
        mnemonic_line = mn.line;
        mnemonic_column = mn.column;
    } else if (first.type == TokenType::Identifier) {
        if (lookup_opcode(lexer::to_upper_copy(first.text), opcode)) {
            mnemonic_line = first.line;
            mnemonic_column = first.column;
        } else {
            label_name = std::string(first.text);
            label_line = first.line;
            label_column = first.column;
            Token second = lexer.getNextToken();
            if (second.type != TokenType::Identifier) {
                push_diagnostic(result.diagnostics, DiagnosticSeverity::Error, "missing instruction", second.line,
                                second.column);
                return false;
            }
            if (!lookup_opcode(lexer::to_upper_copy(second.text), opcode)) {
                push_diagnostic(result.diagnostics, DiagnosticSeverity::Error, "unknown instruction", second.line,
                                second.column);
                return false;
            }
            mnemonic_line = second.line;
            mnemonic_column = second.column;
        }
    } else {
        push_diagnostic(result.diagnostics, DiagnosticSeverity::Error, "expected label or instruction", first.line,
                        first.column);
        return false;
    }

    RawOperand raw{};
    if (!read_raw_operand(lexer, opcode, mnemonic_line, mnemonic_column, raw, result.diagnostics)) {
        return false;
    }

    if (!needs_operand(opcode)) {
        const Token extra = lexer.peekToken();
        if (extra.type != TokenType::Newline && extra.type != TokenType::EndOfFile) {
            push_diagnostic(result.diagnostics, DiagnosticSeverity::Error, "unexpected operand for this instruction",
                            extra.line, extra.column);
            return false;
        }
    }

    if (!expect_end_of_line(lexer, result.diagnostics)) {
        return false;
    }

    if (label_name) {
        if (result.label_indices.count(*label_name) != 0) {
            push_diagnostic(result.diagnostics, DiagnosticSeverity::Error, "duplicate label", label_line, label_column);
            return false;
        }
        // TODO: lazy label resolution / forward references if assembler allows
        result.label_indices.emplace(*label_name, result.instructions.size());
        result.label_define_lines.emplace(*label_name, label_line);
    }

    Instruction inst{};
    inst.opcode = opcode;
    inst.operand = Operand{};
    inst.lineNumber = mnemonic_line;
    result.instructions.push_back(std::move(inst));
    result.raw_operands.push_back(std::move(raw));
    return true;
}

}  // namespace

bool Parser::parseSyntax(Lexer& lexer, ParseResult& result) const {
    result = ParseResult{};
    result.syntax_ok = false;

    for (;;) {
        skip_newlines(lexer);
        const Token t = lexer.peekToken();
        if (t.type == TokenType::EndOfFile) {
            break;
        }
        if (t.type == TokenType::Percent) {
            lexer.getNextToken();
            if (!parse_data_section(lexer, result)) {
                return false;
            }
            while (lexer.peekToken().type != TokenType::EndOfFile) {
                lexer.getNextToken();
            }
            break;
        }
        if (!parse_instruction_line(lexer, result)) {
            return false;
        }
    }

    result.syntax_ok = true;
    return true;
}

ParseResult Parser::parse(std::string_view source) const {
    ParseResult result;
    if (lexer::is_blank(source)) {
        result.syntax_ok = true;
        result.semantic_ok = true;
        result.ok = true;
        return result;
    }
    Lexer lexer(source);
    return parse(lexer);
}

ParseResult Parser::parse(Lexer& lexer) const {
    ParseResult result;
    if (!parseSyntax(lexer, result)) {
        result.ok = false;
        return result;
    }
    SemanticValidator validator;
    if (!validator.run(result)) {
        result.ok = false;
        return result;
    }
    result.ok = true;
    return result;
}

}  // namespace cesil
