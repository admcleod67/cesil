#pragma once

#include <QPlainTextEdit>

class SourceEditor final : public QPlainTextEdit {
    Q_OBJECT

   public:
    explicit SourceEditor(QWidget* parent = nullptr);

   protected:
    void keyPressEvent(QKeyEvent* event) override;
    void changeEvent(QEvent* event) override;

   private:
    void updateTabStopDistance();
};
