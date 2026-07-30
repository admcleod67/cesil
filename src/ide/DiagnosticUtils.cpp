#include "DiagnosticUtils.hpp"

#include <QTextBlock>
#include <QTextCursor>
#include <QTextDocument>

#include <algorithm>
#include <utility>

DiagnosticModel::DiagnosticModel(QObject* parent) : QAbstractTableModel(parent) {}

int DiagnosticModel::rowCount(const QModelIndex& parent) const {
    if (parent.isValid()) {
        return 0;
    }
    return static_cast<int>(m_diagnostics.size());
}

int DiagnosticModel::columnCount(const QModelIndex& parent) const {
    if (parent.isValid()) {
        return 0;
    }
    return static_cast<int>(DiagnosticColumn::Count);
}

QVariant DiagnosticModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() < 0 ||
        index.row() >= static_cast<int>(m_diagnostics.size())) {
        return {};
    }

    const cesil::Diagnostic& d = m_diagnostics[static_cast<std::size_t>(index.row())];

    switch (role) {
        case Qt::DisplayRole:
            if (index.column() == static_cast<int>(DiagnosticColumn::Line)) {
                if (d.line_ <= 0) {
                    return QString();
                }
                return QString::number(d.line_);
            }
            if (index.column() == static_cast<int>(DiagnosticColumn::Description)) {
                return QString::fromStdString(d.message_);
            }
            return {};
        case Qt::TextAlignmentRole:
            if (index.column() == static_cast<int>(DiagnosticColumn::Line) ||
                index.column() == static_cast<int>(DiagnosticColumn::Description)) {
                return QVariant::fromValue(Qt::AlignLeft | Qt::AlignVCenter);
            }
            return {};
        case LineRole:
            return d.line_;
        case ColumnRole:
            return d.column_;
        case SeverityRole:
            return static_cast<int>(d.severity_);
        case MessageRole:
            return QString::fromStdString(d.message_);
        case NavigableRole:
            return d.line_ > 0;
        default:
            return {};
    }
}

QVariant DiagnosticModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const {
    if (orientation != Qt::Horizontal) {
        return {};
    }
    const bool isVisibleColumn =
        section == static_cast<int>(DiagnosticColumn::Line) ||
        section == static_cast<int>(DiagnosticColumn::Description);
    if (role == Qt::TextAlignmentRole && isVisibleColumn) {
        return QVariant::fromValue(Qt::AlignLeft | Qt::AlignVCenter);
    }
    if (role != Qt::DisplayRole) {
        return {};
    }
    if (section == static_cast<int>(DiagnosticColumn::Line)) {
        return tr("Line");
    }
    if (section == static_cast<int>(DiagnosticColumn::Description)) {
        return tr("Description");
    }
    return {};
}

Qt::ItemFlags DiagnosticModel::flags(const QModelIndex& index) const {
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void DiagnosticModel::setDiagnostics(
    const std::vector<cesil::Diagnostic>& diagnostics) {
    beginResetModel();
    m_diagnostics = diagnostics;
    endResetModel();
}

void DiagnosticModel::setFallbackMessage(const QString& message) {
    beginResetModel();
    m_diagnostics.clear();
    m_diagnostics.push_back(cesil::Diagnostic{
        cesil::DiagnosticSeverity::Error, message.toStdString(), 0, 0});
    endResetModel();
}

void DiagnosticModel::clear() {
    beginResetModel();
    m_diagnostics.clear();
    endResetModel();
}

bool DiagnosticModel::isNavigable(int row) const {
    if (row < 0 || row >= static_cast<int>(m_diagnostics.size())) {
        return false;
    }
    return m_diagnostics[static_cast<std::size_t>(row)].line_ > 0;
}

cesil::Diagnostic DiagnosticModel::diagnosticAt(int row) const {
    if (row < 0 || row >= static_cast<int>(m_diagnostics.size())) {
        return {};
    }
    return m_diagnostics[static_cast<std::size_t>(row)];
}

QTextCursor cursorForDiagnostic(QTextDocument* document, int line, int column) {
    QTextCursor cursor(document);
    if (document == nullptr || line <= 0 || document->blockCount() == 0) {
        return cursor;
    }

    const int blockIndex =
        std::clamp(line, 1, document->blockCount()) - 1;
    const QTextBlock block = document->findBlockByNumber(blockIndex);
    cursor.setPosition(block.position());

    if (column > 0) {
        QTextCursor endOfBlock(cursor);
        endOfBlock.movePosition(QTextCursor::EndOfBlock);
        const int target = block.position() + (column - 1);
        cursor.setPosition(std::min(target, endOfBlock.position()));
    }
    return cursor;
}

QString compilationErrorSummary(int count) {
    if (count <= 0) {
        return QObject::tr("No compilation errors.");
    }
    if (count == 1) {
        return QObject::tr("1 compilation error");
    }
    return QObject::tr("%1 compilation errors").arg(count);
}
