#include "lexer/token_utils.hpp"

#include <cctype>

namespace cesil::lexer {

std::string_view trim_view(std::string_view s) {
    while (!s.empty() && std::isspace(static_cast<unsigned char>(s.front()))) {
        s.remove_prefix(1);
    }
    while (!s.empty() && std::isspace(static_cast<unsigned char>(s.back()))) {
        s.remove_suffix(1);
    }
    return s;
}

bool is_blank(std::string_view source) { return trim_view(source).empty(); }

std::string to_upper_copy(std::string_view s) {
    std::string out;
    out.reserve(s.size());
    for (char ch : s) {
        out.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(ch))));
    }
    return out;
}

bool is_valid_identifier(std::string_view name) {
    if (name.empty() || name.size() > 6) {
        return false;
    }
    if (!std::isalpha(static_cast<unsigned char>(name[0]))) {
        return false;
    }
    for (std::size_t i = 1; i < name.size(); ++i) {
        if (!std::isalnum(static_cast<unsigned char>(name[i]))) {
            return false;
        }
    }
    return true;
}

bool parse_signed_integer(std::string_view text, int& out) {
    text = trim_view(text);
    if (text.empty()) {
        return false;
    }
    bool neg = false;
    std::size_t i = 0;
    if (text[i] == '+') {
        ++i;
    } else if (text[i] == '-') {
        neg = true;
        ++i;
    }
    if (i >= text.size()) {
        return false;
    }
    long long acc = 0;
    for (; i < text.size(); ++i) {
        if (!std::isdigit(static_cast<unsigned char>(text[i]))) {
            return false;
        }
        acc = acc * 10 + (text[i] - '0');
        const long long maxv = neg ? 2147483648LL : 2147483647LL;
        if (acc > maxv) {
            return false;
        }
    }
    if (neg) {
        out = -static_cast<int>(acc);
    } else {
        out = static_cast<int>(acc);
    }
    return true;
}

}  // namespace cesil::lexer
