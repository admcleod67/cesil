#pragma once

#include <QMainWindow>
#include <QString>

class QCloseEvent;
class QPlainTextEdit;

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

    QPlainTextEdit* m_editor{};
    QString m_filePath;
};
