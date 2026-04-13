#include "QtIoHost.hpp"

#include <QPlainTextEdit>
#include <QTextCursor>

QtIoHost::QtIoHost(QPlainTextEdit* output) : output_(output) {}

int QtIoHost::readInt() {
    return 0;
}

void QtIoHost::writeInt(int value) {
    append(QString::number(value));
}

void QtIoHost::writeString(const std::string& text) {
    append(QString::fromStdString(text));
}

void QtIoHost::writeLine() {
    append(QStringLiteral("\n"));
}

void QtIoHost::append(const QString& chunk) {
    QTextCursor cursor(output_->document());
    cursor.movePosition(QTextCursor::End);
    cursor.insertText(chunk);
}
