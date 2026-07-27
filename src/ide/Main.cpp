#include <QApplication>

#include "MainWindow.hpp"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationName(QStringLiteral("CESIL IDE"));
    QApplication::setOrganizationName(QStringLiteral("CESIL"));
    QApplication::setApplicationVersion(QStringLiteral(CESIL_VERSION));

    MainWindow window;
    window.resize(600, 400);
    window.show();
    return app.exec();
}
