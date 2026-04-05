#include "runtime/interpreter.hpp"

#include "runtime/opcode.hpp"

#include <limits>
#include <utility>

namespace cesil {

Interpreter::Interpreter(IoHost& io, ExecutionHooks hooks)
    : io_(io), hooks_(std::move(hooks)) {}

void Interpreter::load(std::vector<Instruction> program, std::vector<int> runtimeData,
                       std::unordered_map<std::string, std::size_t> labelIndices) {
    program_ = std::move(program);
    data_ = std::move(runtimeData);
    labelIndices_ = std::move(labelIndices);
    dataPtr_ = 0;
    accumulator_ = 0;
    pc_ = 0;
    store_.clear();
}

void Interpreter::reset() {
    accumulator_ = 0;
    pc_ = 0;
    store_.clear();
    dataPtr_ = 0;
}

int Interpreter::loadOperandValue(const Operand& op, int lineNumber, RunResult& result) {
    if (op.kind_ == OperandKind::Immediate) {
        return op.immediate_;
    }
    if (op.kind_ == OperandKind::Symbolic) {
        const auto it = store_.find(op.symbol_);
        if (it == store_.end()) {
            return 0;
        }
        return it->second;
    }
    pushDiagnostic(result.diagnostics_, DiagnosticSeverity::Error, "missing numeric operand",
                   lineNumber, 0);
    return 0;
}

std::size_t Interpreter::resolveJumpTarget(const std::string& label, int lineNumber,
                                             RunResult& result) {
    const auto it = labelIndices_.find(label);
    if (it == labelIndices_.end()) {
        pushDiagnostic(result.diagnostics_, DiagnosticSeverity::Error,
                       "unknown label '" + label + "'", lineNumber, 0);
        return std::numeric_limits<std::size_t>::max();
    }
    return it->second;
}

bool Interpreter::executeCurrent(bool& halted, RunResult& result) {
    if (pc_ >= program_.size()) {
        return true;
    }

    const Instruction& inst = program_[pc_];
    halted = false;

    auto requireSymbol = [&](const Operand& op) -> bool {
        if (op.kind_ == OperandKind::Symbolic && !op.symbol_.empty()) {
            return true;
        }
        pushDiagnostic(result.diagnostics_, DiagnosticSeverity::Error, "expected symbolic operand",
                       inst.lineNumber_, 0);
        return false;
    };

    switch (inst.opcode_) {
        case OpCode::Load: {
            if (inst.operand_.kind_ == OperandKind::None) {
                pushDiagnostic(result.diagnostics_, DiagnosticSeverity::Error,
                               "LOAD requires an operand", inst.lineNumber_, 0);
                return false;
            }
            accumulator_ = loadOperandValue(inst.operand_, inst.lineNumber_, result);
            if (!result.diagnostics_.empty()) {
                return false;
            }
            ++pc_;
            break;
        }
        case OpCode::Store: {
            if (!requireSymbol(inst.operand_)) {
                return false;
            }
            store_[inst.operand_.symbol_] = accumulator_;
            ++pc_;
            break;
        }
        case OpCode::In: {
            if (dataPtr_ >= data_.size()) {
                io_.writeString("*** PROGRAM REQUIRES MORE DATA ***");
                pushDiagnostic(result.diagnostics_, DiagnosticSeverity::Error,
                               "program requires more data (IN past end of data section)",
                               inst.lineNumber_, 0);
                return false;
            }
            accumulator_ = data_[dataPtr_++];
            ++pc_;
            break;
        }
        case OpCode::Add: {
            if (inst.operand_.kind_ == OperandKind::None) {
                pushDiagnostic(result.diagnostics_, DiagnosticSeverity::Error,
                               "ADD requires an operand", inst.lineNumber_, 0);
                return false;
            }
            accumulator_ += loadOperandValue(inst.operand_, inst.lineNumber_, result);
            if (!result.diagnostics_.empty()) {
                return false;
            }
            ++pc_;
            break;
        }
        case OpCode::Subtract: {
            if (inst.operand_.kind_ == OperandKind::None) {
                pushDiagnostic(result.diagnostics_, DiagnosticSeverity::Error,
                               "SUBTRACT requires an operand", inst.lineNumber_, 0);
                return false;
            }
            accumulator_ -= loadOperandValue(inst.operand_, inst.lineNumber_, result);
            if (!result.diagnostics_.empty()) {
                return false;
            }
            ++pc_;
            break;
        }
        case OpCode::Multiply: {
            if (inst.operand_.kind_ == OperandKind::None) {
                pushDiagnostic(result.diagnostics_, DiagnosticSeverity::Error,
                               "MULTIPLY requires an operand", inst.lineNumber_, 0);
                return false;
            }
            accumulator_ *= loadOperandValue(inst.operand_, inst.lineNumber_, result);
            if (!result.diagnostics_.empty()) {
                return false;
            }
            ++pc_;
            break;
        }
        case OpCode::Divide: {
            if (inst.operand_.kind_ == OperandKind::None) {
                pushDiagnostic(result.diagnostics_, DiagnosticSeverity::Error,
                               "DIVIDE requires an operand", inst.lineNumber_, 0);
                return false;
            }
            const int divisor = loadOperandValue(inst.operand_, inst.lineNumber_, result);
            if (!result.diagnostics_.empty()) {
                return false;
            }
            if (divisor == 0) {
                io_.writeString("*** DIVISION BY ZERO ***");
                pushDiagnostic(result.diagnostics_, DiagnosticSeverity::Error, "division by zero",
                               inst.lineNumber_, 0);
                return false;
            }
            accumulator_ /= divisor;
            ++pc_;
            break;
        }
        case OpCode::Jump: {
            if (!requireSymbol(inst.operand_)) {
                return false;
            }
            const std::size_t target = resolveJumpTarget(inst.operand_.symbol_, inst.lineNumber_, result);
            if (target == std::numeric_limits<std::size_t>::max()) {
                return false;
            }
            pc_ = target;
            break;
        }
        case OpCode::JiZero: {
            if (!requireSymbol(inst.operand_)) {
                return false;
            }
            if (accumulator_ == 0) {
                const std::size_t target =
                    resolveJumpTarget(inst.operand_.symbol_, inst.lineNumber_, result);
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
            if (!requireSymbol(inst.operand_)) {
                return false;
            }
            if (accumulator_ < 0) {
                const std::size_t target =
                    resolveJumpTarget(inst.operand_.symbol_, inst.lineNumber_, result);
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
            io_.writeInt(accumulator_);
            ++pc_;
            break;
        case OpCode::Line:
            io_.writeLine();
            ++pc_;
            break;
        case OpCode::Print: {
            if (inst.operand_.kind_ != OperandKind::Symbolic) {
                pushDiagnostic(result.diagnostics_, DiagnosticSeverity::Error,
                               "PRINT requires a quoted string operand", inst.lineNumber_, 0);
                return false;
            }
            io_.writeString(inst.operand_.symbol_);
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
        result.ok_ = true;
        return result;
    }

    while (pc_ < program_.size()) {
        if (hooks_.beforeInstruction_) {
            hooks_.beforeInstruction_(*this);
        }
        // TODO: map pc to source locations when breakpoint metadata exists (see ExecutionHooks).
        if (hooks_.shouldBreak_ && hooks_.shouldBreak_(pc_)) {
            result.ok_ = true;
            result.stoppedAtBreakpoint_ = true;
            return result;
        }

        bool halted = false;
        if (!executeCurrent(halted, result)) {
            result.ok_ = false;
            return result;
        }

        if (hooks_.afterInstruction_) {
            hooks_.afterInstruction_(*this);
        }

        if (halted) {
            result.ok_ = true;
            return result;
        }
    }

    result.ok_ = true;
    return result;
}

}  // namespace cesil
