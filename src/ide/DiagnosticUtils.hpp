#pragma once

#include <errors/Diagnostic.hpp>
#include <QString>
#include <vector>

class QListWidget;

QString formatDiagnosticLine(const cesil::Diagnostic& d);

void addDiagnosticsToList(QListWidget* list,
                          const std::vector<cesil::Diagnostic>& diagnostics);
