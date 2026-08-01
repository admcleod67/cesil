#pragma once

#include <cstddef>
#include <ostream>
#include <string>
#include <vector>

namespace cesil {

/// Severity for structured compiler/runtime diagnostics.
enum class DiagnosticSeverity {
    Error,
    Warning,
    Note,
};

/// One diagnostic message with optional source location.
struct Diagnostic {
    DiagnosticSeverity severity_{DiagnosticSeverity::Error};
    std::string message_{};
    /// 1-based source line, or 0 if not applicable.
    int line_{0};
    /// 1-based column, or 0 if not applicable.
    int column_{0};
};

/// Maximum compilation diagnostics retained after finalization (Milestone 4).
inline constexpr std::size_t kMaxCompilationDiagnostics = 100;

/// Append a diagnostic to \p out (typically stderr).
///
/// \param out Output stream.
/// \param d Diagnostic to format.
void printDiagnostic(std::ostream& out, const Diagnostic& d);

/// Append a diagnostic to \p sink.
///
/// \param sink Collection to append to.
/// \param severity Severity level.
/// \param message Human-readable message.
/// \param line Optional 1-based line (0 = omit from output).
/// \param column Optional 1-based column (0 = omit from output).
void pushDiagnostic(std::vector<Diagnostic>& sink, DiagnosticSeverity severity,
                    std::string message, int line = 0, int column = 0);

/// Sort by source location, drop exact duplicates, and enforce
/// \ref kMaxCompilationDiagnostics (appending a suppression diagnostic when truncated).
void finalizeDiagnostics(std::vector<Diagnostic>& diagnostics);

}  // namespace cesil
