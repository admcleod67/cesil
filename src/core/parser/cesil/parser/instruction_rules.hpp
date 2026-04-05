#pragma once

#include "cesil/opcode.hpp"

#include <string_view>

namespace cesil::instruction_rules {

bool lookup_opcode(std::string_view upper, OpCode& out);
bool needs_operand(OpCode op);
bool operand_must_be_label(OpCode op);

}  // namespace cesil::instruction_rules
