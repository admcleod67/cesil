#include "cesil/parser/instruction_rules.hpp"

#include <string>
#include <unordered_map>

namespace cesil::instruction_rules {

namespace {

const std::unordered_map<std::string, OpCode>& opcode_map() {
    static const std::unordered_map<std::string, OpCode> kMap = {
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
    return kMap;
}

}  // namespace

bool lookup_opcode(std::string_view upper, OpCode& out) {
    const auto& map = opcode_map();
    const std::string key{upper};
    const auto it = map.find(key);
    if (it == map.end()) {
        return false;
    }
    out = it->second;
    return true;
}

bool needs_operand(OpCode op) {
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

bool operand_must_be_label(OpCode op) {
    switch (op) {
        case OpCode::Jump:
        case OpCode::JiZero:
        case OpCode::JiNeg:
            return true;
        default:
            return false;
    }
}

}  // namespace cesil::instruction_rules
