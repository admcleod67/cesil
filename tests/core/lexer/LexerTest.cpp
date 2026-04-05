#include <catch2/catch_test_macros.hpp>
#include "lexer/Lexer.hpp"
#include "lexer/Token.hpp"

#include <string>
#include <vector>

namespace {

void collectAll(const char* src, std::vector<cesil::Token>& out) {
    cesil::Lexer lex(src);
    for (;;) {
        cesil::Token t = lex.getNextToken();
        out.push_back(t);
        if (t.type_ == cesil::TokenType::EndOfFile) {
            break;
        }
    }
}

}  // namespace

TEST_CASE("Lexer basic tokens", "[lexer]") {
    SECTION("LOAD +0") {
        cesil::Lexer lex("LOAD +0");
        CHECK(lex.getNextToken().type_ == cesil::TokenType::Identifier);
        CHECK(lex.getNextToken().type_ == cesil::TokenType::Number);
        CHECK(lex.getNextToken().type_ == cesil::TokenType::EndOfFile);
    }

    SECTION("Whitespace and newlines") {
        cesil::Lexer lex("  \n\t\n  ");
        CHECK(lex.getNextToken().type_ == cesil::TokenType::Newline);
        CHECK(lex.getNextToken().type_ == cesil::TokenType::Newline);
        CHECK(lex.getNextToken().type_ == cesil::TokenType::EndOfFile);
    }

    SECTION("Comments") {
        cesil::Lexer lex("( full line comment\nLOAD");
        CHECK(lex.getNextToken().type_ == cesil::TokenType::Newline);
        CHECK(lex.getNextToken().type_ == cesil::TokenType::Identifier);
        CHECK(lex.getNextToken().type_ == cesil::TokenType::EndOfFile);
    }

    SECTION("Special characters") {
        cesil::Lexer lex("%\n*\n");
        CHECK(lex.getNextToken().type_ == cesil::TokenType::Percent);
        CHECK(lex.getNextToken().type_ == cesil::TokenType::Newline);
        CHECK(lex.getNextToken().type_ == cesil::TokenType::Star);
        CHECK(lex.getNextToken().type_ == cesil::TokenType::Newline);
        CHECK(lex.getNextToken().type_ == cesil::TokenType::EndOfFile);
    }

    SECTION("Strings and escapes") {
        cesil::Lexer lex(R"(PRINT "a\"b")");
        cesil::Token s = lex.getNextToken();
        CHECK(s.type_ == cesil::TokenType::Identifier);
        cesil::Token str = lex.getNextToken();
        CHECK(str.type_ == cesil::TokenType::String);
        CHECK(str.text_ == "a\"b");
        CHECK(lex.getNextToken().type_ == cesil::TokenType::EndOfFile);
    }

    SECTION("Labels and peek") {
        cesil::Lexer lex("LOOP:LOAD");
        CHECK(lex.getNextToken().type_ == cesil::TokenType::Label);
        CHECK(lex.peekToken().type_ == cesil::TokenType::Identifier);
        CHECK(lex.getNextToken().text_ == "LOAD");
        CHECK(lex.getNextToken().type_ == cesil::TokenType::EndOfFile);
    }

    SECTION("Unknown tokens") {
        std::vector<cesil::Token> toks;
        collectAll("?\n", toks);
        REQUIRE(toks.size() >= 3);
        CHECK(toks[0].type_ == cesil::TokenType::Unknown);
        CHECK(toks[1].type_ == cesil::TokenType::Newline);
        CHECK(toks[2].type_ == cesil::TokenType::EndOfFile);
    }
}
