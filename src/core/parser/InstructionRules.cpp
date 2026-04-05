#include "parser/InstructionRules.hpp"

#include <string>
#include <unordered_map>

namespace cesil::instructionRules {

namespace {

const std::unordered_map<std::string, OpCode>& opcodeMap() {
    static const std::unordered_map<std::string, OpCode> OPCODE_MAP = {
        {"LOAD", OpCode::Load},
        {"STORE", OpCode::Store},
        {"IN", OpCode::In},
        {"ADD", OpCode::Add},
        {"SUBTRACT", OpCode::Subtract},
        {"MULTIPLY", OpCode::Multiply},
        {"DIVIDE", OpCode::Divide},
        {"JUMP", OpCode::Jump},
        {"JIZERO", OpCode::JiZero},
        {"JINEG", OpCode::JiNeg},
        {"OUT", OpCode::Out},
        {"LINE", OpCode::Line},
        {"PRINT", OpCode::Print},
        {"HALT", OpCode::Halt},
    };
    return OPCODE_MAP;
}

}  // namespace

bool lookupOpcode(std::string_view upper, OpCode& out) {
    const auto& map = opcodeMap();
    const std::string key{upper};
    const auto it = map.find(key);
    if (it == map.end()) {
        return false;
    }
    out = it->second;
    return true;
}

bool needsOperand(OpCode op) {
    switch (op) {
        case OpCode::In:
        case OpCode::Out:
        case OpCode::Line:
        case OpCode::Halt:
            return false;
        default:
            return true;
    }
}

bool operandMustBeLabel(OpCode op) {
    switch (op) {
        case OpCode::Jump:
        case OpCode::JiZero:
        case OpCode::JiNeg:
            return true;
        default:
            return false;
    }
}

}  // namespace cesil::instructionRules
