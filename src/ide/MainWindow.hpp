#pragma once

#include <QMainWindow>
#include <QString>

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

    QTabWidget* m_tabs{};
    QPlainTextEdit* m_editor{};
    QListWidget* m_errorsList{};
    QString m_filePath;
};
