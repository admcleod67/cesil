#pragma once

#include <errors/Diagnostic.hpp>

#include <QAbstractTableModel>
#include <QString>

#include <vector>

class QTextCursor;
class QTextDocument;

/// Visible columns in the Errors table.
enum class DiagnosticColumn {
    Line = 0,
    Description = 1,
    Count = 2,
};

/// Table model for structured CESIL diagnostics.
///
/// Visible columns are Line and Description. Line, column, severity, and
/// navigability remain available through custom roles.
class DiagnosticModel final : public QAbstractTableModel {
    Q_OBJECT

   public:
    enum Roles {
        LineRole = Qt::UserRole,
        ColumnRole,
        SeverityRole,
        MessageRole,
        NavigableRole,
    };

    explicit DiagnosticModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index,
                  int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    /// Replace the model contents with structured diagnostics.
    void setDiagnostics(const std::vector<cesil::Diagnostic>& diagnostics);

    /// Show a single unlocated, non-navigable explanatory row.
    void setFallbackMessage(const QString& message);

    /// Clear all rows (successful check / reset).
    void clear();

    bool isNavigable(int row) const;
    cesil::Diagnostic diagnosticAt(int row) const;

   private:
    std::vector<cesil::Diagnostic> m_diagnostics;
};

/// Clamp a one-based diagnostic location into a valid document cursor.
///
/// \p line must be greater than 0. A \p column of 0 means the start of the line.
QTextCursor cursorForDiagnostic(QTextDocument* document, int line, int column);

/// Status text for a successful check or a count of compilation errors.
QString compilationErrorSummary(int count);
