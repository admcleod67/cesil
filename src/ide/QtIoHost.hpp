#pragma once

#include <runtime/Io.hpp>
#include <QString>

class QPlainTextEdit;

/// Routes interpreter output to a \c QPlainTextEdit (program channel).
///
/// Note: the current \c Interpreter reads \c IN values from the parsed data
/// section, not from \ref readInt; \ref readInt remains for the interface.
class QtIoHost final : public cesil::IoHost {
   public:
    explicit QtIoHost(QPlainTextEdit* output);

    int readInt() override;
    void writeInt(int value) override;
    void writeString(const std::string& text) override;
    void writeLine() override;

   private:
    void append(const QString& chunk);

    QPlainTextEdit* output_{};
};
