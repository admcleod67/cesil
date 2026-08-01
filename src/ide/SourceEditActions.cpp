#include "SourceEditActions.hpp"

#include <QAction>
#include <QClipboard>
#include <QGuiApplication>
#include <QPlainTextEdit>
#include <QTextDocument>

namespace {

void syncPasteAction(QPlainTextEdit* editor, QAction* paste) {
    if (paste != nullptr) {
        paste->setEnabled(editor->canPaste());
    }
}

}  // namespace

void bindSourceEditActions(QPlainTextEdit* editor, const SourceEditActions& actions) {
    if (editor == nullptr) {
        return;
    }

    if (actions.undo != nullptr) {
        QObject::connect(actions.undo, &QAction::triggered, editor, &QPlainTextEdit::undo);
        QObject::connect(editor->document(), &QTextDocument::undoAvailable, actions.undo,
                         &QAction::setEnabled);
        actions.undo->setEnabled(editor->document()->isUndoAvailable());
    }
    if (actions.redo != nullptr) {
        QObject::connect(actions.redo, &QAction::triggered, editor, &QPlainTextEdit::redo);
        QObject::connect(editor->document(), &QTextDocument::redoAvailable, actions.redo,
                         &QAction::setEnabled);
        actions.redo->setEnabled(editor->document()->isRedoAvailable());
    }
    if (actions.cut != nullptr) {
        QObject::connect(actions.cut, &QAction::triggered, editor, &QPlainTextEdit::cut);
        QObject::connect(editor, &QPlainTextEdit::copyAvailable, actions.cut, &QAction::setEnabled);
        actions.cut->setEnabled(editor->textCursor().hasSelection());
    }
    if (actions.copy != nullptr) {
        QObject::connect(actions.copy, &QAction::triggered, editor, &QPlainTextEdit::copy);
        QObject::connect(editor, &QPlainTextEdit::copyAvailable, actions.copy,
                         &QAction::setEnabled);
        actions.copy->setEnabled(editor->textCursor().hasSelection());
    }
    if (actions.paste != nullptr) {
        QObject::connect(actions.paste, &QAction::triggered, editor, &QPlainTextEdit::paste);
        if (QClipboard* clipboard = QGuiApplication::clipboard()) {
            QObject::connect(clipboard, &QClipboard::dataChanged, editor, [editor, paste = actions.paste]() {
                syncPasteAction(editor, paste);
            });
        }
        syncPasteAction(editor, actions.paste);
    }
    if (actions.selectAll != nullptr) {
        QObject::connect(actions.selectAll, &QAction::triggered, editor,
                         &QPlainTextEdit::selectAll);
        actions.selectAll->setEnabled(true);
    }
}
