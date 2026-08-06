#pragma once

#include <errors/Diagnostic.hpp>

#include <QMainWindow>
#include <QString>

#include <vector>

class DiagnosticModel;
class QAction;
class QCloseEvent;
class QLabel;
class QModelIndex;
class QPlainTextEdit;
class QTableView;
class QTabWidget;

class MainWindow final : public QMainWindow {
   public:
    explicit MainWindow(QWidget* parent = nullptr);

   protected:
    void closeEvent(QCloseEvent* event) override;

   private:
    void createActions();
    void createMenus();
    void createToolBar();
    void createStatusBar();
    void updateCursorPosition();
    void updateWindowTitle();
    void onSourceContentsChanged();
    void navigateToDiagnostic(const QModelIndex& index);
    void showDiagnostics(const std::vector<cesil::Diagnostic>& diagnostics);
    void showFallbackDiagnostic(const QString& message);
    QString displayFileName() const;

    void newFile();
    void openFile();
    bool saveFile();
    bool saveFileAs();
    bool writeToPath(const QString& path);
    bool handleUnsavedChanges();

    void checkSyntax();
    void runProgram();
    void showAbout();

    QTabWidget* m_tabs{};
    QPlainTextEdit* m_editor{};
    QTableView* m_errorsView{};
    DiagnosticModel* m_errorsModel{};
    QPlainTextEdit* m_output{};
    QLabel* m_cursorLabel{};
    QString m_filePath;

    QAction* m_newAction{};
    QAction* m_openAction{};
    QAction* m_saveAction{};
    QAction* m_saveAsAction{};
    QAction* m_quitAction{};
    QAction* m_undoAction{};
    QAction* m_redoAction{};
    QAction* m_cutAction{};
    QAction* m_copyAction{};
    QAction* m_pasteAction{};
    QAction* m_selectAllAction{};
    QAction* m_checkAction{};
    QAction* m_runAction{};
    QAction* m_aboutAction{};
};
