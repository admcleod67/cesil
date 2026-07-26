#include "MainWindow.hpp"
#include "QtIoHost.hpp"
#include "DiagnosticUtils.hpp"

#include <errors/Diagnostic.hpp>
#include <parser/Parser.hpp>
#include <runtime/Interpreter.hpp>
#include <runtime/Io.hpp>

#include <QAbstractItemView>
#include <QAction>
#include <QByteArray>
#include <QCloseEvent>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QKeySequence>
#include <QListWidget>
#include <QMenuBar>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QStyle>
#include <QTabWidget>
#include <QTextCursor>
#include <QToolBar>

#include <sstream>
#include <string>
#include <string_view>

namespace {

constexpr int kErrorsTabIndex = 1;
constexpr int kOutputTabIndex = 2;

}  // namespace

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    m_tabs = new QTabWidget(this);
    m_editor = new QPlainTextEdit(m_tabs);
    m_errorsList = new QListWidget(m_tabs);
    m_errorsList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_errorsList->setSelectionMode(QAbstractItemView::SingleSelection);

    m_output = new QPlainTextEdit(m_tabs);
    m_output->setReadOnly(true);

    m_tabs->addTab(m_editor, tr("Source"));
    m_tabs->addTab(m_errorsList, tr("Errors"));
    m_tabs->addTab(m_output, tr("Output"));
    setCentralWidget(m_tabs);

    connect(m_editor->document(), &QTextDocument::modificationChanged, this,
            &MainWindow::updateWindowTitle);

    createActions();
    createMenus();
    createToolBar();
    updateWindowTitle();
}

void MainWindow::createActions() {
    m_newAction = new QAction(tr("&New"), this);
    m_newAction->setShortcut(QKeySequence::New);
    m_newAction->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
    connect(m_newAction, &QAction::triggered, this, &MainWindow::newFile);

    m_openAction = new QAction(tr("&Open..."), this);
    m_openAction->setShortcut(QKeySequence::Open);
    m_openAction->setIcon(style()->standardIcon(QStyle::SP_DirOpenIcon));
    connect(m_openAction, &QAction::triggered, this, &MainWindow::openFile);

    m_saveAction = new QAction(tr("&Save"), this);
    m_saveAction->setShortcut(QKeySequence::Save);
    m_saveAction->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    connect(m_saveAction, &QAction::triggered, this,
            [this]() { static_cast<void>(saveFile()); });

    m_saveAsAction = new QAction(tr("Save &As..."), this);
    m_saveAsAction->setShortcut(QKeySequence::SaveAs);
    connect(m_saveAsAction, &QAction::triggered, this,
            [this]() { static_cast<void>(saveFileAs()); });

    m_quitAction = new QAction(tr("&Quit"), this);
    m_quitAction->setShortcut(QKeySequence::Quit);
    connect(m_quitAction, &QAction::triggered, this, &QWidget::close);

    m_checkAction = new QAction(tr("Check &syntax"), this);
    m_checkAction->setShortcut(QKeySequence(Qt::Key_F7));
    m_checkAction->setIcon(style()->standardIcon(QStyle::SP_DialogApplyButton));
    connect(m_checkAction, &QAction::triggered, this, &MainWindow::checkSyntax);

    m_runAction = new QAction(tr("&Run"), this);
    m_runAction->setShortcut(QKeySequence(Qt::Key_F5));
    m_runAction->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    connect(m_runAction, &QAction::triggered, this, &MainWindow::runProgram);
}

void MainWindow::createMenus() {
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(m_newAction);
    fileMenu->addAction(m_openAction);
    fileMenu->addAction(m_saveAction);
    fileMenu->addAction(m_saveAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(m_quitAction);

    QMenu* buildMenu = menuBar()->addMenu(tr("&Build"));
    buildMenu->addAction(m_checkAction);

    QMenu* runMenu = menuBar()->addMenu(tr("&Run"));
    runMenu->addAction(m_runAction);
}

void MainWindow::createToolBar() {
    QToolBar* toolBar = addToolBar(tr("Main"));
    toolBar->setMovable(false);
    toolBar->addAction(m_newAction);
    toolBar->addAction(m_openAction);
    toolBar->addAction(m_saveAction);
    toolBar->addSeparator();
    toolBar->addAction(m_checkAction);
    toolBar->addAction(m_runAction);
}

void MainWindow::newFile() {
    if (!handleUnsavedChanges()) {
        return;
    }

    m_editor->clear();
    m_filePath.clear();
    m_errorsList->clear();
    m_output->clear();
    m_editor->document()->setModified(false);
    updateWindowTitle();
}

void MainWindow::checkSyntax() {
    const QByteArray utf8 = m_editor->toPlainText().toUtf8();
    const std::string_view source(utf8.constData(),
                                  static_cast<std::size_t>(utf8.size()));

    const cesil::Parser parser;
    const cesil::ParseResult result = parser.parse(source);

    m_errorsList->clear();
    ::addDiagnosticsToList(m_errorsList, result.diagnostics_);

    if (m_errorsList->count() == 0) {
        if (result.ok_) {
            m_errorsList->addItem(tr("No issues."));
        } else {
            m_errorsList->addItem(
                tr("Compilation failed (no detailed diagnostics)."));
        }
    }

    m_tabs->setCurrentIndex(kErrorsTabIndex);
}

void MainWindow::runProgram() {
    m_output->clear();

    const QByteArray utf8 = m_editor->toPlainText().toUtf8();
    const std::string_view source(utf8.constData(),
                                  static_cast<std::size_t>(utf8.size()));

    cesil::Parser parser;
    cesil::ParseResult parsed = parser.parse(source);

    if (!parsed.ok_) {
        m_errorsList->clear();
        ::addDiagnosticsToList(m_errorsList, parsed.diagnostics_);
        if (m_errorsList->count() == 0) {
            m_errorsList->addItem(
                tr("Compilation failed (no detailed diagnostics)."));
        }
        m_tabs->setCurrentIndex(kErrorsTabIndex);
        return;
    }

    QtIoHost io(m_output);
    cesil::Interpreter interpreter(io);
    interpreter.load(std::move(parsed.instructions_), std::move(parsed.data_),
                       std::move(parsed.labelIndices_));

    const cesil::RunResult ran = interpreter.run();

    if (!ran.ok_) {
        m_errorsList->clear();
        ::addDiagnosticsToList(m_errorsList, ran.diagnostics_);
        if (m_errorsList->count() == 0) {
            m_errorsList->addItem(tr("Run failed (no detailed diagnostics)."));
        }
        m_tabs->setCurrentIndex(kErrorsTabIndex);
        return;
    }

    m_errorsList->clear();
    m_tabs->setCurrentIndex(kOutputTabIndex);
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
