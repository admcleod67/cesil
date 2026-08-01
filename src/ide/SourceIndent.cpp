#include "SourceIndent.hpp"

#include "SourceEditorLayout.hpp"

#include <QKeyEvent>
#include <QTextBlock>
#include <QTextCursor>
#include <QTextDocument>

#include <algorithm>

namespace sourceIndent {
namespace {

using sourceEditorLayout::contentStartColumn;
using sourceEditorLayout::kVisualTabWidth;
using sourceEditorLayout::leadingCharsToRemoveForOneStop;
using sourceEditorLayout::leadingSpacesForColumn;
using sourceEditorLayout::leadingWhitespaceLength;
using sourceEditorLayout::previousTabStop;
using sourceEditorLayout::spacesToNextStop;
using sourceEditorLayout::visualColumn;

QTextBlock lastSelectedBlock(const QTextCursor& cursor) {
    const int start = cursor.selectionStart();
    const int end = cursor.selectionEnd();
    if (end > start) {
        return cursor.document()->findBlock(end - 1);
    }
    return cursor.document()->findBlock(end);
}

void selectBlocks(QTextCursor& cursor, int firstBlockNumber, int lastBlockNumber) {
    QTextDocument* document = cursor.document();
    QTextBlock first = document->findBlockByNumber(firstBlockNumber);
    QTextBlock last = document->findBlockByNumber(lastBlockNumber);
    if (!first.isValid() || !last.isValid()) {
        return;
    }
    cursor.setPosition(first.position());
    cursor.setPosition(last.position() + last.length() - 1, QTextCursor::KeepAnchor);
}

void indentLinesByOneStop(QTextCursor& cursor) {
    QTextDocument* document = cursor.document();
    const int firstNumber = document->findBlock(cursor.selectionStart()).blockNumber();
    const int lastNumber = lastSelectedBlock(cursor).blockNumber();

    cursor.beginEditBlock();
    for (int number = firstNumber; number <= lastNumber; ++number) {
        QTextBlock block = document->findBlockByNumber(number);
        QTextCursor lineCursor(block);
        lineCursor.movePosition(QTextCursor::StartOfBlock);
        lineCursor.insertText(QString(kVisualTabWidth, QLatin1Char(' ')));
    }
    cursor.endEditBlock();

    selectBlocks(cursor, firstNumber, lastNumber);
}

void outdentLinesByOneStop(QTextCursor& cursor) {
    QTextDocument* document = cursor.document();
    const int firstNumber = document->findBlock(cursor.selectionStart()).blockNumber();
    const int lastNumber = lastSelectedBlock(cursor).blockNumber();

    cursor.beginEditBlock();
    for (int number = firstNumber; number <= lastNumber; ++number) {
        QTextBlock block = document->findBlockByNumber(number);
        const int removeCount = leadingCharsToRemoveForOneStop(block.text());
        if (removeCount <= 0) {
            continue;
        }
        QTextCursor lineCursor(block);
        lineCursor.movePosition(QTextCursor::StartOfBlock);
        lineCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, removeCount);
        lineCursor.removeSelectedText();
    }
    cursor.endEditBlock();

    selectBlocks(cursor, firstNumber, lastNumber);
}

void indentSingleLineTab(QTextCursor& cursor) {
    cursor.beginEditBlock();
    if (cursor.hasSelection()) {
        cursor.removeSelectedText();
    }
    const QTextBlock block = cursor.block();
    const int positionInBlock = cursor.position() - block.position();
    const int column = visualColumn(block.text().left(positionInBlock));
    const int spaces = spacesToNextStop(column);
    cursor.insertText(QString(spaces, QLatin1Char(' ')));
    cursor.endEditBlock();
}

void indentSingleLineShiftTab(QTextCursor& cursor) {
    const QTextBlock block = cursor.block();
    const QString text = block.text();
    const int lead = leadingWhitespaceLength(text);
    const int contentCol = contentStartColumn(text);
    const int targetCol = previousTabStop(contentCol);
    if (targetCol >= contentCol) {
        return;
    }

    cursor.beginEditBlock();
    QTextCursor lineCursor(block);
    lineCursor.movePosition(QTextCursor::StartOfBlock);
    if (lead > 0) {
        lineCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, lead);
        lineCursor.removeSelectedText();
    }
    const QString indent = leadingSpacesForColumn(targetCol);
    if (!indent.isEmpty()) {
        lineCursor.insertText(indent);
    }
    // Place caret at the start of content after outdent.
    lineCursor.movePosition(QTextCursor::StartOfBlock);
    lineCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor,
                            indent.size());
    cursor.endEditBlock();
    cursor = lineCursor;
}

}  // namespace

bool isMultilineSelection(const QTextCursor& cursor) {
    if (!cursor.hasSelection()) {
        return false;
    }
    const int startBlock =
        cursor.document()->findBlock(cursor.selectionStart()).blockNumber();
    const int endBlock = lastSelectedBlock(cursor).blockNumber();
    return startBlock != endBlock;
}

void indentTab(QTextCursor& cursor) {
    if (isMultilineSelection(cursor)) {
        indentLinesByOneStop(cursor);
    } else {
        indentSingleLineTab(cursor);
    }
}

void indentShiftTab(QTextCursor& cursor) {
    if (isMultilineSelection(cursor)) {
        outdentLinesByOneStop(cursor);
    } else {
        indentSingleLineShiftTab(cursor);
    }
}

bool handleTabKey(QKeyEvent* event, QTextCursor& cursor) {
    if (event == nullptr) {
        return false;
    }
    if (event->key() != Qt::Key_Tab && event->key() != Qt::Key_Backtab) {
        return false;
    }
    if (event->key() == Qt::Key_Backtab || event->modifiers().testFlag(Qt::ShiftModifier)) {
        indentShiftTab(cursor);
    } else {
        indentTab(cursor);
    }
    return true;
}

}  // namespace sourceIndent
