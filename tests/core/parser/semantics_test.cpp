#include <catch2/catch_test_macros.hpp>
#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "parser/validator.hpp"

#include <string>

TEST_CASE("Semantic validation", "[semantics]") {
    cesil::Parser parser;

    SECTION("Invalid store with immediate") {
        cesil::Lexer lex("        STORE +1\nHALT\n");
        cesil::ParseResult r;
        REQUIRE(parser.parseSyntax(lex, r));
        REQUIRE(r.syntax_ok);
        cesil::SemanticValidator sv;
        CHECK_FALSE(sv.run(r));
        CHECK_FALSE(r.semantic_ok);
    }

    SECTION("Valid load with immediate") {
        cesil::Lexer lex("        LOAD +0\nHALT\n");
        cesil::ParseResult r;
        REQUIRE(parser.parseSyntax(lex, r));
        cesil::SemanticValidator sv;
        CHECK(sv.run(r));
        CHECK(r.semantic_ok);
        CHECK(r.instructions[0].operand.immediate == 0);
    }

    SECTION("Overflow data value") {
        const std::string src =
            "        LOAD +0\n"
            "%\n"
            "9999999999\n"
            "*\n";
        cesil::Lexer lex(src);
        cesil::ParseResult r;
        REQUIRE(parser.parseSyntax(lex, r));
        cesil::SemanticValidator sv;
        CHECK_FALSE(sv.run(r));
    }
}
