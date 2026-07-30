#include "DiagnosticUtils.hpp"

#include <errors/Diagnostic.hpp>

#include <QTest>
#include <QTextCursor>
#include <QTextDocument>

#include <vector>

class DiagnosticUtilsTest final : public QObject {
    Q_OBJECT

   private slots:
    void modelColumnsAndHeaders();
    void modelDiagnosticsOrderAndMetadata();
    void emptyAndFallbackStates();
    void compilationErrorSummaries();
    void cursorClamping();
};

void DiagnosticUtilsTest::modelColumnsAndHeaders() {
    DiagnosticModel model;
    QCOMPARE(model.columnCount(), 2);
    QCOMPARE(model.headerData(0, Qt::Horizontal).toString(), QStringLiteral("Line"));
    QCOMPARE(model.headerData(1, Qt::Horizontal).toString(),
             QStringLiteral("Description"));
    QCOMPARE(model.rowCount(), 0);
}

void DiagnosticUtilsTest::modelDiagnosticsOrderAndMetadata() {
    DiagnosticModel model;
    std::vector<cesil::Diagnostic> diagnostics{
        {cesil::DiagnosticSeverity::Error, "first", 3, 5},
        {cesil::DiagnosticSeverity::Warning, "second", 8, 0},
    };
    model.setDiagnostics(diagnostics);

    QCOMPARE(model.rowCount(), 2);
    QCOMPARE(model.data(model.index(0, 0)).toString(), QStringLiteral("3"));
    QCOMPARE(model.data(model.index(0, 1)).toString(), QStringLiteral("first"));
    QCOMPARE(model.data(model.index(1, 0)).toString(), QStringLiteral("8"));
    QCOMPARE(model.data(model.index(1, 1)).toString(), QStringLiteral("second"));

    QCOMPARE(model.data(model.index(0, 0), DiagnosticModel::LineRole).toInt(), 3);
    QCOMPARE(model.data(model.index(0, 0), DiagnosticModel::ColumnRole).toInt(), 5);
    QCOMPARE(model.data(model.index(0, 0), DiagnosticModel::SeverityRole).toInt(),
             static_cast<int>(cesil::DiagnosticSeverity::Error));
    QCOMPARE(model.data(model.index(0, 0), DiagnosticModel::NavigableRole).toBool(),
             true);

    QCOMPARE(model.data(model.index(1, 0), DiagnosticModel::ColumnRole).toInt(), 0);
    QCOMPARE(model.data(model.index(1, 0), DiagnosticModel::SeverityRole).toInt(),
             static_cast<int>(cesil::DiagnosticSeverity::Warning));
    QVERIFY(model.isNavigable(0));
    QVERIFY(model.isNavigable(1));
}

void DiagnosticUtilsTest::emptyAndFallbackStates() {
    DiagnosticModel model;
    model.setFallbackMessage(QStringLiteral("Compilation failed (no detailed diagnostics)."));
    QCOMPARE(model.rowCount(), 1);
    QCOMPARE(model.data(model.index(0, 0)).toString(), QString());
    QCOMPARE(model.data(model.index(0, 1)).toString(),
             QStringLiteral("Compilation failed (no detailed diagnostics)."));
    QCOMPARE(model.data(model.index(0, 0), DiagnosticModel::NavigableRole).toBool(),
             false);
    QVERIFY(!model.isNavigable(0));

    model.clear();
    QCOMPARE(model.rowCount(), 0);
}

void DiagnosticUtilsTest::compilationErrorSummaries() {
    QCOMPARE(compilationErrorSummary(0), QStringLiteral("No compilation errors."));
    QCOMPARE(compilationErrorSummary(1), QStringLiteral("1 compilation error"));
    QCOMPARE(compilationErrorSummary(2), QStringLiteral("2 compilation errors"));
}

void DiagnosticUtilsTest::cursorClamping() {
    QTextDocument document;
    document.setPlainText(QStringLiteral("abc\ndef\nghi"));

    {
        const QTextCursor cursor = cursorForDiagnostic(&document, 2, 2);
        QCOMPARE(cursor.blockNumber(), 1);
        QCOMPARE(cursor.positionInBlock(), 1);
    }
    {
        // Column 0 means start of line.
        const QTextCursor cursor = cursorForDiagnostic(&document, 3, 0);
        QCOMPARE(cursor.blockNumber(), 2);
        QCOMPARE(cursor.positionInBlock(), 0);
    }
    {
        // Past end of document clamps to last line / last column.
        const QTextCursor cursor = cursorForDiagnostic(&document, 99, 99);
        QCOMPARE(cursor.blockNumber(), 2);
        QCOMPARE(cursor.positionInBlock(), 3);
    }
    {
        const QTextCursor cursor = cursorForDiagnostic(&document, 1, 50);
        QCOMPARE(cursor.blockNumber(), 0);
        QCOMPARE(cursor.positionInBlock(), 3);
    }
}

QTEST_GUILESS_MAIN(DiagnosticUtilsTest)
#include "DiagnosticUtilsTest.moc"
