#pragma once

#include "runtime/opcode.hpp"

#include <string_view>

namespace cesil::instructionRules {

bool lookupOpcode(std::string_view upper, OpCode& out);
bool needsOperand(OpCode op);
bool operandMustBeLabel(OpCode op);

}  // namespace cesil::instructionRules
