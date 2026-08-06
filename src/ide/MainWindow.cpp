#include "MainWindow.hpp"
#include "DiagnosticUtils.hpp"
#include "QtIoHost.hpp"
#include "SourceEditActions.hpp"
#include "SourceEditor.hpp"
#include "SourceEditorLayout.hpp"

#include <errors/Diagnostic.hpp>
#include <parser/Parser.hpp>
#include <runtime/Interpreter.hpp>
#include <runtime/Io.hpp>

#include <QAction>
#include <QApplication>
#include <QByteArray>
#include <QFileDialog>
#include <QFileInfo>
#include <QFontDatabase>
#include <QHeaderView>
#include <QKeySequence>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QStatusBar>
#include <QStyle>
#include <QTableView>
#include <QTabWidget>
#include <QTextBlock>
#include <QTextCursor>
#include <QToolBar>

#include <string_view>

namespace {

constexpr int kSourceTabIndex = 0;
constexpr int kErrorsTabIndex = 1;
constexpr int kOutputTabIndex = 2;

}  // namespace

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    m_tabs = new QTabWidget(this);
    m_editor = new SourceEditor(m_tabs);

    m_errorsModel = new DiagnosticModel(this);
    m_errorsView = new QTableView(m_tabs);
    m_errorsView->setModel(m_errorsModel);
    m_errorsView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_errorsView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_errorsView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_errorsView->setShowGrid(true);
    m_errorsView->setAlternatingRowColors(false);
    m_errorsView->verticalHeader()->setVisible(false);
    m_errorsView->horizontalHeader()->setStretchLastSection(true);
    m_errorsView->horizontalHeader()->setSectionResizeMode(
        static_cast<int>(DiagnosticColumn::Line), QHeaderView::ResizeToContents);
    m_errorsView->horizontalHeader()->setSectionResizeMode(
        static_cast<int>(DiagnosticColumn::Description), QHeaderView::Stretch);
    m_errorsView->setWordWrap(false);

    m_output = new QPlainTextEdit(m_tabs);
    m_output->setReadOnly(true);
    m_output->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));

    m_tabs->addTab(m_editor, tr("Source"));
    m_tabs->addTab(m_errorsView, tr("Errors"));
    m_tabs->addTab(m_output, tr("Output"));
    setCentralWidget(m_tabs);

    connect(m_editor->document(), &QTextDocument::modificationChanged, this,
            &MainWindow::updateWindowTitle);
    connect(m_editor->document(), &QTextDocument::contentsChanged, this,
            &MainWindow::onSourceContentsChanged);
    connect(m_editor, &QPlainTextEdit::cursorPositionChanged, this,
            &MainWindow::updateCursorPosition);
    connect(m_errorsView, &QAbstractItemView::activated, this,
            &MainWindow::navigateToDiagnostic);

    createActions();
    createMenus();
    createToolBar();
    createStatusBar();
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

    m_undoAction = new QAction(tr("&Undo"), this);
    m_undoAction->setShortcut(QKeySequence::Undo);
    m_redoAction = new QAction(tr("&Redo"), this);
    m_redoAction->setShortcut(QKeySequence::Redo);
    m_cutAction = new QAction(tr("Cu&t"), this);
    m_cutAction->setShortcut(QKeySequence::Cut);
    m_copyAction = new QAction(tr("&Copy"), this);
    m_copyAction->setShortcut(QKeySequence::Copy);
    m_pasteAction = new QAction(tr("&Paste"), this);
    m_pasteAction->setShortcut(QKeySequence::Paste);
    m_selectAllAction = new QAction(tr("Select &All"), this);
    m_selectAllAction->setShortcut(QKeySequence::SelectAll);
    bindSourceEditActions(m_editor,
                          SourceEditActions{m_undoAction, m_redoAction, m_cutAction,
                                            m_copyAction, m_pasteAction, m_selectAllAction});

    m_checkAction = new QAction(tr("&Compile"), this);
    m_checkAction->setShortcut(QKeySequence(Qt::Key_F7));
    m_checkAction->setIcon(style()->standardIcon(QStyle::SP_DialogApplyButton));
    connect(m_checkAction, &QAction::triggered, this, &MainWindow::checkSyntax);

    m_runAction = new QAction(tr("&Run"), this);
    m_runAction->setShortcut(QKeySequence(Qt::Key_F5));
    m_runAction->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    connect(m_runAction, &QAction::triggered, this, &MainWindow::runProgram);

    m_aboutAction = new QAction(tr("&About CESIL IDE..."), this);
    m_aboutAction->setMenuRole(QAction::AboutRole);
    connect(m_aboutAction, &QAction::triggered, this, &MainWindow::showAbout);
}

void MainWindow::createMenus() {
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(m_newAction);
    fileMenu->addAction(m_openAction);
    fileMenu->addAction(m_saveAction);
    fileMenu->addAction(m_saveAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(m_quitAction);

    QMenu* editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(m_undoAction);
    editMenu->addAction(m_redoAction);
    editMenu->addSeparator();
    editMenu->addAction(m_cutAction);
    editMenu->addAction(m_copyAction);
    editMenu->addAction(m_pasteAction);
    editMenu->addSeparator();
    editMenu->addAction(m_selectAllAction);

    QMenu* buildMenu = menuBar()->addMenu(tr("&Build"));
    buildMenu->addAction(m_checkAction);

    QMenu* runMenu = menuBar()->addMenu(tr("&Run"));
    runMenu->addAction(m_runAction);

    QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(m_aboutAction);
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

void MainWindow::createStatusBar() {
    statusBar();
    m_cursorLabel = new QLabel(tr("Ln 1, Col 1"));
    statusBar()->addPermanentWidget(m_cursorLabel);
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::updateCursorPosition() {
    const QTextCursor cursor = m_editor->textCursor();
    const QTextBlock block = cursor.block();
    const int visualCol =
        sourceEditorLayout::visualColumn(block.text().left(cursor.positionInBlock()));
    m_cursorLabel->setText(tr("Ln %1, Col %2")
                               .arg(cursor.blockNumber() + 1)
                               .arg(visualCol));
}

void MainWindow::onSourceContentsChanged() {
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::navigateToDiagnostic(const QModelIndex& index) {
    if (!index.isValid() || !m_errorsModel->isNavigable(index.row())) {
        return;
    }

    const cesil::Diagnostic diagnostic = m_errorsModel->diagnosticAt(index.row());
    QTextCursor cursor =
        cursorForDiagnostic(m_editor->document(), diagnostic.line_, diagnostic.column_);
    m_editor->setTextCursor(cursor);
    m_editor->ensureCursorVisible();
    m_tabs->setCurrentIndex(kSourceTabIndex);
    m_editor->setFocus(Qt::OtherFocusReason);
}

void MainWindow::showDiagnostics(
    const std::vector<cesil::Diagnostic>& diagnostics) {
    m_errorsModel->setDiagnostics(diagnostics);
}

void MainWindow::showFallbackDiagnostic(const QString& message) {
    m_errorsModel->setFallbackMessage(message);
}

void MainWindow::newFile() {
    if (!handleUnsavedChanges()) {
        return;
    }

    m_editor->clear();
    m_filePath.clear();
    m_errorsModel->clear();
    m_output->clear();
    m_editor->document()->setModified(false);
    updateWindowTitle();
    updateCursorPosition();
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::showAbout() {
    const QString text = tr(
                             "<h3>%1</h3>"
                             "<p>Version %2</p>"
                             "<p>Copyright (c) 2026 admcleod67</p>"
                             "<p>Licensed under the MIT License.</p>")
                             .arg(QApplication::applicationName(),
                                  QApplication::applicationVersion());
    QMessageBox::about(this, tr("About %1").arg(QApplication::applicationName()), text);
}

void MainWindow::checkSyntax() {
    const QByteArray utf8 = m_editor->toPlainText().toUtf8();
    const std::string_view source(utf8.constData(),
                                  static_cast<std::size_t>(utf8.size()));

    const cesil::Parser parser;
    const cesil::ParseResult result = parser.parse(source);

    if (result.ok_) {
        m_errorsModel->clear();
        statusBar()->showMessage(compilationErrorSummary(0));
    } else if (result.diagnostics_.empty()) {
        showFallbackDiagnostic(tr("Compilation failed (no detailed diagnostics)."));
        statusBar()->showMessage(tr("Compilation failed."));
    } else {
        showDiagnostics(result.diagnostics_);
        statusBar()->showMessage(compilationErrorSummary(
            static_cast<int>(result.diagnostics_.size())));
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
        if (parsed.diagnostics_.empty()) {
            showFallbackDiagnostic(
                tr("Compilation failed (no detailed diagnostics)."));
            statusBar()->showMessage(tr("Compilation failed."));
        } else {
            showDiagnostics(parsed.diagnostics_);
            statusBar()->showMessage(compilationErrorSummary(
                static_cast<int>(parsed.diagnostics_.size())));
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
        if (ran.diagnostics_.empty()) {
            showFallbackDiagnostic(tr("Run failed (no detailed diagnostics)."));
        } else {
            showDiagnostics(ran.diagnostics_);
        }
        m_tabs->setCurrentIndex(kErrorsTabIndex);
        statusBar()->showMessage(tr("Run failed."));
        return;
    }

    m_errorsModel->clear();
    m_tabs->setCurrentIndex(kOutputTabIndex);
    statusBar()->showMessage(tr("Program finished."));
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
    statusBar()->showMessage(tr("Saved."));
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
    m_errorsModel->clear();
    m_editor->document()->setModified(false);
    updateWindowTitle();
    updateCursorPosition();
    statusBar()->showMessage(tr("Ready"));
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
