#include "SourceEditor.hpp"
#include "SourceIndent.hpp"

#include <QApplication>
#include <QKeyEvent>
#include <QTest>
#include <QTextCursor>

class SourceIndentTest final : public QObject {
    Q_OBJECT

   private slots:
    void tabAtColumnOneInsertsEightSpaces();
    void tabFromMidFieldPadsToNextStop();
    void shiftTabOutdentsLeadingSpaces();
    void shiftTabOnLabelIsNoOp();
    void multilineIndentAndOutdent();
    void tabNeverInsertsTabCharacters();
    void sourceEditorConsumesTabKey();
};

void SourceIndentTest::tabAtColumnOneInsertsEightSpaces() {
    QPlainTextEdit editor;
    QTextCursor cursor = editor.textCursor();
    sourceIndent::indentTab(cursor);
    editor.setTextCursor(cursor);
    QCOMPARE(editor.toPlainText(), QStringLiteral("        "));
    QCOMPARE(cursor.position(), 8);
    QVERIFY(!editor.toPlainText().contains(QLatin1Char('\t')));
}

void SourceIndentTest::tabFromMidFieldPadsToNextStop() {
    QPlainTextEdit editor;
    editor.setPlainText(QStringLiteral("LOOP"));
    QTextCursor cursor = editor.textCursor();
    cursor.movePosition(QTextCursor::End);
    sourceIndent::indentTab(cursor);
    editor.setTextCursor(cursor);
    QCOMPARE(editor.toPlainText(), QStringLiteral("LOOP    "));
    QCOMPARE(cursor.position(), 8);
}

void SourceIndentTest::shiftTabOutdentsLeadingSpaces() {
    QPlainTextEdit editor;
    editor.setPlainText(QStringLiteral("        LOAD"));
    QTextCursor cursor = editor.textCursor();
    cursor.movePosition(QTextCursor::End);
    sourceIndent::indentShiftTab(cursor);
    editor.setTextCursor(cursor);
    QCOMPARE(editor.toPlainText(), QStringLiteral("LOAD"));
}

void SourceIndentTest::shiftTabOnLabelIsNoOp() {
    QPlainTextEdit editor;
    editor.setPlainText(QStringLiteral("LOAD"));
    QTextCursor cursor = editor.textCursor();
    sourceIndent::indentShiftTab(cursor);
    QCOMPARE(editor.toPlainText(), QStringLiteral("LOAD"));
}

void SourceIndentTest::multilineIndentAndOutdent() {
    QPlainTextEdit editor;
    editor.setPlainText(QStringLiteral("LOAD +0\nSTORE X"));
    QTextCursor cursor = editor.textCursor();
    cursor.movePosition(QTextCursor::Start);
    cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    QVERIFY(sourceIndent::isMultilineSelection(cursor));

    sourceIndent::indentTab(cursor);
    editor.setTextCursor(cursor);
    QCOMPARE(editor.toPlainText(), QStringLiteral("        LOAD +0\n        STORE X"));
    QVERIFY(cursor.hasSelection());

    sourceIndent::indentShiftTab(cursor);
    editor.setTextCursor(cursor);
    QCOMPARE(editor.toPlainText(), QStringLiteral("LOAD +0\nSTORE X"));
    QVERIFY(!editor.toPlainText().contains(QLatin1Char('\t')));
}

void SourceIndentTest::tabNeverInsertsTabCharacters() {
    QPlainTextEdit editor;
    QTextCursor cursor = editor.textCursor();
    sourceIndent::indentTab(cursor);
    sourceIndent::indentTab(cursor);
    editor.setTextCursor(cursor);
    QVERIFY(!editor.toPlainText().contains(QLatin1Char('\t')));
    QCOMPARE(editor.toPlainText(), QStringLiteral("                "));
}

void SourceIndentTest::sourceEditorConsumesTabKey() {
    SourceEditor editor;
    QKeyEvent tab(QEvent::KeyPress, Qt::Key_Tab, Qt::NoModifier);
    QApplication::sendEvent(&editor, &tab);
    QCOMPARE(editor.toPlainText(), QStringLiteral("        "));
    QVERIFY(!editor.toPlainText().contains(QLatin1Char('\t')));
}

QTEST_MAIN(SourceIndentTest)
#include "SourceIndentTest.moc"
