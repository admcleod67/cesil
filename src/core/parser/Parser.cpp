#include "parser/Parser.hpp"

#include "lexer/Lexer.hpp"
#include "lexer/TokenUtils.hpp"
#include "parser/InstructionRules.hpp"
#include "parser/Validator.hpp"

#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace cesil {

namespace {

void skipNewlines(Lexer& lexer) {
    while (lexer.peekToken().type_ == TokenType::Newline) {
        lexer.getNextToken();
    }
}

void discardRestOfLine(Lexer& lexer) {
    while (true) {
        const Token t = lexer.peekToken();
        if (t.type_ == TokenType::Newline || t.type_ == TokenType::EndOfFile) {
            break;
        }
        lexer.getNextToken();
    }
}

void recoverToNextLine(Lexer& lexer) {
    discardRestOfLine(lexer);
    if (lexer.peekToken().type_ == TokenType::Newline) {
        lexer.getNextToken();
    }
}

bool expectEndOfLine(Lexer& lexer, std::vector<Diagnostic>& diags) {
    const Token t = lexer.peekToken();
    if (t.type_ == TokenType::Newline) {
        lexer.getNextToken();
        return true;
    }
    if (t.type_ == TokenType::EndOfFile) {
        return true;
    }
    pushDiagnostic(diags, DiagnosticSeverity::Error, "unexpected tokens at end of line", t.line_, t.column_);
    return false;
}

bool readRawOperand(Lexer& lexer, OpCode op, int mnemonicLine, int mnemonicColumn, RawOperand& out,
                    std::vector<Diagnostic>& diags) {
    using cesil::instructionRules::needsOperand;
    if (!needsOperand(op)) {
        out = RawOperand{op, Token{TokenType::EndOfFile, {}, mnemonicLine, mnemonicColumn}};
        return true;
    }

    const Token peeked = lexer.peekToken();
    if (peeked.type_ == TokenType::Newline || peeked.type_ == TokenType::EndOfFile) {
        pushDiagnostic(diags, DiagnosticSeverity::Error, "missing operand", mnemonicLine, mnemonicColumn);
        return false;
    }

    out.opcodeContext_ = op;
    out.token_ = lexer.getNextToken();
    return true;
}

/// Parse data section after `%`. Continues after recoverable row errors.
/// Diagnoses trailing content after `*` as invalid data (not instructions).
bool parseDataSection(Lexer& lexer, ParseResult& result) {
    bool ok = true;
    for (;;) {
        skipNewlines(lexer);
        Token t = lexer.peekToken();
        if (t.type_ == TokenType::EndOfFile) {
            return ok;
        }
        if (t.type_ == TokenType::Star) {
            lexer.getNextToken();
            discardRestOfLine(lexer);
            if (lexer.peekToken().type_ == TokenType::Newline) {
                lexer.getNextToken();
            }
            // Trailing lines after * are invalid data, not code.
            for (;;) {
                skipNewlines(lexer);
                Token trail = lexer.peekToken();
                if (trail.type_ == TokenType::EndOfFile) {
                    break;
                }
                pushDiagnostic(result.diagnostics_, DiagnosticSeverity::Error, "invalid data section integer",
                               trail.line_, trail.column_);
                ok = false;
                recoverToNextLine(lexer);
            }
            return ok;
        }
        if (t.type_ != TokenType::Number) {
            pushDiagnostic(result.diagnostics_, DiagnosticSeverity::Error, "invalid data section integer", t.line_,
                           t.column_);
            ok = false;
            recoverToNextLine(lexer);
            continue;
        }
        lexer.getNextToken();
        result.rawDataTokens_.push_back(t);
        if (!expectEndOfLine(lexer, result.diagnostics_)) {
            ok = false;
            recoverToNextLine(lexer);
        }
    }
}

/// Parse one instruction line. On recoverable failure: diagnose, recover to next
/// line, return false without adding IR (except duplicate-label keeps the instruction).
bool parseInstructionLine(Lexer& lexer, ParseResult& result) {
    using cesil::instructionRules::lookupOpcode;
    using cesil::instructionRules::needsOperand;

    std::optional<std::string> labelName;
    int labelLine = 0;
    int labelColumn = 0;
    OpCode opcode{};
    int mnemonicLine = 0;
    int mnemonicColumn = 0;

    Token first = lexer.getNextToken();

    if (first.type_ == TokenType::Label) {
        labelName = first.text_;
        labelLine = first.line_;
        labelColumn = first.column_;
        Token mn = lexer.getNextToken();
        if (mn.type_ != TokenType::Identifier) {
            pushDiagnostic(result.diagnostics_, DiagnosticSeverity::Error, "missing instruction", mn.line_, mn.column_);
            recoverToNextLine(lexer);
            return false;
        }
        if (!lookupOpcode(lexer::toUpperCopy(mn.text_), opcode)) {
            pushDiagnostic(result.diagnostics_, DiagnosticSeverity::Error, "unknown instruction", mn.line_, mn.column_);
            recoverToNextLine(lexer);
            return false;
        }
        mnemonicLine = mn.line_;
        mnemonicColumn = mn.column_;
    } else if (first.type_ == TokenType::Identifier) {
        if (lookupOpcode(lexer::toUpperCopy(first.text_), opcode)) {
            mnemonicLine = first.line_;
            mnemonicColumn = first.column_;
        } else {
            // Non-opcode identifier: label + mnemonic, or a lone unknown mnemonic.
            const Token peeked = lexer.peekToken();
            if (peeked.type_ == TokenType::Newline || peeked.type_ == TokenType::EndOfFile ||
                peeked.type_ == TokenType::Percent) {
                pushDiagnostic(result.diagnostics_, DiagnosticSeverity::Error, "unknown instruction", first.line_,
                               first.column_);
                recoverToNextLine(lexer);
                return false;
            }
            labelName = std::string(first.text_);
            labelLine = first.line_;
            labelColumn = first.column_;
            Token second = lexer.getNextToken();
            if (second.type_ != TokenType::Identifier) {
                pushDiagnostic(result.diagnostics_, DiagnosticSeverity::Error, "missing instruction", second.line_,
                               second.column_);
                recoverToNextLine(lexer);
                return false;
            }
            if (!lookupOpcode(lexer::toUpperCopy(second.text_), opcode)) {
                pushDiagnostic(result.diagnostics_, DiagnosticSeverity::Error, "unknown instruction", second.line_,
                               second.column_);
                recoverToNextLine(lexer);
                return false;
            }
            mnemonicLine = second.line_;
            mnemonicColumn = second.column_;
        }
    } else {
        pushDiagnostic(result.diagnostics_, DiagnosticSeverity::Error, "expected label or instruction", first.line_,
                       first.column_);
        recoverToNextLine(lexer);
        return false;
    }

    RawOperand raw{};
    if (!readRawOperand(lexer, opcode, mnemonicLine, mnemonicColumn, raw, result.diagnostics_)) {
        recoverToNextLine(lexer);
        return false;
    }

    if (!needsOperand(opcode)) {
        const Token extra = lexer.peekToken();
        if (extra.type_ != TokenType::Newline && extra.type_ != TokenType::EndOfFile) {
            pushDiagnostic(result.diagnostics_, DiagnosticSeverity::Error, "unexpected operand for this instruction",
                           extra.line_, extra.column_);
            recoverToNextLine(lexer);
            return false;
        }
    }

    if (!expectEndOfLine(lexer, result.diagnostics_)) {
        recoverToNextLine(lexer);
        return false;
    }

    bool registerLabel = false;
    if (labelName) {
        if (result.labelIndices_.count(*labelName) != 0) {
            pushDiagnostic(result.diagnostics_, DiagnosticSeverity::Error, "duplicate label", labelLine, labelColumn);
            // Keep the instruction; do not re-register the label.
        } else {
            registerLabel = true;
        }
    }

    if (registerLabel) {
        result.labelIndices_.emplace(*labelName, result.instructions_.size());
        result.labelDefineLines_.emplace(*labelName, labelLine);
    }

    Instruction inst{};
    inst.opcode_ = opcode;
    inst.operand_ = Operand{};
    inst.lineNumber_ = mnemonicLine;
    result.instructions_.push_back(std::move(inst));
    result.rawOperands_.push_back(std::move(raw));
    return true;
}

}  // namespace

bool Parser::parseSyntax(Lexer& lexer, ParseResult& result) const {
    result = ParseResult{};
    result.syntaxOk_ = false;
    bool hadSyntaxError = false;

    for (;;) {
        skipNewlines(lexer);
        const Token t = lexer.peekToken();
        if (t.type_ == TokenType::EndOfFile) {
            break;
        }
        if (t.type_ == TokenType::Percent) {
            lexer.getNextToken();
            if (!parseDataSection(lexer, result)) {
                hadSyntaxError = true;
            }
            break;
        }
        if (!parseInstructionLine(lexer, result)) {
            hadSyntaxError = true;
            continue;
        }
    }

    // Duplicate-label diagnostics are recorded during instruction parse but the
    // line still produced IR; treat any diagnostic so far as syntax failure too.
    if (!result.diagnostics_.empty()) {
        hadSyntaxError = true;
    }

    result.syntaxOk_ = !hadSyntaxError;
    return true;
}

ParseResult Parser::parse(std::string_view source) const {
    ParseResult result;
    if (lexer::isBlank(source)) {
        result.syntaxOk_ = true;
        result.semanticOk_ = true;
        result.ok_ = true;
        return result;
    }
    Lexer lexer(source);
    return parse(lexer);
}

ParseResult Parser::parse(Lexer& lexer) const {
    ParseResult result;
    static_cast<void>(parseSyntax(lexer, result));
    SemanticValidator validator;
    static_cast<void>(validator.run(result));
    finalizeDiagnostics(result.diagnostics_);
    result.ok_ = result.diagnostics_.empty();
    return result;
}

}  // namespace cesil
