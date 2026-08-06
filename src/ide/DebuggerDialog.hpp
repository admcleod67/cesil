#pragma once

#include <parser/Parser.hpp>
#include <runtime/Interpreter.hpp>

#include "QtIoHost.hpp"

#include <QDialog>
#include <QLineEdit>
#include <QListWidget>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSlider>
#include <QTableWidget>
#include <QTimer>

#include <memory>

class QCloseEvent;

/// Visual CESIL-style debugger dialogue (Milestone 9 Stage 3).
class DebuggerDialog final : public QDialog {
    Q_OBJECT

   public:
    DebuggerDialog(cesil::ParseResult parsed, const QString& sourceText,
                   QWidget* parent = nullptr);

    void stopContinuousRun();
    bool isContinuousRunning() const { return continuousRunning_; }

   signals:
    void runningChanged(bool running);

   protected:
    void closeEvent(QCloseEvent* event) override;

   private slots:
    void onStep();
    void onRun();
    void onStop();
    void onReset();
    void onQuit();
    void onSpeedChanged(int value);
    void onTimerTick();

   private:
    void refreshUi();
    void setContinuousRunning(bool running);
    int timerIntervalMs() const;
    bool executeOneStep();
    void fillDataList();

    QPlainTextEdit* source_{};
    QTableWidget* variables_{};
    QLineEdit* accumulator_{};
    QListWidget* data_{};
    QPlainTextEdit* output_{};
    QSlider* speed_{};
    QPushButton* runButton_{};
    QPushButton* stepButton_{};
    QPushButton* stopButton_{};
    QPushButton* resetButton_{};
    QPushButton* quitButton_{};
    QTimer* runTimer_{};

    std::unique_ptr<QtIoHost> io_;
    std::unique_ptr<cesil::Interpreter> interpreter_;
    bool continuousRunning_{false};
};
