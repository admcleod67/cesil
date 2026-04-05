#pragma once

#include "errors/Diagnostic.hpp"
#include "runtime/Hooks.hpp"
#include "runtime/Instruction.hpp"
#include "runtime/Io.hpp"

#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

namespace cesil {

/// Outcome of a full or partial interpreter run.
struct RunResult {
    /// True if execution completed without a fatal runtime error.
    bool ok_{false};
    /// Set when \ref ExecutionHooks::shouldBreak_ stopped execution before the current PC.
    bool stoppedAtBreakpoint_{false};
    std::vector<Diagnostic> diagnostics_{};
};

/// Executes a CESIL program held as a vector of \ref Instruction.
///
/// Owns execution state: accumulator, program counter, and named store locations.
/// Composes an \ref IoHost for I/O and optional \ref ExecutionHooks for debugging.
class Interpreter {
   public:
    /// \param io Host I/O; must outlive the interpreter if stored by reference.
    /// \param hooks Optional stepping/breakpoint callbacks.
    explicit Interpreter(IoHost& io, ExecutionHooks hooks = {});

    /// Replace the loaded program and reset execution state.
    ///
    /// \param program Instruction sequence to run.
    /// \param runtimeData Values consumed by \c IN in order.
    /// \param labelIndices Map from line label to instruction index (from \ref ParseResult).
    void load(std::vector<Instruction> program, std::vector<int> runtimeData = {},
              std::unordered_map<std::string, std::size_t> labelIndices = {});

    /// Reset PC, accumulator, and store without changing the loaded program.
    void reset();

    /// Run from the current PC until HALT, end of program, error, or breakpoint hook.
    RunResult run();

    /// Current accumulator value.
    int accumulator() const { return accumulator_; }

    /// Index of the next instruction in the program vector.
    std::size_t programCounter() const { return pc_; }

    /// Named integer store (CESIL variables).
    const std::unordered_map<std::string, int>& store() const { return store_; }

    /// Loaded program (read-only inspection).
    const std::vector<Instruction>& program() const { return program_; }

   private:
    IoHost& io_;
    ExecutionHooks hooks_;
    std::vector<Instruction> program_{};
    std::vector<int> data_{};
    std::size_t dataPtr_{0};
    std::unordered_map<std::string, std::size_t> labelIndices_{};
    std::unordered_map<std::string, int> store_{};
    int accumulator_{0};
    std::size_t pc_{0};

    bool executeCurrent(bool& halted, RunResult& result);
    int loadOperandValue(const Operand& op, int lineNumber, RunResult& result);
    std::size_t resolveJumpTarget(const std::string& label, int lineNumber, RunResult& result);
};

}  // namespace cesil
