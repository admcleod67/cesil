#include "../../include/cesil/interpreter/interpreter.hpp"

#include "cesil/opcode.hpp"

#include <limits>
#include <utility>

namespace cesil {

Interpreter::Interpreter(IoHost& io, ExecutionHooks hooks)
    : io_(io), hooks_(std::move(hooks)) {}

void Interpreter::load(std::vector<Instruction> program, std::vector<int> runtime_data,
                       std::unordered_map<std::string, std::size_t> label_indices) {
    program_ = std::move(program);
    data_ = std::move(runtime_data);
    label_indices_ = std::move(label_indices);
    data_ptr_ = 0;
    accumulator_ = 0;
    pc_ = 0;
    store_.clear();
}

void Interpreter::reset() {
    accumulator_ = 0;
    pc_ = 0;
    store_.clear();
    data_ptr_ = 0;
}

int Interpreter::load_operand_value(const Operand& op, int line_number, RunResult& result) {
    if (op.kind == OperandKind::Immediate) {
        return op.immediate;
    }
    if (op.kind == OperandKind::Symbolic) {
        const auto it = store_.find(op.symbol);
        if (it == store_.end()) {
            return 0;
        }
        return it->second;
    }
    push_diagnostic(result.diagnostics, DiagnosticSeverity::Error, "missing numeric operand",
                    line_number, 0);
    return 0;
}

std::size_t Interpreter::resolve_jump_target(const std::string& label, int line_number,
                                             RunResult& result) {
    const auto it = label_indices_.find(label);
    if (it == label_indices_.end()) {
        push_diagnostic(result.diagnostics, DiagnosticSeverity::Error,
                        "unknown label '" + label + "'", line_number, 0);
        return std::numeric_limits<std::size_t>::max();
    }
    return it->second;
}

bool Interpreter::execute_current(bool& halted, RunResult& result) {
    if (pc_ >= program_.size()) {
        return true;
    }

    const Instruction& inst = program_[pc_];
    halted = false;

    auto require_symbol = [&](const Operand& op) -> bool {
        if (op.kind == OperandKind::Symbolic && !op.symbol.empty()) {
            return true;
        }
        push_diagnostic(result.diagnostics, DiagnosticSeverity::Error, "expected symbolic operand",
                        inst.lineNumber, 0);
        return false;
    };

    switch (inst.opcode) {
        case OpCode::Load: {
            if (inst.operand.kind == OperandKind::None) {
                push_diagnostic(result.diagnostics, DiagnosticSeverity::Error,
                                "LOAD requires an operand", inst.lineNumber, 0);
                return false;
            }
            accumulator_ = load_operand_value(inst.operand, inst.lineNumber, result);
            if (!result.diagnostics.empty()) {
                return false;
            }
            ++pc_;
            break;
        }
        case OpCode::Store: {
            if (!require_symbol(inst.operand)) {
                return false;
            }
            store_[inst.operand.symbol] = accumulator_;
            ++pc_;
            break;
        }
        case OpCode::In: {
            if (data_ptr_ >= data_.size()) {
                io_.write_string("*** PROGRAM REQUIRES MORE DATA ***");
                push_diagnostic(result.diagnostics, DiagnosticSeverity::Error,
                                "program requires more data (IN past end of data section)",
                                inst.lineNumber, 0);
                return false;
            }
            accumulator_ = data_[data_ptr_++];
            ++pc_;
            break;
        }
        case OpCode::Add: {
            if (inst.operand.kind == OperandKind::None) {
                push_diagnostic(result.diagnostics, DiagnosticSeverity::Error,
                                "ADD requires an operand", inst.lineNumber, 0);
                return false;
            }
            accumulator_ += load_operand_value(inst.operand, inst.lineNumber, result);
            if (!result.diagnostics.empty()) {
                return false;
            }
            ++pc_;
            break;
        }
        case OpCode::Subtract: {
            if (inst.operand.kind == OperandKind::None) {
                push_diagnostic(result.diagnostics, DiagnosticSeverity::Error,
                                "SUBTRACT requires an operand", inst.lineNumber, 0);
                return false;
            }
            accumulator_ -= load_operand_value(inst.operand, inst.lineNumber, result);
            if (!result.diagnostics.empty()) {
                return false;
            }
            ++pc_;
            break;
        }
        case OpCode::Multiply: {
            if (inst.operand.kind == OperandKind::None) {
                push_diagnostic(result.diagnostics, DiagnosticSeverity::Error,
                                "MULTIPLY requires an operand", inst.lineNumber, 0);
                return false;
            }
            accumulator_ *= load_operand_value(inst.operand, inst.lineNumber, result);
            if (!result.diagnostics.empty()) {
                return false;
            }
            ++pc_;
            break;
        }
        case OpCode::Divide: {
            if (inst.operand.kind == OperandKind::None) {
                push_diagnostic(result.diagnostics, DiagnosticSeverity::Error,
                                "DIVIDE requires an operand", inst.lineNumber, 0);
                return false;
            }
            const int divisor = load_operand_value(inst.operand, inst.lineNumber, result);
            if (!result.diagnostics.empty()) {
                return false;
            }
            if (divisor == 0) {
                io_.write_string("*** DIVISION BY ZERO ***");
                push_diagnostic(result.diagnostics, DiagnosticSeverity::Error, "division by zero",
                                inst.lineNumber, 0);
                return false;
            }
            accumulator_ /= divisor;
            ++pc_;
            break;
        }
        case OpCode::Jump: {
            if (!require_symbol(inst.operand)) {
                return false;
            }
            const std::size_t target = resolve_jump_target(inst.operand.symbol, inst.lineNumber, result);
            if (target == std::numeric_limits<std::size_t>::max()) {
                return false;
            }
            pc_ = target;
            break;
        }
        case OpCode::JiZero: {
            if (!require_symbol(inst.operand)) {
                return false;
            }
            if (accumulator_ == 0) {
                const std::size_t target =
                    resolve_jump_target(inst.operand.symbol, inst.lineNumber, result);
                if (target == std::numeric_limits<std::size_t>::max()) {
                    return false;
                }
                pc_ = target;
            } else {
                ++pc_;
            }
            break;
        }
        case OpCode::JiNeg: {
            if (!require_symbol(inst.operand)) {
                return false;
            }
            if (accumulator_ < 0) {
                const std::size_t target =
                    resolve_jump_target(inst.operand.symbol, inst.lineNumber, result);
                if (target == std::numeric_limits<std::size_t>::max()) {
                    return false;
                }
                pc_ = target;
            } else {
                ++pc_;
            }
            break;
        }
        case OpCode::Out:
            io_.write_int(accumulator_);
            ++pc_;
            break;
        case OpCode::Line:
            io_.write_line();
            ++pc_;
            break;
        case OpCode::Print: {
            if (inst.operand.kind != OperandKind::Symbolic) {
                push_diagnostic(result.diagnostics, DiagnosticSeverity::Error,
                                "PRINT requires a quoted string operand", inst.lineNumber, 0);
                return false;
            }
            io_.write_string(inst.operand.symbol);
            ++pc_;
            break;
        }
        case OpCode::Halt:
            halted = true;
            ++pc_;
            break;
    }
    return true;
}

RunResult Interpreter::run() {
    RunResult result;
    if (program_.empty()) {
        result.ok = true;
        return result;
    }

    while (pc_ < program_.size()) {
        if (hooks_.before_instruction) {
            hooks_.before_instruction(*this);
        }
        // TODO: map pc to source locations when breakpoint metadata exists (see ExecutionHooks).
        if (hooks_.should_break && hooks_.should_break(pc_)) {
            result.ok = true;
            result.stopped_at_breakpoint = true;
            return result;
        }

        bool halted = false;
        if (!execute_current(halted, result)) {
            result.ok = false;
            return result;
        }

        if (hooks_.after_instruction) {
            hooks_.after_instruction(*this);
        }

        if (halted) {
            result.ok = true;
            return result;
        }
    }

    result.ok = true;
    return result;
}

}  // namespace cesil
