#include <catch2/catch_test_macros.hpp>

#include "lexer/Lexer.hpp"
#include "parser/Parser.hpp"
#include "runtime/Opcode.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>

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
