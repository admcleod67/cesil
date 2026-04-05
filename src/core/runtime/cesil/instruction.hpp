#pragma once

#include "cesil/opcode.hpp"
#include "cesil/operand.hpp"

namespace cesil {

/// Single decoded CESIL instruction in intermediate representation.
///
/// TODO: binary encoding for CESIL CPU (opcode + immediate + symtab index) will live beside this IR.
///
/// Intended fixed encoding for a hypothetical CESIL CPU (conceptual layout):
/// - Word 0: low byte = static_cast<std::uint8_t>(opcode); remaining bits may pack
///   a small immediate or flags once the ISA is frozen.
/// - Word 1: immediate extension or zero when not used.
/// - Symbol table (out-of-band): index referring to \p operand.symbol when
///   \p operand.kind == OperandKind::Symbolic.
///
/// The in-memory \ref Instruction struct is the source of truth until an encoder exists.
struct Instruction {
    OpCode opcode{OpCode::Halt};
    Operand operand{};
    /// Source line number (1-based), or 0 if unknown.
    int lineNumber{0};
};

}  // namespace cesil
