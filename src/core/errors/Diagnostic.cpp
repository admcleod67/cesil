#include "errors/Diagnostic.hpp"

#include <ostream>

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

}  // namespace cesil
