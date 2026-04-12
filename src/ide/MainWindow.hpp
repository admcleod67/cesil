#pragma once

#include <errors/Diagnostic.hpp>

#include <QMainWindow>
#include <QString>

#include <vector>

class QCloseEvent;
class QListWidget;
class QPlainTextEdit;
class QTabWidget;

class MainWindow final : public QMainWindow {
   public:
    explicit MainWindow(QWidget* parent = nullptr);

   protected:
    void closeEvent(QCloseEvent* event) override;

   private:
    void setupMenu();
    void updateWindowTitle();
    QString displayFileName() const;

    void openFile();
    bool saveFile();
    bool saveFileAs();
    bool writeToPath(const QString& path);
    bool handleUnsavedChanges();

    void checkSyntax();
    void runProgram();

    static void addDiagnosticsToList(QListWidget* list,
                                     const std::vector<cesil::Diagnostic>& diagnostics);

    QTabWidget* m_tabs{};
    QPlainTextEdit* m_editor{};
    QListWidget* m_errorsList{};
    QPlainTextEdit* m_output{};
    QString m_filePath;
};
