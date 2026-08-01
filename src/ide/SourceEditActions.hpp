#pragma once

class QAction;
class QPlainTextEdit;

/// Actions that always target the Source editor (Milestone 3 Stage 2).
struct SourceEditActions {
    QAction* undo{};
    QAction* redo{};
    QAction* cut{};
    QAction* copy{};
    QAction* paste{};
    QAction* selectAll{};
};

/// Wire \p actions to \p editor: triggered slots, enabled-state signals, and an
/// initial sync. Edit commands always operate on \p editor, not the focus widget.
void bindSourceEditActions(QPlainTextEdit* editor, const SourceEditActions& actions);
