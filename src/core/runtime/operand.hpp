#pragma once

#include <string>

namespace cesil {

/// Describes how an instruction operand should be interpreted.
enum class OperandKind {
    /// No operand (e.g. some future pseudo-ops or encoding padding).
    None,
    /// Signed immediate integer literal.
    Immediate,
    /// Symbolic reference (label, store name, or string id for PRINT).
    Symbolic,
};

/// Flexible operand representation for IR and future assembler/IDE integration.
///
/// Immediate values are carried in \p immediate; symbolic operands use \p symbol.
struct Operand {
    OperandKind kind{OperandKind::None};
    int immediate{0};
    std::string symbol{};
};

}  // namespace cesil
