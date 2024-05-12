#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtWebEngine/qtwebengineglobal.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QtWebEngine::initialize();
    QQmlApplicationEngine appEngine;
    appEngine.load(QUrl("main.qml"));

    return app.exec();
}
