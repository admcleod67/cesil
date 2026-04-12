#include <QApplication>
#include <QMainWindow>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QMainWindow window;
    window.setWindowTitle(QStringLiteral("CESIL IDE"));
    window.resize(600, 400);
    window.show();
    return app.exec();
}
