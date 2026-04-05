#pragma once

#include <cstddef>
#include <functional>

namespace cesil {

class Interpreter;

/// Optional callbacks for stepping, breakpoints, and external observation.
///
/// Empty \c std::function members are ignored by the interpreter.
struct ExecutionHooks {
    /// Invoked immediately before the instruction at the current PC executes.
    std::function<void(const Interpreter&)> beforeInstruction_{};

    /// Invoked immediately after an instruction completes.
    std::function<void(const Interpreter&)> afterInstruction_{};

    /// If set and returns true for the upcoming PC, the interpreter stops before
    /// executing that instruction (breakpoint). PC is the index in the program vector.
    ///
    /// TODO: breakpoint source map from Token/Instruction (line/column or IR index mapping).
    std::function<bool(std::size_t pc)> shouldBreak_{};
};

}  // namespace cesil
