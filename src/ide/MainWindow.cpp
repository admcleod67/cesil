#include "MainWindow.hpp"

#include <QAction>
#include <QCloseEvent>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QMenuBar>
#include <QMessageBox>
#include <QPlainTextEdit>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    m_editor = new QPlainTextEdit(this);
    setCentralWidget(m_editor);

    connect(m_editor->document(), &QTextDocument::modificationChanged, this,
            &MainWindow::updateWindowTitle);

    setupMenu();
    updateWindowTitle();
}

void MainWindow::setupMenu() {
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));

    QAction* openAction = fileMenu->addAction(tr("&Open..."));
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);

    QAction* saveAction = fileMenu->addAction(tr("&Save"));
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, &QAction::triggered, this, [this]() { static_cast<void>(saveFile()); });

    QAction* saveAsAction = fileMenu->addAction(tr("Save &As..."));
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    connect(saveAsAction, &QAction::triggered, this, [this]() { static_cast<void>(saveFileAs()); });

    fileMenu->addSeparator();

    QAction* quitAction = fileMenu->addAction(tr("&Quit"));
    quitAction->setShortcut(QKeySequence::Quit);
    connect(quitAction, &QAction::triggered, this, &QWidget::close);
}

QString MainWindow::displayFileName() const {
    if (m_filePath.isEmpty()) {
        return tr("Untitled");
    }
    return QFileInfo(m_filePath).fileName();
}

void MainWindow::updateWindowTitle() {
    QString title = tr("CESIL IDE — %1").arg(displayFileName());
    if (m_editor->document()->isModified()) {
        title += tr(" *");
    }
    setWindowTitle(title);
}

bool MainWindow::writeToPath(const QString& path) {
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, tr("CESIL IDE"),
                              tr("Could not write file:\n%1").arg(path));
        return false;
    }
    const QByteArray data = m_editor->toPlainText().toUtf8();
    if (file.write(data) != data.size()) {
        QMessageBox::critical(this, tr("CESIL IDE"),
                              tr("Could not write entire file:\n%1").arg(path));
        return false;
    }
    m_filePath = path;
    m_editor->document()->setModified(false);
    updateWindowTitle();
    return true;
}

bool MainWindow::saveFile() {
    if (m_filePath.isEmpty()) {
        return saveFileAs();
    }
    return writeToPath(m_filePath);
}

bool MainWindow::saveFileAs() {
    const QString path = QFileDialog::getSaveFileName(
        this, tr("Save As"), m_filePath.isEmpty() ? QString() : m_filePath,
        tr("CESIL programs (*.ces);;All files (*)"));
    if (path.isEmpty()) {
        return false;
    }
    return writeToPath(path);
}

void MainWindow::openFile() {
    if (!handleUnsavedChanges()) {
        return;
    }

    const QString path = QFileDialog::getOpenFileName(
        this, tr("Open"), QString(),
        tr("CESIL programs (*.ces);;All files (*)"));
    if (path.isEmpty()) {
        return;
    }

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, tr("CESIL IDE"),
                              tr("Could not open file:\n%1").arg(path));
        return;
    }

    m_editor->setPlainText(QString::fromUtf8(file.readAll()));
    m_filePath = path;
    m_editor->document()->setModified(false);
    updateWindowTitle();
}

bool MainWindow::handleUnsavedChanges() {
    if (!m_editor->document()->isModified()) {
        return true;
    }

    const QMessageBox::StandardButton reply = QMessageBox::question(
        this, tr("CESIL IDE"),
        tr("Save changes to \"%1\"?").arg(displayFileName()),
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
        QMessageBox::Save);

    if (reply == QMessageBox::Cancel) {
        return false;
    }
    if (reply == QMessageBox::Discard) {
        return true;
    }
    return saveFile();
}

void MainWindow::closeEvent(QCloseEvent* event) {
    if (!handleUnsavedChanges()) {
        event->ignore();
        return;
    }
    event->accept();
}
