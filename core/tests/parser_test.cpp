#include <catch2/catch_test_macros.hpp>
#include "../include/cesil/lexer/lexer.hpp"
#include "cesil/opcode.hpp"
#include "cesil/operand.hpp"
#include "../include/cesil/parser/parser.hpp"

#include <string>

TEST_CASE("Parser basic functionality", "[parser]") {
    cesil::Parser parser;

    SECTION("Simple instructions") {
        const cesil::ParseResult r = parser.parse("LOOP    LOAD +1\nHALT\n");
        REQUIRE(r.ok);
        CHECK(r.instructions.size() == 2);
        CHECK(r.label_indices.count("LOOP") == 1);
        CHECK(r.instructions[0].opcode == cesil::OpCode::Load);
        CHECK(r.instructions[0].operand.kind == cesil::OperandKind::Immediate);
        CHECK(r.instructions[0].operand.immediate == 1);
        CHECK(r.instructions[1].opcode == cesil::OpCode::Halt);
    }

    SECTION("Labels and strings") {
        const cesil::ParseResult r = parser.parse("DONE: PRINT \"Hi\"\nHALT\n");
        REQUIRE(r.ok);
        CHECK(r.label_indices.count("DONE") == 1);
        CHECK(r.instructions[0].opcode == cesil::OpCode::Print);
        CHECK(r.instructions[0].operand.symbol == "Hi");
    }

    SECTION("Jumps and multi-line") {
        const std::string src =
            "        JUMP THERE\n"
            "THERE   HALT\n";
        const cesil::ParseResult r = parser.parse(src);
        REQUIRE(r.ok);
        CHECK(r.label_indices.count("THERE") == 1);
    }

    SECTION("Duplicate labels") {
        const std::string src =
            "HERE    HALT\n"
            "HERE    HALT\n";
        const cesil::ParseResult r = parser.parse(src);
        CHECK_FALSE(r.ok);
        CHECK_FALSE(r.diagnostics.empty());
    }

    SECTION("Unresolved jumps") {
        const std::string src = "        JUMP NOWHERE\nHALT\n";
        const cesil::ParseResult r = parser.parse(src);
        CHECK_FALSE(r.ok);
        CHECK_FALSE(r.diagnostics.empty());
    }

    SECTION("Parse from Lexer") {
        cesil::Lexer lex("LOAD +2\nHALT\n");
        cesil::ParseResult r = parser.parse(lex);
        REQUIRE(r.ok);
        CHECK(r.instructions.size() == 2);
        CHECK(r.instructions[0].operand.immediate == 2);
    }

    SECTION("Data sections") {
        const std::string src =
            "        LOAD +0\n"
            "%\n"
            "5\n"
            "*\n";
        const cesil::ParseResult r = parser.parse(src);
        REQUIRE(r.ok);
        CHECK(r.data.size() == 1);
        CHECK(r.data[0] == 5);
    }
}
