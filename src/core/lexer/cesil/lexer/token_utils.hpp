#pragma once

#include <string>
#include <string_view>

namespace cesil::lexer {

std::string_view trim_view(std::string_view s);
bool is_blank(std::string_view source);
std::string to_upper_copy(std::string_view s);
bool is_valid_identifier(std::string_view name);
bool parse_signed_integer(std::string_view text, int& out);

}  // namespace cesil::lexer
