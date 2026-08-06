#include "SourceEditor.hpp"

#include "SourceEditorLayout.hpp"
#include "SourceIndent.hpp"

#include <QEvent>
#include <QFontDatabase>
#include <QFontMetricsF>
#include <QKeyEvent>

SourceEditor::SourceEditor(QWidget* parent) : QPlainTextEdit(parent) {
    setTabChangesFocus(false);
    setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    updateTabStopDistance();
}

void SourceEditor::keyPressEvent(QKeyEvent* event) {
    QTextCursor cursor = textCursor();
    if (sourceIndent::handleTabKey(event, cursor)) {
        setTextCursor(cursor);
        event->accept();
        return;
    }
    QPlainTextEdit::keyPressEvent(event);
}

void SourceEditor::changeEvent(QEvent* event) {
    QPlainTextEdit::changeEvent(event);
    if (event != nullptr && event->type() == QEvent::FontChange) {
        updateTabStopDistance();
    }
}

void SourceEditor::updateTabStopDistance() {
    const qreal spaceWidth = QFontMetricsF(font()).horizontalAdvance(QLatin1Char(' '));
    setTabStopDistance(spaceWidth * sourceEditorLayout::kVisualTabWidth);
}
