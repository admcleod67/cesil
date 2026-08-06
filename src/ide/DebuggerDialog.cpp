#include "DebuggerDialog.hpp"
#include "QtIoHost.hpp"

#include <runtime/Interpreter.hpp>

#include <QCloseEvent>
#include <QColor>
#include <QFontDatabase>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSlider>
#include <QTableWidget>
#include <QTextBlock>
#include <QTextCursor>
#include <QTimer>
#include <QVBoxLayout>

#include <algorithm>
#include <vector>

namespace {

constexpr int kSpeedMinMs = 1;
constexpr int kSpeedMaxMs = 500;
constexpr int kSpeedDefaultMs = 250;

}  // namespace

DebuggerDialog::DebuggerDialog(cesil::ParseResult parsed, const QString& sourceText,
                               QWidget* parent)
    : QDialog(parent) {
    setWindowTitle(tr("Debugger"));
    setMinimumSize(640, 480);
    resize(720, 560);

    const QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);

    source_ = new QPlainTextEdit(this);
    source_->setReadOnly(true);
    source_->setFont(fixedFont);
    source_->setPlainText(sourceText);

    auto* sourceLabel = new QLabel(tr("Source:"), this);
    auto* sourceBox = new QVBoxLayout;
    sourceBox->addWidget(sourceLabel);
    sourceBox->addWidget(source_, 1);

    variables_ = new QTableWidget(0, 2, this);
    variables_->setHorizontalHeaderLabels({tr("Name"), tr("Value")});
    variables_->horizontalHeader()->setStretchLastSection(true);
    variables_->verticalHeader()->setVisible(false);
    variables_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    variables_->setSelectionMode(QAbstractItemView::NoSelection);

    auto* variablesLabel = new QLabel(tr("Variables:"), this);
    auto* variablesBox = new QVBoxLayout;
    variablesBox->addWidget(variablesLabel);
    variablesBox->addWidget(variables_, 1);

    accumulator_ = new QLineEdit(this);
    accumulator_->setReadOnly(true);
    auto* accumulatorLabel = new QLabel(tr("Accumulator:"), this);
    auto* accumulatorRow = new QHBoxLayout;
    accumulatorRow->addWidget(accumulatorLabel);
    accumulatorRow->addWidget(accumulator_, 1);

    data_ = new QListWidget(this);
    auto* dataLabel = new QLabel(tr("Data:"), this);
    auto* dataBox = new QVBoxLayout;
    dataBox->addWidget(dataLabel);
    dataBox->addWidget(data_, 1);

    speed_ = new QSlider(Qt::Vertical, this);
    speed_->setRange(kSpeedMinMs, kSpeedMaxMs);
    speed_->setValue(kSpeedDefaultMs);
    auto* slowLabel = new QLabel(tr("Slow"), this);
    auto* fastLabel = new QLabel(tr("Fast"), this);
    auto* speedLabel = new QLabel(tr("Speed:"), this);
    auto* speedBox = new QVBoxLayout;
    speedBox->addWidget(speedLabel);
    speedBox->addWidget(slowLabel);
    speedBox->addWidget(speed_, 1);
    speedBox->addWidget(fastLabel);
    connect(speed_, &QSlider::valueChanged, this, &DebuggerDialog::onSpeedChanged);

    auto* rightColumn = new QVBoxLayout;
    rightColumn->addLayout(variablesBox, 2);
    rightColumn->addLayout(accumulatorRow);
    rightColumn->addLayout(dataBox, 1);
    rightColumn->addLayout(speedBox);

    auto* topRow = new QHBoxLayout;
    topRow->addLayout(sourceBox, 3);
    topRow->addLayout(rightColumn, 2);

    runButton_ = new QPushButton(tr("Run"), this);
    stepButton_ = new QPushButton(tr("Step"), this);
    stopButton_ = new QPushButton(tr("Stop"), this);
    resetButton_ = new QPushButton(tr("Reset"), this);
    quitButton_ = new QPushButton(tr("Quit"), this);
    stopButton_->setEnabled(false);

    connect(runButton_, &QPushButton::clicked, this, &DebuggerDialog::onRun);
    connect(stepButton_, &QPushButton::clicked, this, &DebuggerDialog::onStep);
    connect(stopButton_, &QPushButton::clicked, this, &DebuggerDialog::onStop);
    connect(resetButton_, &QPushButton::clicked, this, &DebuggerDialog::onReset);
    connect(quitButton_, &QPushButton::clicked, this, &DebuggerDialog::onQuit);

    auto* buttonRow = new QHBoxLayout;
    buttonRow->addStretch();
    buttonRow->addWidget(runButton_);
    buttonRow->addWidget(stepButton_);
    buttonRow->addWidget(stopButton_);
    buttonRow->addWidget(resetButton_);
    buttonRow->addWidget(quitButton_);
    buttonRow->addStretch();

    output_ = new QPlainTextEdit(this);
    output_->setReadOnly(true);
    output_->setFont(fixedFont);
    auto* outputLabel = new QLabel(tr("Output:"), this);
    auto* outputBox = new QVBoxLayout;
    outputBox->addWidget(outputLabel);
    outputBox->addWidget(output_, 1);

    auto* layout = new QVBoxLayout(this);
    layout->addLayout(topRow, 3);
    layout->addLayout(buttonRow);
    layout->addLayout(outputBox, 2);

    io_ = std::make_unique<QtIoHost>(output_);
    interpreter_ = std::make_unique<cesil::Interpreter>(*io_);
    interpreter_->load(std::move(parsed.instructions_), std::move(parsed.data_),
                       std::move(parsed.labelIndices_));

    runTimer_ = new QTimer(this);
    runTimer_->setInterval(timerIntervalMs());
    connect(runTimer_, &QTimer::timeout, this, &DebuggerDialog::onTimerTick);

    fillDataList();
    refreshUi();
}

void DebuggerDialog::stopContinuousRun() {
    if (!continuousRunning_) {
        return;
    }
    runTimer_->stop();
    setContinuousRunning(false);
}

void DebuggerDialog::closeEvent(QCloseEvent* event) {
    stopContinuousRun();
    QDialog::closeEvent(event);
}

void DebuggerDialog::onStep() {
    if (continuousRunning_) {
        return;
    }
    executeOneStep();
}

void DebuggerDialog::onRun() {
    if (continuousRunning_) {
        return;
    }
    if (interpreter_->programCounter() >= interpreter_->program().size()) {
        return;
    }
    setContinuousRunning(true);
    runTimer_->start(timerIntervalMs());
}

void DebuggerDialog::onStop() {
    stopContinuousRun();
}

void DebuggerDialog::onReset() {
    stopContinuousRun();
    interpreter_->reset();
    output_->clear();
    refreshUi();
}

void DebuggerDialog::onQuit() {
    stopContinuousRun();
    close();
}

void DebuggerDialog::onSpeedChanged(int) {
    if (continuousRunning_) {
        runTimer_->setInterval(timerIntervalMs());
    }
}

void DebuggerDialog::onTimerTick() {
    if (!executeOneStep()) {
        stopContinuousRun();
    }
}

void DebuggerDialog::setContinuousRunning(bool running) {
    if (continuousRunning_ == running) {
        return;
    }
    continuousRunning_ = running;
    stopButton_->setEnabled(running);
    runButton_->setEnabled(!running &&
                           interpreter_->programCounter() < interpreter_->program().size());
    stepButton_->setEnabled(!running);
    emit runningChanged(running);
}

int DebuggerDialog::timerIntervalMs() const {
    return speed_->value();
}

bool DebuggerDialog::executeOneStep() {
    const cesil::RunResult result = interpreter_->step();
    refreshUi();

    const bool atEnd = interpreter_->programCounter() >= interpreter_->program().size();

    if (!result.ok_) {
        if (continuousRunning_) {
            setContinuousRunning(false);
        } else {
            runButton_->setEnabled(!atEnd);
        }
        return false;
    }

    if (atEnd) {
        if (continuousRunning_) {
            runTimer_->stop();
            setContinuousRunning(false);
        } else {
            runButton_->setEnabled(false);
        }
    }

    return !atEnd;
}

void DebuggerDialog::fillDataList() {
    data_->clear();
    for (int value : interpreter_->data()) {
        data_->addItem(QString::number(value));
    }
}

void DebuggerDialog::refreshUi() {
    accumulator_->setText(QString::number(interpreter_->accumulator()));

    const auto& store = interpreter_->store();
    std::vector<std::string> names;
    names.reserve(store.size());
    for (const auto& [name, value] : store) {
        names.push_back(name);
    }
    std::sort(names.begin(), names.end());

    variables_->setRowCount(static_cast<int>(names.size()));
    for (int row = 0; row < static_cast<int>(names.size()); ++row) {
        const std::string& name = names[static_cast<std::size_t>(row)];
        variables_->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(name)));
        variables_->setItem(
            row, 1,
            new QTableWidgetItem(QString::number(store.at(name))));
    }

    const int line = interpreter_->sourceLineAtPc();
    QList<QTextEdit::ExtraSelection> extra;
    if (line > 0) {
        for (QTextBlock block = source_->document()->begin(); block.isValid();
             block = block.next()) {
            if (block.blockNumber() + 1 == line) {
                QTextEdit::ExtraSelection selection;
                selection.format.setBackground(QColor(173, 216, 230));
                selection.cursor = QTextCursor(block);
                selection.cursor.clearSelection();
                selection.cursor.movePosition(QTextCursor::EndOfBlock,
                                              QTextCursor::KeepAnchor);
                extra.append(selection);
                source_->setTextCursor(selection.cursor);
                source_->ensureCursorVisible();
                break;
            }
        }
    }
    source_->setExtraSelections(extra);

    const bool atEnd = interpreter_->programCounter() >= interpreter_->program().size();
    if (!continuousRunning_) {
        runButton_->setEnabled(!atEnd);
    }
}
