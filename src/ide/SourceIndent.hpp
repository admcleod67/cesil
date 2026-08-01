#pragma once

class QKeyEvent;
class QTextCursor;

namespace sourceIndent {

bool isMultilineSelection(const QTextCursor& cursor);

/// Tab: pad with spaces to the next CESIL column (single-line) or indent each
/// selected line by one stop (multiline). Never inserts tab characters.
void indentTab(QTextCursor& cursor);

/// Shift-Tab: outdent leading whitespace only toward the previous stop.
void indentShiftTab(QTextCursor& cursor);

/// Handle Tab / Shift-Tab. Returns true if the event was consumed.
bool handleTabKey(QKeyEvent* event, QTextCursor& cursor);

}  // namespace sourceIndent
