#include "DiagnosticUtils.hpp"

#include <QListWidget>
#include <sstream>

QString formatDiagnosticLine(const cesil::Diagnostic& d) {
    std::ostringstream line;
    cesil::printDiagnostic(line, d);
    QString text = QString::fromStdString(line.str());
    if (text.endsWith(QLatin1Char('\n'))) {
        text.chop(1);
    }
    return text;
}

void addDiagnosticsToList(QListWidget* list,
                          const std::vector<cesil::Diagnostic>& diagnostics) {
    for (const cesil::Diagnostic& d : diagnostics) {
        list->addItem(formatDiagnosticLine(d));
    }
}
