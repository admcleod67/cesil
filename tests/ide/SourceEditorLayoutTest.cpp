#include "SourceEditorLayout.hpp"

#include <QTest>

class SourceEditorLayoutTest final : public QObject {
    Q_OBJECT

   private slots:
    void visualColumns();
    void tabStops();
    void spacesToNextStop_data();
    void spacesToNextStop();
    void leadingWhitespaceHelpers();
};

void SourceEditorLayoutTest::visualColumns() {
    using sourceEditorLayout::visualColumn;

    QCOMPARE(visualColumn(QStringView{}), 1);
    QCOMPARE(visualColumn(QStringLiteral("        ")), 9);
    QCOMPARE(visualColumn(QStringLiteral("                ")), 17);
    QCOMPARE(visualColumn(QStringLiteral("\t")), 9);
    QCOMPARE(visualColumn(QStringLiteral("\t\t")), 17);
    QCOMPARE(visualColumn(QStringLiteral("ABC\t")), 9);
    QCOMPARE(visualColumn(QStringLiteral("        \t")), 17);
    QCOMPARE(visualColumn(QStringLiteral("LOOP")), 5);
    QCOMPARE(visualColumn(QStringLiteral("LOOP\t")), 9);
}

void SourceEditorLayoutTest::tabStops() {
    using sourceEditorLayout::nextTabStop;
    using sourceEditorLayout::previousTabStop;

    QCOMPARE(nextTabStop(1), 9);
    QCOMPARE(nextTabStop(5), 9);
    QCOMPARE(nextTabStop(9), 17);
    QCOMPARE(nextTabStop(17), 25);
    QCOMPARE(nextTabStop(20), 25);

    QCOMPARE(previousTabStop(1), 1);
    QCOMPARE(previousTabStop(5), 1);
    QCOMPARE(previousTabStop(9), 1);
    QCOMPARE(previousTabStop(17), 9);
    QCOMPARE(previousTabStop(25), 17);
}

void SourceEditorLayoutTest::spacesToNextStop_data() {
    QTest::addColumn<int>("column");
    QTest::addColumn<int>("spaces");

    QTest::newRow("at1") << 1 << 8;
    QTest::newRow("at5") << 5 << 4;
    QTest::newRow("at9") << 9 << 8;
    QTest::newRow("at17") << 17 << 8;
    QTest::newRow("at20") << 20 << 5;
}

void SourceEditorLayoutTest::spacesToNextStop() {
    QFETCH(int, column);
    QFETCH(int, spaces);
    QCOMPARE(sourceEditorLayout::spacesToNextStop(column), spaces);
}

void SourceEditorLayoutTest::leadingWhitespaceHelpers() {
    using sourceEditorLayout::contentStartColumn;
    using sourceEditorLayout::leadingCharsToRemoveForOneStop;
    using sourceEditorLayout::leadingWhitespaceLength;

    QCOMPARE(leadingWhitespaceLength(QStringLiteral("LOAD")), 0);
    QCOMPARE(leadingWhitespaceLength(QStringLiteral("        LOAD")), 8);
    QCOMPARE(leadingWhitespaceLength(QStringLiteral("\tLOAD")), 1);

    QCOMPARE(contentStartColumn(QStringLiteral("LOAD")), 1);
    QCOMPARE(contentStartColumn(QStringLiteral("        LOAD")), 9);
    QCOMPARE(contentStartColumn(QStringLiteral("\tLOAD")), 9);

    QCOMPARE(leadingCharsToRemoveForOneStop(QStringLiteral("LOAD")), 0);
    QCOMPARE(leadingCharsToRemoveForOneStop(QStringLiteral("    LOAD")), 4);
    QCOMPARE(leadingCharsToRemoveForOneStop(QStringLiteral("\tLOAD")), 1);
    QCOMPARE(leadingCharsToRemoveForOneStop(QStringLiteral("        LOAD")), 8);
}

QTEST_MAIN(SourceEditorLayoutTest)
#include "SourceEditorLayoutTest.moc"
