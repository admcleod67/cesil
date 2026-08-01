#include <catch2/catch_test_macros.hpp>

#include "errors/Diagnostic.hpp"
#include "lexer/Lexer.hpp"
#include "parser/Parser.hpp"
#include "runtime/Opcode.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace {

std::filesystem::path diagnosticsDir() {
    return std::filesystem::path(CESIL_SOURCE_DIR) / "testdata" / "diagnostics";
}

std::string readFixture(std::string_view name) {
    const auto path = diagnosticsDir() / std::string(name);
    std::ifstream in(path);
    REQUIRE(in);
    std::ostringstream buffer;
    buffer << in.rdbuf();
    return buffer.str();
}

bool hasMessageContaining(const cesil::ParseResult& r, std::string_view needle) {
    for (const auto& d : r.diagnostics_) {
        if (d.message_.find(needle) != std::string::npos) {
            return true;
        }
    }
    return false;
}

std::size_t countMessageContaining(const cesil::ParseResult& r, std::string_view needle) {
    std::size_t count = 0;
    for (const auto& d : r.diagnostics_) {
        if (d.message_.find(needle) != std::string::npos) {
            ++count;
        }
    }
    return count;
}

}  // namespace

TEST_CASE("Diagnostic accumulation fixtures", "[parser][diagnostics]") {
    cesil::Parser parser;

    SECTION("syntax-multi-unknown") {
        const auto r = parser.parse(readFixture("syntax-multi-unknown.ces"));
        CHECK_FALSE(r.ok_);
        CHECK(countMessageContaining(r, "unknown instruction") >= 2);
    }

    SECTION("syntax-missing-operands") {
        const auto r = parser.parse(readFixture("syntax-missing-operands.ces"));
        CHECK_FALSE(r.ok_);
        CHECK(countMessageContaining(r, "missing operand") >= 3);
    }

    SECTION("syntax-junk-after-mnemonic") {
        const auto r = parser.parse(readFixture("syntax-junk-after-mnemonic.ces"));
        CHECK_FALSE(r.ok_);
        CHECK(countMessageContaining(r, "unexpected") >= 2);
        CHECK(hasMessageContaining(r, "unexpected"));
    }

    SECTION("semantic-multi-undefined-jump") {
        const auto r = parser.parse(readFixture("semantic-multi-undefined-jump.ces"));
        CHECK_FALSE(r.ok_);
        CHECK(countMessageContaining(r, "undefined label") >= 2);
    }

    SECTION("semantic-duplicate-labels") {
        const auto r = parser.parse(readFixture("semantic-duplicate-labels.ces"));
        CHECK_FALSE(r.ok_);
        CHECK(hasMessageContaining(r, "duplicate label"));
        CHECK(hasMessageContaining(r, "undefined label"));
    }

    SECTION("semantic-bad-operands") {
        const auto r = parser.parse(readFixture("semantic-bad-operands.ces"));
        CHECK_FALSE(r.ok_);
        CHECK(hasMessageContaining(r, "PRINT operand"));
        CHECK(hasMessageContaining(r, "STORE target"));
        CHECK(hasMessageContaining(r, "jump target"));
        // ADD XYZ is a legal variable operand — not a shape error.
        CHECK(countMessageContaining(r, "invalid operand") == 0);
    }

    SECTION("data-multi-invalid") {
        const auto r = parser.parse(readFixture("data-multi-invalid.ces"));
        CHECK_FALSE(r.ok_);
        CHECK(countMessageContaining(r, "invalid data") >= 2);
        CHECK(r.data_.size() == 3);
        CHECK(r.data_[0] == 1);
        CHECK(r.data_[1] == 2);
        CHECK(r.data_[2] == 3);
    }

    SECTION("mixed-syntax-then-semantic") {
        const auto r = parser.parse(readFixture("mixed-syntax-then-semantic.ces"));
        CHECK_FALSE(r.ok_);
        CHECK(hasMessageContaining(r, "unknown instruction"));
        CHECK(hasMessageContaining(r, "undefined label"));
    }

    SECTION("mixed-recover-then-valid") {
        const auto r = parser.parse(readFixture("mixed-recover-then-valid.ces"));
        CHECK_FALSE(r.ok_);
        CHECK(countMessageContaining(r, "unknown instruction") == 1);
        REQUIRE(r.instructions_.size() >= 4);
        CHECK(r.instructions_[0].opcode_ == cesil::OpCode::Load);
        CHECK(r.instructions_[1].opcode_ == cesil::OpCode::Out);
        CHECK(r.instructions_[2].opcode_ == cesil::OpCode::Line);
        CHECK(r.instructions_[3].opcode_ == cesil::OpCode::Halt);
    }

    SECTION("structural-percent-boundary") {
        const auto r = parser.parse(readFixture("structural-percent-boundary.ces"));
        CHECK_FALSE(r.ok_);
        CHECK(hasMessageContaining(r, "invalid data"));
        CHECK(r.data_.size() == 1);
        CHECK(r.data_[0] == 5);
    }

    SECTION("structural-empty-ish") {
        const auto r = parser.parse(readFixture("structural-empty-ish.ces"));
        CHECK(r.ok_);
        CHECK(r.diagnostics_.empty());
        REQUIRE(r.instructions_.size() == 1);
        CHECK(r.instructions_[0].opcode_ == cesil::OpCode::Halt);
    }
}

TEST_CASE("Diagnostic finalization ordering from parse", "[parser][diagnostics]") {
    // Semantic error on an earlier line; syntax error later. Without finalize,
    // syntax-pass order would list the late error first.
    constexpr std::string_view source =
        "        JUMP    GHOST\n"
        "        LINE\n"
        "        NOPE\n"
        "        HALT\n"
        "\n"
        "%\n"
        "*\n";
    const auto r = cesil::Parser{}.parse(std::string(source));
    CHECK_FALSE(r.ok_);
    REQUIRE(r.diagnostics_.size() >= 2);
    CHECK(hasMessageContaining(r, "undefined label"));
    CHECK(hasMessageContaining(r, "unknown instruction"));

    int lastLine = 0;
    for (const auto& d : r.diagnostics_) {
        if (d.line_ > 0) {
            CHECK(d.line_ >= lastLine);
            lastLine = d.line_;
        }
    }
    REQUIRE(r.diagnostics_[0].line_ > 0);
    REQUIRE(r.diagnostics_[1].line_ > 0);
    CHECK(r.diagnostics_[0].line_ < r.diagnostics_[1].line_);
    CHECK(r.diagnostics_[0].message_.find("undefined label") != std::string::npos);
    CHECK(r.diagnostics_[1].message_.find("unknown instruction") != std::string::npos);
}

TEST_CASE("finalizeDiagnostics sorts, dedupes, and limits", "[diagnostics]") {
    SECTION("stable source order with unlocated last") {
        std::vector<cesil::Diagnostic> diags;
        cesil::pushDiagnostic(diags, cesil::DiagnosticSeverity::Error, "late", 5, 1);
        cesil::pushDiagnostic(diags, cesil::DiagnosticSeverity::Error, "early", 2, 3);
        cesil::pushDiagnostic(diags, cesil::DiagnosticSeverity::Error, "unlocated", 0, 0);
        cesil::pushDiagnostic(diags, cesil::DiagnosticSeverity::Error, "same-line-later-col", 2, 9);
        cesil::finalizeDiagnostics(diags);
        REQUIRE(diags.size() == 4);
        CHECK(diags[0].message_ == "early");
        CHECK(diags[1].message_ == "same-line-later-col");
        CHECK(diags[2].message_ == "late");
        CHECK(diags[3].message_ == "unlocated");
    }

    SECTION("exact duplicates removed") {
        std::vector<cesil::Diagnostic> diags;
        cesil::pushDiagnostic(diags, cesil::DiagnosticSeverity::Error, "dup", 3, 4);
        cesil::pushDiagnostic(diags, cesil::DiagnosticSeverity::Error, "other", 1, 1);
        cesil::pushDiagnostic(diags, cesil::DiagnosticSeverity::Error, "dup", 3, 4);
        cesil::finalizeDiagnostics(diags);
        REQUIRE(diags.size() == 2);
        CHECK(diags[0].message_ == "other");
        CHECK(diags[1].message_ == "dup");
    }

    SECTION("limit truncates with suppression diagnostic") {
        std::vector<cesil::Diagnostic> diags;
        for (std::size_t i = 0; i < cesil::kMaxCompilationDiagnostics + 25; ++i) {
            cesil::pushDiagnostic(diags, cesil::DiagnosticSeverity::Error,
                                  "err " + std::to_string(i), static_cast<int>(i + 1), 1);
        }
        cesil::finalizeDiagnostics(diags);
        REQUIRE(diags.size() == cesil::kMaxCompilationDiagnostics);
        CHECK(diags.back().message_ == "further compilation errors suppressed");
        CHECK(diags.back().line_ == 0);
        CHECK(diags.back().column_ == 0);
        CHECK(diags[cesil::kMaxCompilationDiagnostics - 2].message_ ==
              "err " + std::to_string(cesil::kMaxCompilationDiagnostics - 2));
    }
}
