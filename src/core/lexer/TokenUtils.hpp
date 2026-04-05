#pragma once

#include <string>
#include <string_view>

namespace cesil::lexer {

std::string_view trimView(std::string_view s);
bool isBlank(std::string_view source);
std::string toUpperCopy(std::string_view s);
bool isValidIdentifier(std::string_view name);
bool parseSignedInteger(std::string_view text, int& out);

}  // namespace cesil::lexer
