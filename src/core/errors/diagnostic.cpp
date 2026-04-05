#include "cesil/diagnostic.hpp"

#include <ostream>

namespace cesil {

namespace {

const char* severity_label(DiagnosticSeverity s) {
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

void print_diagnostic(std::ostream& out, const Diagnostic& d) {
    out << severity_label(d.severity) << ": " << d.message;
    if (d.line > 0) {
        out << " (line " << d.line;
        if (d.column > 0) {
            out << ", column " << d.column;
        }
        out << ')';
    }
    out << '\n';
}

void push_diagnostic(std::vector<Diagnostic>& sink, DiagnosticSeverity severity,
                     std::string message, int line, int column) {
    sink.push_back(Diagnostic{severity, std::move(message), line, column});
}

}  // namespace cesil
