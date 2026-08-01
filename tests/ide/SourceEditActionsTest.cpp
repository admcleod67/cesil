#include "SourceEditActions.hpp"

#include <QAction>
#include <QClipboard>
#include <QGuiApplication>
#include <QPlainTextEdit>
#include <QTest>
#include <QTextCursor>

class SourceEditActionsTest final : public QObject {
    Q_OBJECT

   private slots:
    void init();
    void cleanup();

    void freshEditorEnabledStates();
    void undoRedoAvailability();
    void cutCopyFollowSelection();
    void actionsMutateEditor();

   private:
    SourceEditActions makeActions();

    QPlainTextEdit* m_editor{};
    QAction* m_undo{};
    QAction* m_redo{};
    QAction* m_cut{};
    QAction* m_copy{};
    QAction* m_paste{};
    QAction* m_selectAll{};
};

SourceEditActions SourceEditActionsTest::makeActions() {
    return SourceEditActions{m_undo, m_redo, m_cut, m_copy, m_paste, m_selectAll};
}

void SourceEditActionsTest::init() {
    m_editor = new QPlainTextEdit;
    m_undo = new QAction(m_editor);
    m_redo = new QAction(m_editor);
    m_cut = new QAction(m_editor);
    m_copy = new QAction(m_editor);
    m_paste = new QAction(m_editor);
    m_selectAll = new QAction(m_editor);
    bindSourceEditActions(m_editor, makeActions());
}

void SourceEditActionsTest::cleanup() {
    delete m_editor;
    m_editor = nullptr;
    m_undo = m_redo = m_cut = m_copy = m_paste = m_selectAll = nullptr;
}

void SourceEditActionsTest::freshEditorEnabledStates() {
    QVERIFY(!m_undo->isEnabled());
    QVERIFY(!m_redo->isEnabled());
    QVERIFY(!m_cut->isEnabled());
    QVERIFY(!m_copy->isEnabled());
    QVERIFY(m_selectAll->isEnabled());
    QCOMPARE(m_paste->isEnabled(), m_editor->canPaste());
}

void SourceEditActionsTest::undoRedoAvailability() {
    m_editor->setPlainText(QStringLiteral("abc"));
    // setPlainText clears the undo stack in Qt; insert via cursor to create undo.
    m_editor->clear();
    QTextCursor cursor = m_editor->textCursor();
    cursor.insertText(QStringLiteral("abc"));
    m_editor->setTextCursor(cursor);
    QTRY_VERIFY(m_undo->isEnabled());
    QVERIFY(!m_redo->isEnabled());

    m_undo->trigger();
    QTRY_VERIFY(!m_undo->isEnabled());
    QTRY_VERIFY(m_redo->isEnabled());
    QCOMPARE(m_editor->toPlainText(), QString());

    m_redo->trigger();
    QTRY_VERIFY(m_undo->isEnabled());
    QCOMPARE(m_editor->toPlainText(), QStringLiteral("abc"));
}

void SourceEditActionsTest::cutCopyFollowSelection() {
    m_editor->setPlainText(QStringLiteral("hello"));
    QVERIFY(!m_cut->isEnabled());
    QVERIFY(!m_copy->isEnabled());

    m_editor->selectAll();
    QTRY_VERIFY(m_cut->isEnabled());
    QTRY_VERIFY(m_copy->isEnabled());

    QTextCursor cursor = m_editor->textCursor();
    cursor.clearSelection();
    cursor.movePosition(QTextCursor::Start);
    m_editor->setTextCursor(cursor);
    QTRY_VERIFY(!m_cut->isEnabled());
    QTRY_VERIFY(!m_copy->isEnabled());
}

void SourceEditActionsTest::actionsMutateEditor() {
    QTextCursor cursor = m_editor->textCursor();
    cursor.insertText(QStringLiteral("cut-me"));
    m_editor->setTextCursor(cursor);
    m_editor->selectAll();
    QTRY_VERIFY(m_cut->isEnabled());

    m_cut->trigger();
    QCOMPARE(m_editor->toPlainText(), QString());

    // System clipboard may be unavailable in some CI/sandbox environments.
    if (QClipboard* clipboard = QGuiApplication::clipboard()) {
        clipboard->setText(QStringLiteral("pasted"));
    }
    if (m_editor->canPaste()) {
        QTRY_VERIFY(m_paste->isEnabled());
        m_paste->trigger();
        QCOMPARE(m_editor->toPlainText(), QStringLiteral("pasted"));
    } else {
        cursor = m_editor->textCursor();
        cursor.insertText(QStringLiteral("pasted"));
        m_editor->setTextCursor(cursor);
    }

    cursor = m_editor->textCursor();
    cursor.movePosition(QTextCursor::End);
    cursor.insertText(QStringLiteral("-extra"));
    m_editor->setTextCursor(cursor);
    m_selectAll->trigger();
    QVERIFY(m_editor->textCursor().hasSelection());
    QCOMPARE(m_editor->textCursor().selectedText(), QStringLiteral("pasted-extra"));

    QVERIFY(m_undo->isEnabled());
    m_undo->trigger();
    QVERIFY(m_editor->toPlainText() != QStringLiteral("pasted-extra") ||
            m_editor->document()->isRedoAvailable());
}

QTEST_MAIN(SourceEditActionsTest)
#include "SourceEditActionsTest.moc"
