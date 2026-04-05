#include "lexer/lexer.hpp"

#include <cctype>
#include <utility>

namespace cesil {

Lexer::Lexer(std::string_view source) : buffer_(source) {}

bool Lexer::at_end() const { return pos_ >= buffer_.size(); }

char Lexer::peek_char() const {
    if (at_end()) {
        return '\0';
    }
    return buffer_[pos_];
}

void Lexer::advance_one() {
    if (at_end()) {
        return;
    }
    const char c = buffer_[pos_++];
    if (c == '\r') {
        if (!at_end() && peek_char() == '\n') {
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

void Lexer::skip_horizontal_space() {
    while (!at_end()) {
        const char c = peek_char();
        if (c == ' ' || c == '\t') {
            advance_one();
        } else {
            break;
        }
    }
}

Token Lexer::make_token(TokenType type, std::string text, int line, int col) const {
    return Token{type, std::move(text), line, col};
}

Token Lexer::read_token_impl() {
    for (;;) {
        skip_horizontal_space();

        if (at_end()) {
            return make_token(TokenType::EndOfFile, {}, line_, column_);
        }

        if (!line_has_started_token_ && peek_char() == '(') {
            advance_one();
            while (!at_end() && peek_char() != '\r' && peek_char() != '\n') {
                advance_one();
            }
            if (!at_end()) {
                const int nl_line = line_;
                const int nl_col = column_;
                advance_one();
                line_has_started_token_ = false;
                return make_token(TokenType::Newline, "\n", nl_line, nl_col);
            }
            line_has_started_token_ = false;
            return make_token(TokenType::EndOfFile, {}, line_, column_);
        }

        const char c = peek_char();
        if (c == '\r' || c == '\n') {
            const int nl_line = line_;
            const int nl_col = column_;
            advance_one();
            line_has_started_token_ = false;
            return make_token(TokenType::Newline, "\n", nl_line, nl_col);
        }

        const int tok_line = line_;
        const int tok_col = column_;
        line_has_started_token_ = true;

        if (c == '%') {
            advance_one();
            return make_token(TokenType::Percent, "%", tok_line, tok_col);
        }
        if (c == '*') {
            advance_one();
            return make_token(TokenType::Star, "*", tok_line, tok_col);
        }

        if (c == '"') {
            advance_one();
            std::string value;
            while (!at_end()) {
                char ch = peek_char();
                if (ch == '\\' && pos_ + 1 < buffer_.size()) {
                    advance_one();
                    const char next = peek_char();
                    if (next == '"' || next == '\\') {
                        advance_one();
                        value.push_back(next);
                        continue;
                    }
                    value.push_back('\\');
                    continue;
                }
                if (ch == '"') {
                    advance_one();
                    return make_token(TokenType::String, std::move(value), tok_line, tok_col);
                }
                advance_one();
                value.push_back(ch);
            }
            return make_token(TokenType::Unknown, std::move(value), tok_line, tok_col);
        }

        if (std::isalpha(static_cast<unsigned char>(c))) {
            std::string ident;
            while (!at_end()) {
                const char ch = peek_char();
                if (!std::isalnum(static_cast<unsigned char>(ch))) {
                    break;
                }
                ident.push_back(ch);
                advance_one();
            }
            if (!at_end() && peek_char() == ':') {
                advance_one();
                return make_token(TokenType::Label, std::move(ident), tok_line, tok_col);
            }
            return make_token(TokenType::Identifier, std::move(ident), tok_line, tok_col);
        }

        if (c == '+' || c == '-' || std::isdigit(static_cast<unsigned char>(c))) {
            std::string num;
            if (c == '+' || c == '-') {
                num.push_back(c);
                advance_one();
            }
            if (at_end() || !std::isdigit(static_cast<unsigned char>(peek_char()))) {
                return make_token(TokenType::Unknown, std::move(num), tok_line, tok_col);
            }
            while (!at_end() && std::isdigit(static_cast<unsigned char>(peek_char()))) {
                num.push_back(peek_char());
                advance_one();
            }
            return make_token(TokenType::Number, std::move(num), tok_line, tok_col);
        }

        std::string u(1, c);
        advance_one();
        return make_token(TokenType::Unknown, std::move(u), tok_line, tok_col);
    }
}

Token Lexer::getNextToken() {
    if (peek_buffer_) {
        Token t = std::move(*peek_buffer_);
        peek_buffer_.reset();
        return t;
    }
    return read_token_impl();
}

Token Lexer::peekToken() {
    if (!peek_buffer_) {
        peek_buffer_ = read_token_impl();
    }
    return *peek_buffer_;
}

}  // namespace cesil
