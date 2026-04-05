#include "lexer/lexer.hpp"

#include <cctype>
#include <utility>

namespace cesil {

Lexer::Lexer(std::string_view source) : buffer_(source) {}

bool Lexer::atEnd() const { return pos_ >= buffer_.size(); }

char Lexer::peekChar() const {
    if (atEnd()) {
        return '\0';
    }
    return buffer_[pos_];
}

void Lexer::advanceOne() {
    if (atEnd()) {
        return;
    }
    const char c = buffer_[pos_++];
    if (c == '\r') {
        if (!atEnd() && peekChar() == '\n') {
            ++pos_;
        }
        ++line_;
        column_ = 1;
    } else if (c == '\n') {
        ++line_;
        column_ = 1;
    } else if (c == '\t') {
        column_ += 8 - ((column_ - 1) % 8);
    } else {
        ++column_;
    }
}

void Lexer::skipHorizontalSpace() {
    while (!atEnd()) {
        const char c = peekChar();
        if (c == ' ' || c == '\t') {
            advanceOne();
        } else {
            break;
        }
    }
}

Token Lexer::makeToken(TokenType type, std::string text, int line, int col) const {
    return Token{type, std::move(text), line, col};
}

Token Lexer::readTokenImpl() {
    for (;;) {
        skipHorizontalSpace();

        if (atEnd()) {
            return makeToken(TokenType::EndOfFile, {}, line_, column_);
        }

        if (!lineHasStartedToken_ && peekChar() == '(') {
            advanceOne();
            while (!atEnd() && peekChar() != '\r' && peekChar() != '\n') {
                advanceOne();
            }
            if (!atEnd()) {
                const int nlLine = line_;
                const int nlCol = column_;
                advanceOne();
                lineHasStartedToken_ = false;
                return makeToken(TokenType::Newline, "\n", nlLine, nlCol);
            }
            lineHasStartedToken_ = false;
            return makeToken(TokenType::EndOfFile, {}, line_, column_);
        }

        const char c = peekChar();
        if (c == '\r' || c == '\n') {
            const int nlLine = line_;
            const int nlCol = column_;
            advanceOne();
            lineHasStartedToken_ = false;
            return makeToken(TokenType::Newline, "\n", nlLine, nlCol);
        }

        const int tokLine = line_;
        const int tokCol = column_;
        lineHasStartedToken_ = true;

        if (c == '%') {
            advanceOne();
            return makeToken(TokenType::Percent, "%", tokLine, tokCol);
        }
        if (c == '*') {
            advanceOne();
            return makeToken(TokenType::Star, "*", tokLine, tokCol);
        }

        if (c == '"') {
            advanceOne();
            std::string value;
            while (!atEnd()) {
                char ch = peekChar();
                if (ch == '\\' && pos_ + 1 < buffer_.size()) {
                    advanceOne();
                    const char next = peekChar();
                    if (next == '"' || next == '\\') {
                        advanceOne();
                        value.push_back(next);
                        continue;
                    }
                    value.push_back('\\');
                    continue;
                }
                if (ch == '"') {
                    advanceOne();
                    return makeToken(TokenType::String, std::move(value), tokLine, tokCol);
                }
                advanceOne();
                value.push_back(ch);
            }
            return makeToken(TokenType::Unknown, std::move(value), tokLine, tokCol);
        }

        if (std::isalpha(static_cast<unsigned char>(c))) {
            std::string ident;
            while (!atEnd()) {
                const char ch = peekChar();
                if (!std::isalnum(static_cast<unsigned char>(ch))) {
                    break;
                }
                ident.push_back(ch);
                advanceOne();
            }
            if (!atEnd() && peekChar() == ':') {
                advanceOne();
                return makeToken(TokenType::Label, std::move(ident), tokLine, tokCol);
            }
            return makeToken(TokenType::Identifier, std::move(ident), tokLine, tokCol);
        }

        if (c == '+' || c == '-' || std::isdigit(static_cast<unsigned char>(c))) {
            std::string num;
            if (c == '+' || c == '-') {
                num.push_back(c);
                advanceOne();
            }
            if (atEnd() || !std::isdigit(static_cast<unsigned char>(peekChar()))) {
                return makeToken(TokenType::Unknown, std::move(num), tokLine, tokCol);
            }
            while (!atEnd() && std::isdigit(static_cast<unsigned char>(peekChar()))) {
                num.push_back(peekChar());
                advanceOne();
            }
            return makeToken(TokenType::Number, std::move(num), tokLine, tokCol);
        }

        std::string u(1, c);
        advanceOne();
        return makeToken(TokenType::Unknown, std::move(u), tokLine, tokCol);
    }
}

Token Lexer::getNextToken() {
    if (peekBuffer_) {
        Token t = std::move(*peekBuffer_);
        peekBuffer_.reset();
        return t;
    }
    return readTokenImpl();
}

Token Lexer::peekToken() {
    if (!peekBuffer_) {
        peekBuffer_ = readTokenImpl();
    }
    return *peekBuffer_;
}

}  // namespace cesil
