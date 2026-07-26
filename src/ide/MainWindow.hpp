#pragma once

#include <errors/Diagnostic.hpp>

#include <QMainWindow>
#include <QString>

#include <vector>

class QAction;
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
    void createActions();
    void createMenus();
    void createToolBar();
    void updateWindowTitle();
    QString displayFileName() const;

    void newFile();
    void openFile();
    bool saveFile();
    bool saveFileAs();
    bool writeToPath(const QString& path);
    bool handleUnsavedChanges();

    void checkSyntax();
    void runProgram();

    QTabWidget* m_tabs{};
    QPlainTextEdit* m_editor{};
    QListWidget* m_errorsList{};
    QPlainTextEdit* m_output{};
    QString m_filePath;

    QAction* m_newAction{};
    QAction* m_openAction{};
    QAction* m_saveAction{};
    QAction* m_saveAsAction{};
    QAction* m_quitAction{};
    QAction* m_checkAction{};
    QAction* m_runAction{};
};
