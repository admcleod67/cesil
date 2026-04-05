#include "runtime/Opcode.hpp"

namespace cesil {

const char* toString(OpCode op) noexcept {
    switch (op) {
        case OpCode::Load:
            return "LOAD";
        case OpCode::Store:
            return "STORE";
        case OpCode::In:
            return "IN";
        case OpCode::Add:
            return "ADD";
        case OpCode::Subtract:
            return "SUBTRACT";
        case OpCode::Multiply:
            return "MULTIPLY";
        case OpCode::Divide:
            return "DIVIDE";
        case OpCode::Jump:
            return "JUMP";
        case OpCode::JiZero:
            return "JIZERO";
        case OpCode::JiNeg:
            return "JINEG";
        case OpCode::Out:
            return "OUT";
        case OpCode::Line:
            return "LINE";
        case OpCode::Print:
            return "PRINT";
        case OpCode::Halt:
            return "HALT";
        default:
            return "UNKNOWN";
    }
}

}  // namespace cesil
