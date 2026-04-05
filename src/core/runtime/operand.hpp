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
/// Immediate values are carried in \p immediate_; symbolic operands use \p symbol_.
struct Operand {
    OperandKind kind_{OperandKind::None};
    int immediate_{0};
    std::string symbol_{};
};

}  // namespace cesil
