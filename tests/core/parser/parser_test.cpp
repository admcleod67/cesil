#include <catch2/catch_test_macros.hpp>
#include "lexer/lexer.hpp"
#include "runtime/opcode.hpp"
#include "runtime/operand.hpp"
#include "parser/parser.hpp"

#include <string>

TEST_CASE("Parser basic functionality", "[parser]") {
    cesil::Parser parser;

    SECTION("Simple instructions") {
        const cesil::ParseResult r = parser.parse("LOOP    LOAD +1\nHALT\n");
        REQUIRE(r.ok_);
        CHECK(r.instructions_.size() == 2);
        CHECK(r.labelIndices_.count("LOOP") == 1);
        CHECK(r.instructions_[0].opcode_ == cesil::OpCode::Load);
        CHECK(r.instructions_[0].operand_.kind_ == cesil::OperandKind::Immediate);
        CHECK(r.instructions_[0].operand_.immediate_ == 1);
        CHECK(r.instructions_[1].opcode_ == cesil::OpCode::Halt);
    }

    SECTION("Labels and strings") {
        const cesil::ParseResult r = parser.parse("DONE: PRINT \"Hi\"\nHALT\n");
        REQUIRE(r.ok_);
        CHECK(r.labelIndices_.count("DONE") == 1);
        CHECK(r.instructions_[0].opcode_ == cesil::OpCode::Print);
        CHECK(r.instructions_[0].operand_.symbol_ == "Hi");
    }

    SECTION("Jumps and multi-line") {
        const std::string src =
            "        JUMP THERE\n"
            "THERE   HALT\n";
        const cesil::ParseResult r = parser.parse(src);
        REQUIRE(r.ok_);
        CHECK(r.labelIndices_.count("THERE") == 1);
    }

    SECTION("Duplicate labels") {
        const std::string src =
            "HERE    HALT\n"
            "HERE    HALT\n";
        const cesil::ParseResult r = parser.parse(src);
        CHECK_FALSE(r.ok_);
        CHECK_FALSE(r.diagnostics_.empty());
    }

    SECTION("Unresolved jumps") {
        const std::string src = "        JUMP NOWHERE\nHALT\n";
        const cesil::ParseResult r = parser.parse(src);
        CHECK_FALSE(r.ok_);
        CHECK_FALSE(r.diagnostics_.empty());
    }

    SECTION("Parse from Lexer") {
        cesil::Lexer lex("LOAD +2\nHALT\n");
        cesil::ParseResult r = parser.parse(lex);
        REQUIRE(r.ok_);
        CHECK(r.instructions_.size() == 2);
        CHECK(r.instructions_[0].operand_.immediate_ == 2);
    }

    SECTION("Data sections") {
        const std::string src =
            "        LOAD +0\n"
            "%\n"
            "5\n"
            "*\n";
        const cesil::ParseResult r = parser.parse(src);
        REQUIRE(r.ok_);
        CHECK(r.data_.size() == 1);
        CHECK(r.data_[0] == 5);
    }
}
