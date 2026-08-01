#include "errors/Diagnostic.hpp"

#include <algorithm>
#include <climits>
#include <cstddef>
#include <ostream>
#include <utility>
#include <vector>

namespace cesil {

namespace {

const char* severityLabel(DiagnosticSeverity s) {
    switch (s) {
        case DiagnosticSeverity::Error:
            return "error";
        case DiagnosticSeverity::Warning:
            return "warning";
        case DiagnosticSeverity::Note:
            return "note";
    }
    return "diagnostic";
}

int sortLineKey(int line) { return line > 0 ? line : INT_MAX; }

int sortColumnKey(int column) { return column > 0 ? column : INT_MAX; }

}  // namespace

void printDiagnostic(std::ostream& out, const Diagnostic& d) {
    out << severityLabel(d.severity_) << ": " << d.message_;
    if (d.line_ > 0) {
        out << " (line " << d.line_;
        if (d.column_ > 0) {
            out << ", column " << d.column_;
        }
        out << ')';
    }
    out << '\n';
}

void pushDiagnostic(std::vector<Diagnostic>& sink, DiagnosticSeverity severity,
                    std::string message, int line, int column) {
    sink.push_back(Diagnostic{severity, std::move(message), line, column});
}

void finalizeDiagnostics(std::vector<Diagnostic>& diagnostics) {
    if (diagnostics.empty()) {
        return;
    }

    std::vector<std::pair<Diagnostic, std::size_t>> indexed;
    indexed.reserve(diagnostics.size());
    for (std::size_t i = 0; i < diagnostics.size(); ++i) {
        indexed.emplace_back(std::move(diagnostics[i]), i);
    }
    diagnostics.clear();

    std::stable_sort(indexed.begin(), indexed.end(),
                     [](const std::pair<Diagnostic, std::size_t>& a,
                        const std::pair<Diagnostic, std::size_t>& b) {
                         const int lineA = sortLineKey(a.first.line_);
                         const int lineB = sortLineKey(b.first.line_);
                         if (lineA != lineB) {
                             return lineA < lineB;
                         }
                         const int colA = sortColumnKey(a.first.column_);
                         const int colB = sortColumnKey(b.first.column_);
                         if (colA != colB) {
                             return colA < colB;
                         }
                         return a.second < b.second;
                     });

    for (auto& entry : indexed) {
        const Diagnostic& d = entry.first;
        if (!diagnostics.empty()) {
            const Diagnostic& prev = diagnostics.back();
            if (prev.line_ == d.line_ && prev.column_ == d.column_ && prev.message_ == d.message_) {
                continue;
            }
        }
        diagnostics.push_back(std::move(entry.first));
    }

    if (diagnostics.size() > kMaxCompilationDiagnostics) {
        diagnostics.resize(kMaxCompilationDiagnostics - 1);
        pushDiagnostic(diagnostics, DiagnosticSeverity::Error, "further compilation errors suppressed", 0, 0);
    }
}

}  // namespace cesil
